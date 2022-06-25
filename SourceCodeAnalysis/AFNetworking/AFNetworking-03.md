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


```Objective-C

```

```Objective-C

```


```Objective-C

```

```Objective-C

```

```Objective-C

```

```Objective-C

```

<br>

```Objective-C
```

```Objective-C
```

```Objective-C
```

```Objective-C
```

```Objective-C
```

```Objective-C
```
<br>



<br>


<br>