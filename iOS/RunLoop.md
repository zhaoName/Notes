
# RunLoop

<br>


## 一、RunLoop 概念

`RunLoop `顾名思义就是运行循环，它能使程序不会马上退出，保持在运行状态。有消息到来时会立刻被唤醒处理消息，没要消息处理时会进入休眠状态以避免浪费资源。

OSX/iOS 系统中，提供了两个这样的对象：`NSRunLoop` 和 `CFRunLoopRef`。
`CFRunLoopRef` 是在 `CoreFoundation` 框架内的，它提供了纯 C 函数的 API，所有这些 API 都是线程安全的。`NSRunLoop` 是基于 `CFRunLoopRef` 的封装，提供了面向对象的 API，但是这些 API 不是线程安全的。

```
NSLog(@"%p %p", [NSRunLoop currentRunLoop], [NSRunLoop mainRunLoop]);
NSLog(@"%p %p", CFRunLoopGetCurrent(), CFRunLoopGetMain());

// 打印结果
2019-07-10 11:50:34.482015 RunLoopDemo[733:70653] 0x1740bb3c0 0x1740bb3c0
2019-07-10 11:50:34.482115 RunLoopDemo[733:70653] 0x17417b900 0x17417b900
```

从打印结果上看`NSRunLoop `和`CFRunLoopRef `获取的`RunLoop`不是一个。

```
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

iOS 开发中能遇到两个线程对象: pthread_t 和 NSThread，它俩是一对一的关系。你可以通过`pthread_main_thread_np()`或`[NSThread mainThread]`来获取主线程；也可以通过`pthread_self() `或`[NSThread currentThread]`来获取当前线程。CFRunLoop 是基于 pthread 来管理的。

在`CF-1153.18`的`CFRunLoop.h`中可以看到并没有暴露创建`RunLoop` 的结构。只提供了两个获取`RunLoop` 的函数：`CFRunLoopGetCurrent()`和`CFRunLoopGetMain()`。它们俩的内部实现逻辑大致如下：

```
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

![](https://images.gitee.com/uploads/images/2019/0710/172810_bbb6752a_1355277.png "RunLoop_image0101.png")

一个`RunLoop` 包含若干个`Mode `，每个`Mode ` 又包含若干个`Source/Timer/Observer`。每次调用`RunLoop` 的主函数时，只能指定其中一个`Mode `，这个`Mode `被称作 `CurrentMode`。如果需要切换`Mode`，只能退出`Loop `，再重新指定一个`Mode ` 进入。这样做主要是为了分隔开不同组的`Source/Timer/Observer`，让其互不影响。

### 0x01 CFRunLoopSourceRef

`CFRunLoopSourceRef` 是事件产生的地方。`Source`有两个版本：`Source0` 和 `Source1`。

- `Source0` 只包含了一个回调（函数指针），它并不能主动触发事件。使用时，你需要先调用`CFRunLoopSourceSignal(source)`，将这个`Source` 标记为待处理，然后手动调用`CFRunLoopWakeUp(runloop)` 来唤醒`RunLoop`，让其处理这个事件。

- `Source1` 包含了一个`mach_port`和一个回调（函数指针），被用于通过内核和其他线程相互发送消息。这种`Source `能主动唤醒`RunLoop `的线程，其原理在下面会讲到。


### 0x02 CFRunLoopTimerRef

`CFRunLoopTimerRef`是基于时间的触发器。其包含一个时间长度和一个回调（函数指针）。当其加入到`RunLoop `时，`RunLoop `会注册对应的时间点，当时间点到时，`RunLoop`会被唤醒以执行那个回调。

### 0x03 CFRunLoopModeRef

`CFRunLoopMode`和`CFRunLoop`的底层结构大致如下

```
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

```
CF_EXPORT CFStringRef CFRunLoopCopyCurrentMode(CFRunLoopRef rl);

CF_EXPORT CFArrayRef CFRunLoopCopyAllModes(CFRunLoopRef rl);
```

- `CommonModes `：一个`Mode `可以将自己标记为`Common `属性（通过将其`ModeName`添加到`RunLoop`的`commonModes`中）。每当`RunLoop `的内容发生变化时，`RunLoop `都会自动将 `_commonModeItems` 里的`Source/Observer/Timer`同步到具有`Common `标记的所有`Mode`里。

```
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

```
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



## 四、



<br>

写于2019-07-09

<br>