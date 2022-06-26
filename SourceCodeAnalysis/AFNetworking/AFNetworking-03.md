# AFNetworking - AFNetworkReachabilityManager

本文主要介绍 AFNetworkReachabilityManager 的实现，所用版本为 AFNetworking 4.0.1。

<br>

## 一、如何判断网络是否可用

`SCNetworkReachabilityFlags ` 表明指定的网络节点或地址是否可以访问，是否需要连接以及在建立连接时是否需要用户手动操作(如输入用户名、密码)。

```Objective-C
typedef CF_OPTIONS(uint32_t, SCNetworkReachabilityFlags) {
	kSCNetworkReachabilityFlagsTransientConnection		= 1<<0,
	kSCNetworkReachabilityFlagsReachable				= 1<<1,
	kSCNetworkReachabilityFlagsConnectionRequired		= 1<<2,
	kSCNetworkReachabilityFlagsConnectionOnTraffic		= 1<<3,
	kSCNetworkReachabilityFlagsInterventionRequired		= 1<<4,
	kSCNetworkReachabilityFlagsConnectionOnDemand		= 1<<5,
	kSCNetworkReachabilityFlagsIsLocalAddress			= 1<<16,
	kSCNetworkReachabilityFlagsIsDirect					= 1<<17,
	kSCNetworkReachabilityFlagsIsWWAN					= 1<<18,

	kSCNetworkReachabilityFlagsConnectionAutomatic	= kSCNetworkReachabilityFlagsConnectionOnTraffic
};
```

- `kSCNetworkReachabilityFlagsTransientConnection`：表明此地址或节点名可通过连接访问 (如 PPP)

- `kSCNetworkReachabilityFlagsReachable`: 表明可在当前网络配置下访问该节点或地址
- `kSCNetworkReachabilityFlagsConnectionRequired `：表明可在当前网络配置下访问该节点或地址，但首先需要建立连接。如拨号连接时将返回此状态。
- `kSCNetworkReachabilityFlagsConnectionOnTraffic`：表明可在当前网络配置下访问该节点或地址，但首先需要建立连接。定向到指定名称或地址的任何流量都将启动连接。
- `kSCNetworkReachabilityFlagsInterventionRequired`： 表明可在当前网络配置下访问该节点或地址，但首先需要建立连接。如需要用户提供密码、token 等。
- `kSCNetworkReachabilityFlagsConnectionOnDemand`：表明可在当前网络配置下访问该节点或地址，但首先需要建立连接。`CFSocketStream` 将“按需”建立连接。其他 API 不会建立连接。
- `kSCNetworkReachabilityFlagsIsLocalAddress`：表明指定的节点名或地址是与当前系统上的网络接口相关联的。
- `kSCNetworkReachabilityFlagsIsDirect`：表示到指定节点名或地址的网络流量不会通过网关，而是直接路由到系统中的一个接口。
- `kSCNetworkReachabilityFlagsIsWWAN`： 表明可以连接到蜂窝网络，如`EDGE`、`GPRS` `Cell` 等。


那我们怎么根据这些 flag 认为指定的地址或节点是否可用呢？AFNetworking 中做出如下判断：

```Objective-C
static AFNetworkReachabilityStatus AFNetworkReachabilityStatusForFlags(SCNetworkReachabilityFlags flags) {
    BOOL isReachable = ((flags & kSCNetworkReachabilityFlagsReachable) != 0);
    BOOL needsConnection = ((flags & kSCNetworkReachabilityFlagsConnectionRequired) != 0);
    BOOL canConnectionAutomatically = (((flags & kSCNetworkReachabilityFlagsConnectionOnDemand ) != 0) || ((flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0));
    BOOL canConnectWithoutUserInteraction = (canConnectionAutomatically && (flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0);
    // 当前网络可访问 且不需要手动干预 就认为此地址或节点可访问
    BOOL isNetworkReachable = (isReachable && (!needsConnection || canConnectWithoutUserInteraction));

    AFNetworkReachabilityStatus status = AFNetworkReachabilityStatusUnknown;
    if (isNetworkReachable == NO) {
        status = AFNetworkReachabilityStatusNotReachable;
    }
#if	TARGET_OS_IPHONE
    else if ((flags & kSCNetworkReachabilityFlagsIsWWAN) != 0) {
        status = AFNetworkReachabilityStatusReachableViaWWAN;
    }
#endif
    else {
        status = AFNetworkReachabilityStatusReachableViaWiFi;
    }

    return status;
}
```

<br>

## 二、AFNetworkReachabilityManager

### 0x01 初始化

`AFNetworkReachabilityManager ` 提供了 5 个初始化方法。4 个类方法和 1 个实例方法，4 个类方法最后都会调用实例方法完成初始化。

- 单例初始化

```Objective-C
+ (instancetype)sharedManager {
    static AFNetworkReachabilityManager *_sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _sharedManager = [self manager];
    });

    return _sharedManager;
}
```

- 通过地址或域名初始化

