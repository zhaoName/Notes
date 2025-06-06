
# RunLoop

<br>


## 一、RunLoop 概念

`RunLoop `顾名思义就是运行循环，它能使程序不会马上退出，保持在运行状态。有消息到来时会立刻被唤醒处理消息，没要消息处理时会进入休眠状态以避免浪费资源。

`OSX/iOS `系统中，提供了两个这样的对象：`NSRunLoop` 和 `CFRunLoopRef`。
`CFRunLoopRef` 是在 `CoreFoundation` 框架内的，它提供了纯 C 函数的 API，所有这些 API 都是线程安全的。`NSRunLoop` 是基于 `CFRunLoopRef` 的封装，提供了面向对象的 API，但是这些 API 不是线程安全的。

```Objective-C
NSLog(@"%p %p", [NSRunLoop currentRunLoop], [NSRunLoop mainRunLoop]);
NSLog(@"%p %p", CFRunLoopGetCurrent(), CFRunLoopGetMain());

// 打印结果
2019-07-10 11:50:34.482015 RunLoopDemo[733:70653] 0x1740bb3c0 0x1740bb3c0
2019-07-10 11:50:34.482115 RunLoopDemo[733:70653] 0x17417b900 0x17417b900
```

从打印结果上看`NSRunLoop `和`CFRunLoopRef `获取的`RunLoop`不是一个。

```Objective-C
NSLog(@"%@", [NSRunLoop mainRunLoop]);

// 打印结果
2019-07-10 11:50:34.514571 RunLoopDemo[733:70653] <CFRunLoop 0x17417b900 [0x1b88d4bb8]>{wakeup port = 0x1d03, stopped = false, ignoreWakeUps = false, 
current mode = UIInitializationRunLoopMode,
common modes = <CFBasicHash 0x17404dd10 [0x1b88d4bb8]>{type = mutable set, count = 2,
entries =>
	0 : <CFString 0x1b3211020 [0x1b88d4bb8]>{contents = "UITrackingRunLoopMode"}
	2 : <CFString 0x1b27a1470 [0x1b88d4bb8]>{contents = "kCFRunLoopDefaultMode"}
}
...
```

这样就解释了`NSRunLoop`是基于`CFRunLoopRef `的封装。


`CFRunLoopRef`的代码是开源的，你可以在这里 `http://opensource.apple.com/tarballs/CF/` 下载到整个 CoreFoundation 的源码来查看。

本篇文章所用源码为`CF-1153.18`。

## 二、RunLoop 和线程的关系

iOS 开发中能遇到两个线程对象:`pthread_t ` 和`NSThread `，它俩是一对一的关系。你可以通过`pthread_main_thread_np()`或`[NSThread mainThread]`来获取主线程；也可以通过`pthread_self() `或`[NSThread currentThread]`来获取当前线程。`CFRunLoop` 是基于`pthread ` 来管理的。

在`CF-1153.18`的`CFRunLoop.h`中可以看到并没有暴露创建`RunLoop` 的结构。只提供了两个获取`RunLoop` 的函数：`CFRunLoopGetCurrent()`和`CFRunLoopGetMain()`。它们俩的内部实现逻辑大致如下：

