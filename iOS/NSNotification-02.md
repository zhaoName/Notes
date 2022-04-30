# NSNotification


<br>







<br>


```Objective-C

```

```Objective-C

```


```Objective-C

```

<br>

## 二、`NSNotificationQueue `

对于 NSNotificationQueue 总结如下

依赖runloop，所以如果在其他子线程使用NSNotificationQueue，需要开启runloop
最终还是通过NSNotificationCenter进行发送通知，所以这个角度讲它还是同步的
所谓异步，指的是非实时发送而是在合适的时机发送，并没有开启异步线程

### 0x01 `NSNotificationCoalescing`

`NSNotificationCoalescing` 的定义如下：

```Objective-C
typedef NS_OPTIONS(NSUInteger, NSNotificationCoalescing) {
    NSNotificationNoCoalescing = 0,
    NSNotificationCoalescingOnName = 1,
    NSNotificationCoalescingOnSender = 2
};
```

- `NSNotificationNoCoalescing`：不合并通知

- `NSNotificationCoalescingOnName`：合并相同 `name` 的通知
- `NSNotificationCoalescingOnSender`：合并相同 `object` 的通知


### 0x02 `NSPostingStyle`

`NSPostingStyle` 的定义如下：

```Objective-C
typedef NS_ENUM(NSUInteger, NSPostingStyle) {
    NSPostWhenIdle = 1,
    NSPostASAP = 2,
    NSPostNow = 3
};
```

- `NSPostWhenIdle`：空闲时发送通知。 当 runloop 处于等待或空闲状态时，发送通知。

- `NSPostASAP`：尽快发送通知。当当前 runloop 所执行代码完成时，将会发送通知。
> Any notification enqueued with the NSPostASAP posting style is posted to the notification center when the code executing in the current run loop callout completes. Callouts can be Application Kit event messages, file descriptor changes, timers, or another asynchronous notification.
- `NSPostNow`：合并后立即发通知。它和 `NSNotificationCenter` 的效果一样，使用的它的原因是它可用于在发布通知之前合并先前排队的通知。

```Objective-C
- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = UIColor.redColor;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(actionAsyncNotifition:) name:Test_Notification_Name_Queue object:nil];
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [self notifiWithRunloop];
    });
}

- (void)notifiWithRunloop 
{
    CFRunLoopObserverRef observer = CFRunLoopObserverCreateWithHandler(CFAllocatorGetDefault(), kCFRunLoopAllActivities, YES, 0, ^(CFRunLoopObserverRef observer, CFRunLoopActivity activity) {
        if(activity == kCFRunLoopEntry){
            NSLog(@"进入Runloop");
        }else if(activity == kCFRunLoopBeforeWaiting){
            NSLog(@"即将进入等待状态");
        }else if(activity == kCFRunLoopAfterWaiting){
            NSLog(@"结束等待状态");
        }
    });
    CFRunLoopAddObserver(CFRunLoopGetCurrent(), observer, kCFRunLoopDefaultMode);
    CFRelease(observer);
    
    // NSPostWhenIdle
    NSNotification *notification1 = [NSNotification notificationWithName:Test_Notification_Name_Queue object:nil userInfo:@{@"key":@"value1"}];
    [[NSNotificationQueue defaultQueue] enqueueNotification:notification1 postingStyle:NSPostWhenIdle coalesceMask:NSNotificationNoCoalescing forModes:@[NSDefaultRunLoopMode]];
    
    // NSPostASAP
    NSNotification *notification2 = [NSNotification notificationWithName:Test_Notification_Name_Queue object:nil userInfo:@{@"key":@"value2"}];
    [[NSNotificationQueue defaultQueue] enqueueNotification:notification2 postingStyle:NSPostASAP coalesceMask:NSNotificationNoCoalescing forModes:@[NSDefaultRunLoopMode]];
    
    // NSPostNow
    NSNotification *notification3 = [NSNotification notificationWithName:Test_Notification_Name_Queue object:nil userInfo:@{@"key":@"value3"}];
    [[NSNotificationQueue defaultQueue] enqueueNotification:notification3 postingStyle:NSPostNow coalesceMask:NSNotificationNoCoalescing forModes:@[NSDefaultRunLoopMode]];
    
    [[NSRunLoop currentRunLoop] addPort:[NSPort new] forMode:NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] run];
}
```

