
# App优化-卡顿监测


如果 RunLoop 的线程，进入睡眠前方法的执行时间过长而导致无法进入睡眠，或者线程唤醒后接收消息时间过长而无法进入下一步的话，就可以认为是线程受阻了。如果这个线程是主线程的话，表现出来的就是出现了卡顿。

```Objective-C
#import "ZZMonitor.h"

@implementation ZZMonitor
{
    int runloopCount;
    CFRunLoopObserverRef _observer;
    dispatch_semaphore_t _semaphore;
    CFRunLoopActivity _activity;
}

static ZZMonitor *_mon = nil;

+ (ZZMonitor *)defaultMonitor
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (_mon == nil) {
            _mon = [[ZZMonitor alloc] init];
        }
    });
    return _mon;
}

/*
 typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
     kCFRunLoopEntry         = (1UL << 0), // 即将进入 Loop
     kCFRunLoopBeforeTimers  = (1UL << 1), // 即将处理 Timer
     kCFRunLoopBeforeSources = (1UL << 2), // 即将处理 Source
     kCFRunLoopBeforeWaiting = (1UL << 5), // 即将进入休眠
     kCFRunLoopAfterWaiting  = (1UL << 6), // 刚从休眠中唤醒
     kCFRunLoopExit          = (1UL << 7), // 即将退出 Loop
     kCFRunLoopAllActivities = 0x0FFFFFFFU // 所有 Activity
 };
 */
- (void)startMonitor
{
    _semaphore = dispatch_semaphore_create(0);
    
    // (__bridge void*)self 会传到 runloopObserverCallBack 的 info 参数中
    CFRunLoopObserverContext context = {0, (__bridge void*)self, NULL, NULL};
    _observer = CFRunLoopObserverCreate(CFAllocatorGetDefault(), kCFRunLoopAllActivities, YES, 0, &runloopObserverCallBack, &context);
    CFRunLoopAddObserver(CFRunLoopGetMain(), _observer, kCFRunLoopCommonModes);
    
    
    // 子线程监控时长
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        while (YES)
        {
            //NSLog(@"while start==%ld", _activity);
            long st = dispatch_semaphore_wait(self->_semaphore, dispatch_time(DISPATCH_TIME_NOW, 20*NSEC_PER_MSEC));
            //NSLog(@"dispatch_semaphore_wait==%ld", _activity);
            if (st != 0) {
                if (self->_activity == kCFRunLoopBeforeSources || self->_activity == kCFRunLoopAfterWaiting) {
                    if (++self->runloopCount < 5) continue;
                    NSLog(@"有点卡。。。");
                }
            }
            self->runloopCount = 0;
        }
    });
}


- (void)stopMonitor
{
    if (!_observer) return;
    CFRunLoopRemoveObserver(CFRunLoopGetMain(), _observer, kCFRunLoopCommonModes);
    CFRelease(_observer);
}

#pragma mark -- private method

void runloopObserverCallBack(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void *info)
{
    //NSLog(@"%@---%ld---%@", observer, activity, info);
    ZZMonitor *monitor = (__bridge ZZMonitor *)info;
    
    monitor->_activity = activity;
    dispatch_semaphore_signal(monitor->_semaphore);
}

```


**参考:**

- [iOS实时卡顿监控](http://www.tanhao.me/code/151113.html/)

- [深入剖析 iOS 性能优化](http://www.starming.com/2017/06/20/deeply-ios-performance-optimization/)

- [RunLoop](https://github.com/zhaoName/Notes/blob/master/iOS/RunLoop.md)