```Objective-C
static CFMutableDictionaryRef __CFRunLoops = NULL;
static CFLock_t loopsLock = CFLockInit;

// should only be called by Foundation
// t==0 is a synonym for "main thread" that always works
CF_EXPORT CFRunLoopRef _CFRunLoopGet0(pthread_t t)
{
    if (pthread_equal(t, kNilPthreadT)) {
        t = pthread_main_thread_np();
    }
    __CFLock(&loopsLock);
    if (!__CFRunLoops) {
        __CFUnlock(&loopsLock);
        // 创建字典
        CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorSystemDefault, 0, NULL, &kCFTypeDictionaryValueCallBacks);
        // 由主线程创建主线上的 RunLoop
        CFRunLoopRef mainLoop = __CFRunLoopCreate(pthread_main_thread_np());
        // 主线程为 key，RunLoop 为 value 存入字典
        CFDictionarySetValue(dict, pthreadPointer(pthread_main_thread_np()), mainLoop);
        if (!OSAtomicCompareAndSwapPtrBarrier(NULL, dict, (void * volatile *)&__CFRunLoops)) {
            CFRelease(dict);
        }
        CFRelease(mainLoop);
        __CFLock(&loopsLock);
    }
    
    // 由传进来的线程 取 loop
    CFRunLoopRef loop = (CFRunLoopRef)CFDictionaryGetValue(__CFRunLoops, pthreadPointer(t));
    __CFUnlock(&loopsLock);
    if (!loop) {
        // 取不出来就创建一个
        CFRunLoopRef newLoop = __CFRunLoopCreate(t);
        __CFLock(&loopsLock);
        loop = (CFRunLoopRef)CFDictionaryGetValue(__CFRunLoops, pthreadPointer(t));
        if (!loop) {
            CFDictionarySetValue(__CFRunLoops, pthreadPointer(t), newLoop);
            loop = newLoop;
        }
        // don't release run loops inside the loopsLock, because CFRunLoopDeallocate may end up taking it
        __CFUnlock(&loopsLock);
        CFRelease(newLoop);
    }
    if (pthread_equal(t, pthread_self())) {
        // 注册一个回调，当线程销毁时，顺便也销毁其对应的 RunLoop 
        _CFSetTSD(__CFTSDKeyRunLoop, (void *)loop, NULL);
        if (0 == _CFGetTSD(__CFTSDKeyRunLoopCntr)) {
            _CFSetTSD(__CFTSDKeyRunLoopCntr, (void *)(PTHREAD_DESTRUCTOR_ITERATIONS-1), (void (*)(void *))__CFFinalizeRunLoop);
        }
    }
    return loop;
}

CFRunLoopRef CFRunLoopGetMain(void) {
    CHECK_FOR_FORK();
    static CFRunLoopRef __main = NULL; // no retain needed
    if (!__main) __main = _CFRunLoopGet0(pthread_main_thread_np()); // no CAS needed
    return __main;
}

CFRunLoopRef CFRunLoopGetCurrent(void) {
    CHECK_FOR_FORK();
    CFRunLoopRef rl = (CFRunLoopRef)_CFGetTSD(__CFTSDKeyRunLoop);
    if (rl) return rl;
    return _CFRunLoopGet0(pthread_self());
}
```

由源码可知：

- 线程和`RunLoop` 是一对一的关系，保存在一个静态全局字典中，线程为`key `，`RunLoop`为`value `

- `RunLoop`的创建类似懒加载，刚创建的线程没有`RunLoop`，若不主动获取那它一直不会有

- `RunLoop`的创建是发生在第一次获取时，`RunLoop`的销毁是发生在线程结束时

- 只能在某个线程内容获取其`RunLoop`，主线程除外。


## 三、RunLoop 相关类

在`CoreFoundation`里面关于`RunLoop`有5个类:

- `CFRunLoopRef`

- `CFRunLoopModeRef`

- `CFRunLoopSourceRef`

- `CFRunLoopTimerRef`

- `CFRunLoopObserverRef`

其中`CFRunLoopModeRef`类并没有对外暴露，只是通过`CFRunLoopRef`的接口进行了封装。他们的关系如下:

![](../Images/iOS/RunLoop/RunLoop_image0101.png)

一个`RunLoop` 包含若干个`Mode `，每个`Mode ` 又包含若干个`Source/Timer/Observer`。每次调用`RunLoop` 的主函数时，只能指定其中一个`Mode `，这个`Mode `被称作 `CurrentMode`。如果需要切换`Mode`，只能退出`Loop `，再重新指定一个`Mode ` 进入。这样做主要是为了分隔开不同组的`Source/Timer/Observer`，让其互不影响。

### 0x01 CFRunLoopSourceRef

`CFRunLoopSourceRef` 是事件产生的地方。`Source`有两个版本：`Source0` 和 `Source1`。

- `Source0` 只包含了一个回调（函数指针），它并不能主动触发事件。使用时，你需要先调用`CFRunLoopSourceSignal(source)`，将这个`Source` 标记为待处理，然后手动调用`CFRunLoopWakeUp(runloop)` 来唤醒`RunLoop`，让其处理这个事件。

- `Source1` 包含了一个`mach_port`和一个回调（函数指针），被用于通过内核和其他线程相互发送消息。这种`Source `能主动唤醒`RunLoop `的线程，其原理在下面会讲到。


### 0x02 CFRunLoopTimerRef

