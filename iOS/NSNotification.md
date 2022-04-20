# NSNotification


<br>

## 一、NSNotification

`NSNotification` 包含名称、对象、可选字典。

- 名称：`NSNotification` 的名称

- 对象：通知的发布者想要发送给该通知的观察者的任何对象（通常是发布通知的对象）
- 可选字典：可能包含有关通知的附加信息

定义如下：

```Objective-C
@interface NSNotification : NSObject <NSCopying, NSCoding>

@property (readonly, copy) NSNotificationName name;
@property (nullable, readonly, retain) id object;
@property (nullable, readonly, copy) NSDictionary *userInfo;

- (instancetype)initWithName:(NSNotificationName)name object:(nullable id)object userInfo:(nullable NSDictionary *)userInfo API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end


@interface NSNotification (NSNotificationCreation)

+ (instancetype)notificationWithName:(NSNotificationName)aName object:(nullable id)anObject;
+ (instancetype)notificationWithName:(NSNotificationName)aName object:(nullable id)anObject userInfo:(nullable NSDictionary *)aUserInfo;

- (instancetype)init /*API_UNAVAILABLE(macos, ios, watchos, tvos)*/;	/* do not invoke; not a valid initializer for this class */

@end
```

> You don’t usually create your own notifications directly, but instead call the `NSNotificationCenter` methods `postNotificationName:object:` and `postNotificationName:object:userInfo:`.

官方文档有句话表明，我们经常使用 `NSNotificationCenter` 的 `postNotificationName:object:` 和 `postNotificationName:object:userInfo:` 来间接创建 `NSNotification` 对象，而不是调用它的初始化方法。

<br>

## 二、NSNotificationCenter

<br>



<br>

**Reference**

- [NSNotification - Apple](https://developer.apple.com/documentation/foundation/nsnotification?language=objc)

- [NSNotificationCenter - GNUStep](https://github.com/gnustep/libs-base/blob/master/Source/NSNotificationCenter.m)

<br>