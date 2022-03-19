
# Singleton Pattern (单例模式)


<br>

## 一、单例模式

### 0x01 简介

在我们编写代码过程中，会遇到一些全局只需要一个实例的类。如线程池、播放器、聊天框、文件管理类等。事实上这些类对象只能有一个，若创建多个实例，就可能会导致程序行为异常、资源抢夺等问题。这时就需要用到我们常说的单例模式。

**单例模式：确保一个类只有一个实例，并提供一个全局访问点。**

若我们使用静态全局对象实例或类方法来提供全局访问点，这样并不能保证全局对象只被初始化一次。

若使用静态全局变量保持对类的实例的唯一引用，虽然既能保证提供全局访问点，也能保证访问这个全局变量对象只被初始化一次，但不能保证你的小伙伴不会定义一个同样类型的全局变量。


<br>

### 0x02 OC 中单例的使用

iOS 中也有很多系统给我们定义好的单例类，如 `NSFileManager`、 `UIApplication`。我们可以使用系统提供的 `Dispatch Once` 代码片段来快速创建单例，且不用考虑线程安全的问题。

如下创建一个单例类 `SingletonPattern`：

```Objective-C
@implementation SingletonPattern

static SingletonPattern *_instance = nil;
static dispatch_once_t onceToken;

+ (instancetype)shareInstance
{
    dispatch_once(&onceToken, ^{
        if (_instance == nil) {
            _instance = [[super allocWithZone:NULL] init];
        }
    });
    return _instance;
}

+ (instancetype)allocWithZone:(struct _NSZone *)zone
{
    return [self shareInstance];
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)mutableCopyWithZone:(NSZone *)zone
{
    return self;
}

+ (void)destoryInstance
{
    onceToken = 0;
    _instance = nil;
}

- (void)dealloc
{
    NSLog(@"%s", __func__);
}

@end
```

示例：

```Objective-C
NSLog(@"%@", [SingletonPattern shareInstance]);
NSLog(@"%@", [[SingletonPattern alloc] init]);
NSLog(@"%@", [SingletonPattern new]);
NSLog(@"%@", [SingletonPattern new].copy);

[SingletonPattern destoryInstance];

NSLog(@"%@", [SingletonPattern shareInstance]);

// <SingletonPattern: 0x600000604330>
// <SingletonPattern: 0x600000604330>
// <SingletonPattern: 0x600000604330>
// <SingletonPattern: 0x600000604330>
// -[SingletonPattern dealloc]
//  <SingletonPattern: 0x600000600490>
```

<br>


### 0x03 Swift 中单例的使用

Swift 中单例的创建更简单，若不需要销毁单例，代码如下：

```Swift
class Singletion {
    static let shared = Singletion()
    
    private init() { }
}
```