`CFRunLoopTimerRef`是基于时间的触发器。其包含一个时间长度和一个回调（函数指针）。当其加入到`RunLoop `时，`RunLoop `会注册对应的时间点，当时间点到时，`RunLoop`会被唤醒以执行那个回调。

### 0x03 CFRunLoopModeRef

`CFRunLoopMode`和`CFRunLoop`的底层结构大致如下

```Objective-C
// CFRunLoop.c

struct __CFRunLoopMode {
	// Mode Name, 例如 @"kCFRunLoopDefaultMode"
    CFStringRef _name;            
    CFMutableSetRef _sources0;    // Set
    CFMutableSetRef _sources1;    // Set
    CFMutableArrayRef _observers; // Array
    CFMutableArrayRef _timers;    // Array
    ...
};

struct __CFRunLoop {
    CFMutableSetRef _commonModes;     // Set
    CFMutableSetRef _commonModeItems; // Set<Source/Observer/Timer>
    CFRunLoopModeRef _currentMode;    // Current`RunLoop` Mode
    CFMutableSetRef _modes;           // Set
    ...
};
```

- `RunLoop`只能选择一个`mode`作为`_currentMode`。而且源码中也提供接口获取`_currentMode `。

```Objective-C
CF_EXPORT CFStringRef CFRunLoopCopyCurrentMode(CFRunLoopRef rl);

CF_EXPORT CFArrayRef CFRunLoopCopyAllModes(CFRunLoopRef rl);
```

- `CommonModes `：一个`Mode `可以将自己标记为`Common `属性（通过将其`ModeName`添加到`RunLoop`的`commonModes`中）。每当`RunLoop `的内容发生变化时，`RunLoop `都会自动将 `_commonModeItems` 里的`Source/Observer/Timer`同步到具有`Common `标记的所有`Mode`里。

```Objective-C
CFRunLoopAddCommonMode(CFRunLoopRef`RunLoop`, CFStringRef modeName);

CFRunLoopRunInMode(CFStringRef mode, CFTimeInterval seconds, Boolean returnAfterSourceHandled)
```

- 苹果公开的`Mode`有两个：`kCFRunLoopDefaultMode(NSRunLoopCommonModes)`和`UITrackingRunLoopMode `。

	- `kCFRunLoopDefaultMode(NSRunLoopCommonModes)`：`App`的默认`Mode`，通常主线程是在这个`Mode`下运行

	- `UITrackingRunLoopMode`：用于`ScrollView` 追踪触摸滑动，保证界面滑动时不受其他 `Mode` 影响

- 还有个`kCFRunLoopCommonModes `，但它不是`Mode`，它是一个标记`Common`的字符串。

- 上面的`Source/Timer/Observer` 被统称为`mode item`，一个`item `可以被同时加入多个`mode `。但一个`item ` 被重复加入同一个`mode ` 时是不会有效果的。如果一个 mode 中一个`item `都没有，则`RunLoop `会直接退出，不进入循环。


### 0x03 CFRunLoopObserverRef

`CFRunLoopObserverRef`是观察者，每个`Observer ` 都包含了一个回调（函数指针），当`RunLoop `的状态发生变化时，观察者就能通过回调接受到这个变化。可以观测的时间点有以下几个

```Objective-C
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
    kCFRunLoopEntry         = (1UL << 0), // 即将进入 Loop
    kCFRunLoopBeforeTimers  = (1UL << 1), // 即将处理 Timer
    kCFRunLoopBeforeSources = (1UL << 2), // 即将处理 Source
    kCFRunLoopBeforeWaiting = (1UL << 5), // 即将进入休眠
    kCFRunLoopAfterWaiting  = (1UL << 6), // 刚从休眠中唤醒
    kCFRunLoopExit          = (1UL << 7), // 即将退出 Loop
    kCFRunLoopAllActivities = 0x0FFFFFFFU // 所有 Activity
};
```

- 监听定时器