```Objective-C
-[ViewController actionAsyncNotifition:] --- ZZ_Test_Notification_Name_Queue --- <NSThread: 0x600001a0a5c0>{number = 5, name = (null)}
(null)--- {
    key = value3;
}

进入Runloop
-[ViewController actionAsyncNotifition:] --- ZZ_Test_Notification_Name_Queue --- <NSThread: 0x600001a0a5c0>{number = 5, name = (null)}
(null)--- {
    key = value2;
}

即将进入等待状态
-[ViewController actionAsyncNotifition:] --- ZZ_Test_Notification_Name_Queue --- <NSThread: 0x600001a0a5c0>{number = 5, name = (null)}
(null)--- {
    key = value1;
}

结束等待状态
即将进入等待状态
```

<br>

### 0x03 在指定线程发送通知

通知在哪个线程发布的，就会在哪个线程响应方法。和 `addObserver:` 所在的线程没有关系。

```Objective-C
- (void)viewDidLoad {
    [super viewDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(actionTestNotifition:) name:Test_Notification_Name_Queue object:nil];
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:Test_Notification_Name_Queue object:nil];
    });
}
```

```Objective-C
<NSThread: 0x600002c7b440>{number = 5, name = (null)}

-[ViewController actionTestNotifition:] --- ZZ_Test_Notification_Name_Queue --- <NSThread: 0x60000278c900>{number = 4, name = (null)}
<ViewController: 0x7f8048807bd0>--- (null)
```

那如何在指定线程接收通知，下面是官网的给出例子

```Objective-C
// ZZParticularThreadNOtification.h

@interface ZZParticularThreadNOtification : NSObject

- (void)setUpThreadingSupport;
- (void)processNotification:(NSNotification *)notification;

@end
```

```Objective-C
// ZZParticularThreadNOtification.m

@interface ZZParticularThreadNOtification ()<NSMachPortDelegate>

@property NSMutableArray *notifications;
@property NSThread *notificationThread;
@property NSLock *notificationLock;
@property NSMachPort *notificationPort;

@end

@implementation ZZParticularThreadNOtification

- (void)setUpThreadingSupport
{
    if (self.notifications) { return; }
    
    self.notifications = [[NSMutableArray alloc] init];
    self.notificationLock  = [[NSLock alloc] init];
    self.notificationThread = [NSThread currentThread];
 
    self.notificationPort = [[NSMachPort alloc] init];
    [self.notificationPort setDelegate:self];
    [[NSRunLoop currentRunLoop] addPort:self.notificationPort forMode:NSRunLoopCommonModes];
}

- (void)processNotification:(NSNotification *)notification
{
    NSLog(@"%s --- %@", __func__, [NSThread currentThread]);
    
    if ([NSThread currentThread] != self.notificationThread) {
        // Forward the notification to the correct thread.
        [self.notificationLock lock];
        [self.notifications addObject:notification];
        [self.notificationLock unlock];
        [self.notificationPort sendBeforeDate:[NSDate date] components:nil from:nil reserved:0];
    }
    else {
        // Process the notification here;
        NSLog(@"current thread = %@", [NSThread currentThread]);
        NSLog(@"process notification");
    }
}

// NSMachPortDelegate
- (void)handleMachMessage:(void *)msg
{
    [self.notificationLock lock];
 
    while ([self.notifications count]) {
        NSNotification *notification = [self.notifications objectAtIndex:0];
        [self.notifications removeObjectAtIndex:0];
        [self.notificationLock unlock];
        [self processNotification:notification];
        [self.notificationLock lock];
    };
 
    [self.notificationLock unlock];
}
@end
```

在 `ViewController` 中使用

```Objective-C
- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.particular = [ZZParticularThreadNOtification new];
    [self.particular setUpThreadingSupport];
    [[NSNotificationCenter defaultCenter] addObserver:self.particular selector:@selector(actionAsyncNotifition:) name:Test_Notification_Name_Queue object:nil];
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:Test_Notification_Name_Queue object:self];
    });
}
```
打印结果如下：

```Objective-C
-[ZZParticularThreadNOtification processNotification:] --- <NSThread: 0x600002808140>{number = 1, name = main}
process notification
```

<br>

**Reference**

- [Delivering Notifications To Particular Threads](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Notifications/Articles/Threading.html#//apple_ref/doc/uid/20001289-CEGJFDFG)


<br>