```Objective-C
+ (instancetype)managerForDomain:(NSString *)domain {
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(kCFAllocatorDefault, [domain UTF8String]);

    AFNetworkReachabilityManager *manager = [[self alloc] initWithReachability:reachability];
    
    CFRelease(reachability);

    return manager;
}

+ (instancetype)managerForAddress:(const void *)address {
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, (const struct sockaddr *)address);
    AFNetworkReachabilityManager *manager = [[self alloc] initWithReachability:reachability];

    CFRelease(reachability);
    
    return manager;
}

+ (instancetype)manager
{
#if (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 90000) || (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101100)
    struct sockaddr_in6 address;
    bzero(&address, sizeof(address));
    address.sin6_len = sizeof(address);
    address.sin6_family = AF_INET6;
#else
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_len = sizeof(address);
    address.sin_family = AF_INET;
#endif
    return [self managerForAddress:&address];
}
```

- 实例方法初始化

```Objective-C
- (instancetype)initWithReachability:(SCNetworkReachabilityRef)reachability {
    self = [super init];
    if (!self) {
        return nil;
    }

    _networkReachability = CFRetain(reachability);
    self.networkReachabilityStatus = AFNetworkReachabilityStatusUnknown;

    return self;
}
```

初始化的目的有两个

-  设置 `SCNetworkReachabilityRef`

- 设置 `networkReachabilityStatus` 的默认状态为 `AFNetworkReachabilityStatusUnknown`

### 0x02 `startMonitoring`

`startMonitoring`方法中监听网络状态，如下

```Objective-C
- (void)startMonitoring {
    [self stopMonitoring];

    if (!self.networkReachability) { return; }

    __weak __typeof(self)weakSelf = self;
    
    AFNetworkReachabilityStatusCallback callback = ^(AFNetworkReachabilityStatus status) {
        __strong __typeof(weakSelf)strongSelf = weakSelf;
        
        strongSelf.networkReachabilityStatus = status;
        if (strongSelf.networkReachabilityStatusBlock) {
            strongSelf.networkReachabilityStatusBlock(status);
        }
        return strongSelf;
    };

    SCNetworkReachabilityContext context = {0, (__bridge void *)callback, AFNetworkReachabilityRetainCallback, AFNetworkReachabilityReleaseCallback, NULL};
    SCNetworkReachabilitySetCallback(self.networkReachability, AFNetworkReachabilityCallback, &context);
    SCNetworkReachabilityScheduleWithRunLoop(self.networkReachability, CFRunLoopGetMain(), kCFRunLoopCommonModes);

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0),^{
        SCNetworkReachabilityFlags flags;
        if (SCNetworkReachabilityGetFlags(self.networkReachability, &flags)) {
            AFPostReachabilityStatusChange(flags, callback);
        }
    });
}
```

后续网络状态发生变化的回调

```Objective-C
static void AFNetworkReachabilityCallback(SCNetworkReachabilityRef __unused target, SCNetworkReachabilityFlags flags, void *info) {
    NSLog(@"AFNetworkReachabilityCallback--- %d", flags);
    AFPostReachabilityStatusChange(flags, (__bridge AFNetworkReachabilityStatusCallback)info);
}
```

将网络状态发生变化的回调 通过 `AFNetworkReachabilityStatusCallback` 传给外部调用者，并发送通知。

```Objective-C
/**
 * Queue a status change notification for the main thread.
 *
 * This is done to ensure that the notifications are received in the same order
 * as they are sent. If notifications are sent directly, it is possible that
 * a queued notification (for an earlier status condition) is processed after
 * the later update, resulting in the listener being left in the wrong state.
 */
static void AFPostReachabilityStatusChange(SCNetworkReachabilityFlags flags, AFNetworkReachabilityStatusCallback block) {
    AFNetworkReachabilityStatus status = AFNetworkReachabilityStatusForFlags(flags);
    dispatch_async(dispatch_get_main_queue(), ^{
        AFNetworkReachabilityManager *manager = nil;
        if (block) {
            manager = block(status);
        }
        NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
        NSDictionary *userInfo = @{ AFNetworkingReachabilityNotificationStatusItem: @(status) };
        [notificationCenter postNotificationName:AFNetworkingReachabilityDidChangeNotification object:manager userInfo:userInfo];
    });
}
```

### 0x03 KVO 

若外部设置通过 KVO 监听 `networkReachabilityStatus` 属性，则`reachable ` 、`reachableViaWWAN ` 或 `reachableViaWiFi ` 的变化都会触发 KVO。

```Objective-C
+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
    if ([key isEqualToString:@"reachable"] || [key isEqualToString:@"reachableViaWWAN"] || [key isEqualToString:@"reachableViaWiFi"]) {
        return [NSSet setWithObject:@"networkReachabilityStatus"];
    }

    return [super keyPathsForValuesAffectingValueForKey:key];
}
```

<br>

所以 AFNetworking 提供三种监听网络变化的方式： block 、通知、 KVO。

<br>