```Objective-C
void observerRunLoopActivity(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void *info)
{
    switch (activity) {
        case kCFRunLoopEntry:
            NSLog(@"kCFRunLoopEntry");
            break;
        case kCFRunLoopBeforeTimers:
            NSLog(@"kCFRunLoopBeforeTimers");
            break;
        case kCFRunLoopBeforeSources:
            NSLog(@"kCFRunLoopBeforeSources");
            break;
        case kCFRunLoopBeforeWaiting:
            NSLog(@"kCFRunLoopBeforeWaiting");
            break;
        case kCFRunLoopAfterWaiting:
            NSLog(@"kCFRunLoopAfterWaiting");
            break;
        case kCFRunLoopExit:
            NSLog(@"kCFRunLoopExit");
            break;
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    CFRunLoopObserverRef ob =  CFRunLoopObserverCreate(CFAllocatorGetDefault(), kCFRunLoopAllActivities, YES, 0, &observerRunLoopActivity, nil);
    CFRunLoopAddObserver(CFRunLoopGetMain(), ob, kCFRunLoopCommonModes);
    CFRelease(ob);
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [NSTimer scheduledTimerWithTimeInterval:2 repeats:YES block:^(NSTimer * _Nonnull timer) {
        NSLog(@"timer----------");
    }];
}

// 点击屏幕后 打印结果
...
2019-07-11 00:06:03.587942 RunLoopDemo[625:200646] kCFRunLoopBeforeWaiting
2019-07-11 00:06:05.546818 RunLoopDemo[625:200646] kCFRunLoopAfterWaiting
2019-07-11 00:06:05.547063 RunLoopDemo[625:200646] timer----------
2019-07-11 00:06:05.547215 RunLoopDemo[625:200646] kCFRunLoopBeforeTimers
2019-07-11 00:06:05.547300 RunLoopDemo[625:200646] kCFRunLoopBeforeSources
2019-07-11 00:06:05.547404 RunLoopDemo[625:200646] kCFRunLoopBeforeWaiting
2019-07-11 00:06:07.547074 RunLoopDemo[625:200646] kCFRunLoopAfterWaiting
2019-07-11 00:06:07.547857 RunLoopDemo[625:200646] timer----------
2019-07-11 00:06:07.548004 RunLoopDemo[625:200646] kCFRunLoopBeforeTimers
2019-07-11 00:06:07.548088 RunLoopDemo[625:200646] kCFRunLoopBeforeSources
2019-07-11 00:06:07.548192 RunLoopDemo[625:200646] kCFRunLoopBeforeWaiting
...
```

- 监听`Mode`切换

屏幕上放个`UITextView`，滑动监听`Mode`切换

```Objective-C
CFRunLoopObserverRef ob = CFRunLoopObserverCreateWithHandler(CFAllocatorGetDefault(), kCFRunLoopAllActivities, YES, 0, ^(CFRunLoopObserverRef observer, CFRunLoopActivity activity) {
    switch (activity) {
        case kCFRunLoopEntry:
            NSLog(@"kCFRunLoopEntry===%@", CFRunLoopCopyCurrentMode(CFRunLoopGetCurrent()));
            break;
        case kCFRunLoopExit:
            NSLog(@"kCFRunLoopExit===%@", CFRunLoopCopyCurrentMode(CFRunLoopGetCurrent()));
            break;
        default:
            break;
    }
});
CFRunLoopAddObserver(CFRunLoopGetMain(), ob, kCFRunLoopCommonModes);
CFRelease(ob);

// 滑动 UITextView 打印结果
2019-07-11 00:15:51.626812 RunLoopDemo[647:203273] kCFRunLoopEntry===kCFRunLoopDefaultMode
// 开始滑动
2019-07-11 00:15:57.473468 RunLoopDemo[647:203273] kCFRunLoopExit===kCFRunLoopDefaultMode
2019-07-11 00:15:57.474280 RunLoopDemo[647:203273] kCFRunLoopEntry===UITrackingRunLoopMode
// 结束滑动
2019-07-11 00:16:06.588931 RunLoopDemo[647:203273] kCFRunLoopExit===UITrackingRunLoopMode
2019-07-11 00:16:06.589225 RunLoopDemo[647:203273] kCFRunLoopEntry===kCFRunLoopDefaultMode    
```

这样也验证了`RunLoop`只能指定其中一个`Mode `，如果需要切换`Mode`，只能退出`Loop `，再重新指定一个`Mode ` 进入。

## 四、RunLoop 的内部逻辑