这时因为 Swift 中 `static` 修饰的变量内部会调用 `dispatch_once_f` 方法。具体看参考[类型属性](https://github.com/zhaoName/Notes/blob/master/Swift/Property.md#%E5%9B%9B%E7%B1%BB%E5%9E%8B%E5%B1%9E%E6%80%A7)

示例

```Swift
print(Mems.ptr(ofRef: Singletion.shared))
print(Mems.ptr(ofRef: Singletion.shared))
print(Mems.ptr(ofRef: Singletion.shared))

// 0x00000001018652b0
// 0x00000001018652b0
// 0x00000001018652b0
```

若单例最后需要销毁，则创建方式略显复杂

```Swift
class Singletion {
    private static var _shared: Singletion?
    
    private init() { }
    
    class func shared() -> Singletion {
        guard let sing = _shared else {
            _shared = Singletion()
            return _shared!
        }
        return sing
    }
    
    class func destory() {
        _shared = nil
    }
    
    deinit {
        print(#function)
    }
}
```

示例：

```swift
print(Mems.ptr(ofRef: Singletion.shared()))
print(Mems.ptr(ofRef: Singletion.shared()))

Singletion.destory()
print(Mems.ptr(ofRef: Singletion.shared()))

// 0x0000000100689a40
// 0x0000000100689a40
// deinit
// 0x0000000100693cf0
```

<br>

## 二、单例的滥用

### 0x01 全局状态

单例就是披着羊皮的全局状态。一个单例可以被使用在任何地方，而不需要显式地声明依赖。如下：

```Objective-C
@interface SPSingleton : NSObject

+ (instancetype)sharedInstance;

- (NSUInteger)badMutableState;
- (void)setBadMutableState:(NSUInteger)badMutableState;

@end

@implementation SPConsumerA

- (void)someMethod
{
    if ([[SPSingleton sharedInstance] badMutableState]) {
        // ...
    }
}

@end

@implementation SPConsumerB

- (void)someOtherMethod
{
    [[SPSingleton sharedInstance] setBadMutableState:0];
}

@end
```
`SPConsumerA` 和 `SPConsumerB` 是两个完全独立的模块。但是 `SPConsumerB` 可以通过使用单例提供的共享状态来影响 `SPConsumerA` 的行为。这种情况应该只能发生在 `consumer` B 显式引用了 `A`，并表明了两者之间的关系时。这里使用了单例，由于其具有全局和多状态的特性，导致隐式地在两个看起来完全不相关的模块之间建立了耦合。

<br>

### 0x02 对象的生命周期

假设我们正在构建一个应用，在这个应用里用户可以看到他们的好友列表。他们的每个朋友都有一张个人信息的图片，并且我们想使我们的应用能够下载并且在设备上缓存这些图片。我们可以写一个 `SPThumbnailCache` 单例

```Objective-C
@interface SPThumbnailCache : NSObject

+ (instancetype)sharedThumbnailCache;

- (void)cacheProfileImage:(NSData *)imageData forUserId:(NSString *)userId;
- (NSData *)cachedProfileImageForUserId:(NSString *)userId;

@end
```

若后来我们决定去实现‘注销’功能，这样用户可以在应用中进行账号切换。突然我们发现我们将要面临一个讨厌的问题：用户相关的状态存储在全局单例中。当用户注销后，我们希望能够清理掉所有的硬盘上的持久化状态。否则，我们将会把这些被遗弃的数据残留在用户的设备上，浪费宝贵的硬盘空间。

```Objective-C
+ (void)tearDown
{
    // The SPThumbnailCache will clean up persistent states when deallocated
    sharedThumbnailCache = nil;
}
```
 虽然销毁单例能够解决问题，但代价太大，且执行销毁的时机非常重要。假设当用户正在执行登出操作时，有一些后台任务正在执行把图片保存到缓存中的操作:

```Objective-C
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    [[SPThumbnailCache sharedThumbnailCache] cacheProfileImage:newImage forUserId:userId];
});
```
我们需要保证在所有的后台任务完成前， `tearDown` 一定不能被执行。否则一个新的缩略图 `cache` 的实例将会被延迟创建，并且之前用户的数据 (`newImage` 对象) 会被存储在它里面。




这里我们得到的教训是，单例应该只用来保存全局的状态，并且不能和任何作用域绑定。如果这些状态的作用域比一个完整的应用程序的生命周期要短，那么这个状态就不应该使用单例来管理。用一个单例来管理用户绑定的状态，是代码的坏味道，你应该认真的重新评估你的对象图的设计。
<br>

## 三、避免单例的滥用

既然我们的缩略图 `cache` 的缓存状态是和具体的用户绑定的，那么让我们来定义一个`user`对象吧：

```Objective-C
@interface SPUser : NSObject

@property (nonatomic, readonly) SPThumbnailCache *thumbnailCache;

@end

@implementation SPUser

- (instancetype)init
{
    if ((self = [super init])) {
        _thumbnailCache = [[SPThumbnailCache alloc] init];

        // Initialize other user-specific state...
    }
    return self;
}

@end
```

我们现在用一个对象来作为一个经过认证的用户会话的模型类，并且我们可以把所有和用户相关的状态存储在这个对象中。现在假设我们有一个`view controller`来展现好友列表：

```Objective-C
@interface SPFriendListViewController : UIViewController

- (instancetype)initWithUser:(SPUser *)user;

@end
```
我们可以显式地把经过认证的 `user` 对象作为参数传递给这个 `view controller`。这种把依赖性传递给依赖对象的技术正式的叫法是依赖注入。

但类之间的依赖过多，也会有问题。如代码复用起来很难。代码抽离封装会拖泥带水，必须带着全部依赖的类。

<br>

参考：

- [避免滥用单例](https://objccn.io/issue-13-2/)

- [单例的正确写法](https://zhuanlan.zhihu.com/p/37660939)

<br>