根据苹果在[文档](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Multithreading/RunLoopManagement/RunLoopManagement.html#//apple_ref/doc/uid/10000057i-CH16-SW23)里的说明，RunLoop 内部的逻辑大致如下:

![](../Images/iOS/RunLoop/RunLoop_image0102.png)


虽然我们可以在苹果开源的代码中查看`RunLoop`源码，但不知道该从哪入手。有个取巧的办法：在`viewDidLoad`中下断点，查看函数调用栈

![](../Images/iOS/RunLoop/RunLoop_image0103.png)


可以看到`CoreFoundation`中`RunLoop`的入口函数是`CFRunLoopRunSpecific `。那我们就可以从这里入手查看其源码。

其内部代码整理如下(只保留了主要逻辑代码)

```Objective-C
// CFRunLoop.c

// 用 kCFRunLoopDefaultMode 启动 RunLoop
void CFRunLoopRun(void) {
    int32_t result;
    do {
        result = CFRunLoopRunSpecific(CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 1.0e10, false);
        CHECK_FOR_FORK();
    } while (kCFRunLoopRunStopped != result && kCFRunLoopRunFinished != result);
}

// 用指定的 Mode 启动启动 RunLoop 并允许设置 RunLoop 超时时间
SInt32 CFRunLoopRunInMode(CFStringRef modeName, CFTimeInterval seconds, Boolean returnAfterSourceHandled) {     /* DOES CALLOUT */
    CHECK_FOR_FORK();
    return CFRunLoopRunSpecific(CFRunLoopGetCurrent(), modeName, seconds, returnAfterSourceHandled);
}

SInt32 CFRunLoopRunSpecific(CFRunLoopRef rl, CFStringRef modeName, CFTimeInterval seconds, Boolean returnAfterSourceHandled)
{
    // 首先根据 modeName 找到对应 mode
    CFRunLoopModeRef currentMode = __CFRunLoopFindMode(rl, modeName, false);
    // currentMode 为空或 currentMode 里没有 source/timer/observer, 就结束
    if (NULL == currentMode || __CFRunLoopModeIsEmpty(rl, currentMode, rl->_currentMode)) {
        Boolean did = false;
        if (currentMode) __CFRunLoopModeUnlock(currentMode);
        __CFRunLoopUnlock(rl);
        return did ? kCFRunLoopRunHandledSource : kCFRunLoopRunFinished;
    }
    
    // 1. 通知 Observers: RunLoop 即将进入 loop
	if (currentMode->_observerMask & kCFRunLoopEntry ) __CFRunLoopDoObservers(rl, currentMode, kCFRunLoopEntry);
    
    // RunLoop 处理各种事件
	result = __CFRunLoopRun(rl, currentMode, seconds, returnAfterSourceHandled, previousMode);
    
    // 10. 通知 observers RunLoop 即将推出
	if (currentMode->_observerMask & kCFRunLoopExit ) __CFRunLoopDoObservers(rl, currentMode, kCFRunLoopExit);
    return result;
}

static int32_t __CFRunLoopRun(CFRunLoopRef rl, CFRunLoopModeRef rlm, CFTimeInterval seconds, Boolean stopAfterHandle, CFRunLoopModeRef previousMode)
{
    int32_t retVal = 0;
    do {
        
        // 2.通知 Observers: RunLoop 即将触发 Timer
        if (rlm->_observerMask & kCFRunLoopBeforeTimers) __CFRunLoopDoObservers(rl, rlm, kCFRunLoopBeforeTimers);
        // 3.通知 Observers: RunLoop 即将触发 Source0 (非port) 回调。
        if (rlm->_observerMask & kCFRunLoopBeforeSources) __CFRunLoopDoObservers(rl, rlm, kCFRunLoopBeforeSources);
        // 执行被加入的 block
        __CFRunLoopDoBlocks(rl, rlm);
        
        // 4. RunLoop 触发 Source0 (非port) 回调
        Boolean sourceHandledThisLoop = __CFRunLoopDoSources0(rl, rlm, stopAfterHandle);
        if (sourceHandledThisLoop) {
            // 执行被加入的 block
            __CFRunLoopDoBlocks(rl, rlm);
        }

        // 5. 如果有 Source1 (基于port) 处于 ready 状态，直接处理这个 Source1 然后跳转去处理消息。
        if (__CFRunLoopServiceMachPort(dispatchPort, &msg, sizeof(msg_buffer), &livePort, 0, &voucherState, NULL)) {
            goto handle_msg;
        }
        
        // 6. 通知 Observers: RunLoop 的线程即将进入休眠(sleep)。
        __CFRunLoopDoObservers(rl, rlm, kCFRunLoopBeforeWaiting);
        __CFRunLoopSetSleeping(rl);
        
        // 7. 调用 mach_msg 等待接受 mach_port 的消息。线程将进入休眠, 直到被下面某一个事件唤醒。
        // • 一个基于 port 的Source 的事件。
        // • 一个 Timer 到时间了
        // • RunLoop 自身的超时时间到了
        // • 被其他什么调用者手动唤醒
        __CFRunLoopServiceMachPort(waitSet, &msg, sizeof(msg_buffer), &livePort, poll ? 0 : TIMEOUT_INFINITY, &voucherState, &voucherCopy);
           
        // 8.通知 Observers: RunLoop 的线程刚刚被唤醒了
        __CFRunLoopUnsetSleeping(rl);
        __CFRunLoopDoObservers(rl, rlm, kCFRunLoopAfterWaiting);
        
        // 被唤醒后 收到消息 处理消息
        handle_msg:;
        __CFRunLoopSetIgnoreWakeUps(rl);
        
        if (msg_is_timer) {
            // 9.1 被 timer 唤醒，触发 timer 的回调，处理 timer
            if (!__CFRunLoopDoTimers(rl, rlm, mach_absolute_time())) {
                // Re-arm the next timer, because we apparently fired early
                __CFArmNextTimerInMode(rlm, rl);
            }
        }
        else if (livePort == dispatchPort) {
            // 9.2 被 GCD 唤醒，执行 block
            __CFRUNLOOP_IS_SERVICING_THE_MAIN_DISPATCH_QUEUE__(msg);
        }
        else {
            // 9.3 如果一个 Source1 (基于port) 发出事件了，处理这个事件
            CFRunLoopSourceRef rls = __CFRunLoopModeFindSourceForMachPort(rl, rlm, livePort);
            if (rls) {
                mach_msg_header_t *reply = NULL;
                sourceHandledThisLoop = __CFRunLoopDoSource1(rl, rlm, rls, msg, msg->msgh_size, &reply) || sourceHandledThisLoop;
                if (NULL != reply) {
                    (void)mach_msg(reply, MACH_SEND_MSG, reply->msgh_size, 0, MACH_PORT_NULL, 0, MACH_PORT_NULL);
                    CFAllocatorDeallocate(kCFAllocatorSystemDefault, reply);
                }
            }
        }
        // 执行加入到Loop的block
        __CFRunLoopDoBlocks(rl, rlm);
        

        if (sourceHandledThisLoop && stopAfterHandle) {
            // 进入loop时参数说处理完事件就返回
            retVal = kCFRunLoopRunHandledSource;
        }
        else if (timeout_context->termTSR < mach_absolute_time()) {
            // 超出传入参数标记的超时时间了
            retVal = kCFRunLoopRunTimedOut;
        }
        else if (__CFRunLoopIsStopped(rl)) {
            // 被外部调用者强制停止了
            __CFRunLoopUnsetStopped(rl);
            retVal = kCFRunLoopRunStopped;
        }
        else if (rlm->_stopped) {
            // 被外部调用者强制停止了
            rlm->_stopped = false;
            retVal = kCFRunLoopRunStopped;
        }
        else if (__CFRunLoopModeIsEmpty(rl, rlm, previousMode)) {
            // source/timer/observer一个都没有了
            retVal = kCFRunLoopRunFinished;
        }
        
        // 如果没超时，mode里没空，loop也没被停止，那继续loop。
    } while (0 == retVal);
    
    return retVal;
}
```

可以看到，实际上`RunLoop `就是这样一个函数，其内部是一个`do-while `循环。当你调用`CFRunLoopRun() `时，线程就会一直停留在这个循环里；直到超时或被手动停止，该函数才会返回。


## 五、线程保活


### 0x01 `[[NSRunLoop currentRunLoop] run]`

我们来看下苹果文档上对`run`方法的解释

> If no input sources or timers are attached to the run loop, this method exits immediately; otherwise, it runs the receiver in the NSDefaultRunLoopMode by repeatedly invoking runMode:beforeDate:. In other words, this method effectively begins an infinite loop that processes data from the run loop’s input sources and timers.

> Manually removing all known input sources and timers from the run loop is not a guarantee that the run loop will exit. macOS can install and remove additional input sources as needed to process requests targeted at the receiver’s thread. Those sources could therefore prevent the run loop from exiting.

> If you want the run loop to terminate, you shouldn't use this method. Instead, use one of the other run methods and also check other arbitrary conditions of your own, in a loop. A simple example would be:

```Objective-C
BOOL shouldKeepRunning = YES; // global
NSRunLoop *theRL = [NSRunLoop currentRunLoop];
while (shouldKeepRunning && [theRL runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]]);
```
> where shouldKeepRunning is set to NO somewhere else in the program.

- 若`RunLoop `中没有`observer/port/timer`，`run`方法会马上结束

- `run`方法内部会重复调用`runMode:beforeDate:`

- 若想手动控制`RunLoop`的开启和销毁，则不能调用`run`，而应该像示例`while`中调用`runMode:beforeDate:`


### 0x02 具体实现

```Objective-C
// ZNThread.m
- (void)dealloc
{
    NSLog(@"%s", __func__);
}

// ViewController.m
@property (nonatomic, strong) ZNThread *thread;/**< */
@property (nonatomic, assign) BOOL shouldKeepRunning;/**< */

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.shouldKeepRunning = YES;
    self.thread = [[ZNThread alloc] initWithTarget:self selector:@selector(startRunLoop) object:nil];
    [self.thread start];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self stopRunLoop];
}

- (void)startRunLoop
{
    NSLog(@"%s----%@", __func__, [NSThread currentThread]);
    [[NSRunLoop currentRunLoop] addPort:[NSPort new] forMode:NSDefaultRunLoopMode];
    while (self.shouldKeepRunning) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
    NSLog(@"RunLoop end-----");
}

/// 手动结束 RunLoop
- (IBAction)stopRunLoop
{
    if (self.thread == nil) return;
    
    // 这里注意 waitUntilDone 一定要设置为YES
    // 否则会出现控制器已销毁，再去销毁 RunLoop，这回导致坏内存访问
    [self performSelector:@selector(stop) onThread:self.thread withObject:nil waitUntilDone:YES];
    NSLog(@"end-----%@", [NSThread currentThread]);
}

- (void)stop
{
    _shouldKeepRunning = NO;
    CFRunLoopStop(CFRunLoopGetCurrent());
    // 为防止重复销毁 RunLoop
    self.thread = nil;
    NSLog(@"%s---%@", __func__, [NSThread currentThread]);
}

// 子线程中做事情
- (void)threadKeepLive
{
    NSLog(@"%s---%@", __func__, [NSThread currentThread]);
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self performSelector:@selector(threadKeepLive) onThread:self.thread withObject:nil waitUntilDone:NO];
}

- (void)dealloc
{
    NSLog(@"%s", __func__);
}
```

- 手动销毁`RunLoop`

```Objective-C
2019-07-12 15:41:28.699297 RunLoopDemo[758:113100] -[ViewController startRunLoop]----<ZNThread: 0x170271ec0>{number = 3, name = (null)}

2019-07-12 15:41:30.475383 RunLoopDemo[758:113100] -[ViewController threadKeepLive]---<ZNThread: 0x170271ec0>{number = 3, name = (null)}
2019-07-12 15:41:30.640859 RunLoopDemo[758:113100] -[ViewController threadKeepLive]---<ZNThread: 0x170271ec0>{number = 3, name = (null)}
2019-07-12 15:41:30.822796 RunLoopDemo[758:113100] -[ViewController threadKeepLive]---<ZNThread: 0x170271ec0>{number = 3, name = (null)}

2019-07-12 15:41:36.400358 RunLoopDemo[758:113100] -[ViewController stop]---<ZNThread: 0x170271ec0>{number = 3, name = (null)}
2019-07-12 15:41:36.400611 RunLoopDemo[758:113100] RunLoop end-----
2019-07-12 15:41:36.401533 RunLoopDemo[758:113007] -[ZNThread dealloc]
2019-07-12 15:41:36.402046 RunLoopDemo[758:113007] end-----<NSThread: 0x170079a80>{number = 1, name = main}

2019-07-12 15:41:39.769626 RunLoopDemo[758:113007] -[ViewController dealloc]
```

- 销毁`ViewController`后，`RunLoop`也会自动销毁

```Objective-C
2019-07-12 15:43:14.318075 RunLoopDemo[761:113546] -[ViewController startRunLoop]----<ZNThread: 0x17007fa40>{number = 3, name = (null)}

2019-07-12 15:43:15.248557 RunLoopDemo[761:113546] -[ViewController threadKeepLive]---<ZNThread: 0x17007fa40>{number = 3, name = (null)}
2019-07-12 15:43:15.444342 RunLoopDemo[761:113546] -[ViewController threadKeepLive]---<ZNThread: 0x17007fa40>{number = 3, name = (null)}
2019-07-12 15:43:15.610601 RunLoopDemo[761:113546] -[ViewController threadKeepLive]---<ZNThread: 0x17007fa40>{number = 3, name = (null)}

2019-07-12 15:43:17.623291 RunLoopDemo[761:113546] -[ViewController stop]---<ZNThread: 0x17007fa40>{number = 3, name = (null)}
2019-07-12 15:43:17.623553 RunLoopDemo[761:113546] RunLoop end-----
2019-07-12 15:43:17.626384 RunLoopDemo[761:113491] -[ZNThread dealloc]
2019-07-12 15:43:17.626794 RunLoopDemo[761:113491] end-----<NSThread: 0x17406a500>{number = 1, name = main}

2019-07-12 15:43:18.138968 RunLoopDemo[761:113491] -[ViewController dealloc]
```

## 六、苹果用 RunLoop 实现的功能

### 0x01 `PerformSelecter `

- 当调用`NSObject ` 的`performSelecter:afterDelay:`后，实际上其内部会创建一个`Timer `并添加到当前线程的`RunLoop `中。所以如果当前线程没有`RunLoop `，则这个方法会失效。

> This method sets up a timer to perform the aSelector message on the current thread’s run loop. The timer is configured to run in the default mode (NSDefaultRunLoopMode). When the timer fires, the thread attempts to dequeue the message from the run loop and perform the selector. It succeeds if the run loop is running and in the default mode; otherwise, the timer waits until the run loop is in the default mode

- 当调用`performSelector:onThread:`时，实际上其会创建一个`Timer `加到对应的线程去，同样的，如果对应线程没有`RunLoop `该方法也会失效。


### 0x02 `GCD`


`GCD `提供的某些接口也用到了`RunLoop `， 例如 `dispatch_async()`。当调用`dispatch_async(dispatch_get_main_queue(), block)`时，`libDispatch`会向主线程的`RunLoop `发送消息，`RunLoop `会被唤醒，并从消息中取得这个`block `，并在回调 `__CFRUNLOOP_IS_SERVICING_THE_MAIN_DISPATCH_QUEUE__()` 里执行这个 block。但这个逻辑仅限于`dispatch ` 到主线程，`dispatch `到其他线程仍然是由`libDispatch `处理的。

### 0x03 `AutoreleasePool`

`App `启动后，苹果在主线程`RunLoop ` 里注册了两个`Observer `，其回调都是 `_wrapRunLoopWithAutoreleasePoolHandler()`。

第一个 `Observer` 监视的事件是 `Entry`(即将进入`Loop`)，其回调内会调用`_objc_autoreleasePoolPush()` 创建自动释放池。其 `order` 是-2147483647，优先级最高，保证创建释放池发生在其他所有回调之前。

第二个 Observer 监视了两个事件： BeforeWaiting(准备进入休眠) 时调用`_objc_autoreleasePoolPop()`和`_objc_autoreleasePoolPush() `释放旧的池并创建新池；Exit(即将退出Loop) 时调用` _objc_autoreleasePoolPop()` 来释放自动释放池。这个`Observer ` 的`order ` 是 2147483647，优先级最低，保证其释放池子发生在其他所有回调之后。

在主线程执行的代码，通常是写在诸如事件回调、`Timer `回调内的。这些回调会被`RunLoop ` 创建好的`AutoreleasePool ` 环绕着，所以不会出现内存泄漏，开发者也不必显示创建`Pool `了。


<br>

参考：[深入理解RunLoop](https://blog.ibireme.com/2015/05/18/runloop/)

写于2019-07-10

<br>