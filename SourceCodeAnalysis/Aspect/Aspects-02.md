# Aspects - 02

本文介绍 Aspects 如何 hook 方法。

<br>

## 一、hook 前准备

Aspects 整个库里面就只有两个供外部调用的方法，如下。Aspects 是 `NSobject` 的一个`extension`，只要是 `NSObject`，都可以使用这两个方法。这两个方法名字都是同一个，入参和返回值也一样，唯一不同的是一个是加号方法一个是减号方法。一个是用来 hook 类方法，一个是用来 hook 实例方法。

```Objective-C
/**
 Aspects uses Objective-C message forwarding to hook into messages. This will create some overhead. Don't add aspects to methods that are called a lot. Aspects is meant for view/controller code that is not called a 1000 times per second.

 Adding aspects returns an opaque token which can be used to deregister again. All calls are thread safe.
 */
@interface NSObject (Aspects)

/// Adds a block of code before/instead/after the current `selector` for a specific class.
///
/// @param block Aspects replicates the type signature of the method being hooked.
/// The first parameter will be `id<AspectInfo>`, followed by all parameters of the method.
/// These parameters are optional and will be filled to match the block signature.
/// You can even use an empty block, or one that simple gets `id<AspectInfo>`.
///
/// @note Hooking static methods is not supported.
/// @return A token which allows to later deregister the aspect.
+ (id<AspectToken>)aspect_hookSelector:(SEL)selector
                           withOptions:(AspectOptions)options
                            usingBlock:(id)block
                                 error:(NSError **)error;

/// Adds a block of code before/instead/after the current `selector` for a specific instance.
- (id<AspectToken>)aspect_hookSelector:(SEL)selector
                           withOptions:(AspectOptions)options
                            usingBlock:(id)block
                                 error:(NSError **)error;

@end
```

从注释可知：

- Aspects 利用的 OC 的消息转发机制，hook 消息。这样会有一些性能开销。不要把Aspects 加到经常被使用的方法里面。Aspects 是用来设计给 `view/controller` 代码使用的，而不是用来 hook 每秒调用 1000 次的方法的。

- 添加 Aspects 之后，会返回一个隐式的 token，这个 token 会被用来注销 hook 方法的
- 所有的调用都是线程安全的。
- 可以传不带参数的 block ，但若有参数，则第一个参数必为 `id<AspectInfo>`

`NSObject+Aspects.m` 的实现如下：

```Objective-C
@implementation NSObject (Aspects)

+ (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error {
    return aspect_add((id)self, selector, options, block, error);
}

/// @return A token which allows to later deregister the aspect.
- (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error {
    return aspect_add(self, selector, options, block, error);
}
```

这两个方法内部都会调用`aspect_add(self, selector, options, block, error)` 方法，所以我们可以从 `aspect_add` 开始研究，

```Objective-C
static id aspect_add(id self, SEL selector, AspectOptions options, id block, NSError **error) {
    NSCParameterAssert(self);
    NSCParameterAssert(selector);
    NSCParameterAssert(block);

    __block AspectIdentifier *identifier = nil;
    aspect_performLocked(^{
        // 判断当期那传入的 selector 是否允许被 hook
        if (aspect_isSelectorAllowedAndTrack(self, selector, options, error)) {
            // 用runtime的关联对象给 self 增加一个 aspectContainer 属性
            AspectsContainer *aspectContainer = aspect_getContainerForObject(self, selector);
            // 获取 block 的方法签名，并与原方法的方法签名对比
            identifier = [AspectIdentifier identifierWithSelector:selector object:self options:options block:block error:error];
            if (identifier) {
                // identifier 放入对应的数组中
                [aspectContainer addAspect:identifier withOptions:options];

                // Modify the class to allow message interception.
                aspect_prepareClassAndHookSelector(self, selector, error);
            }
        }
    });
    return identifier;
}
```

其函数调用栈如下：

```Objective-C
- aspect_hookSelector:(SEL)selector withOptions:(AspectOptions)options usingBlock:(id)block error:(NSError **)error
└── aspect_add(self, selector, options, block, error);
    └── aspect_performLocked
        ├── aspect_isSelectorAllowedAndTrack
        ├── aspect_getContainerForObject
        └── aspect_prepareClassAndHookSelector
```

<br>

### 0x01 `aspect_performLocked`

`aspect_performLocked` 中使用效率较高的自旋锁 `OSSpinLock` 来保证线程安全。

```Objective-C
static void aspect_performLocked(dispatch_block_t block) {
    static OSSpinLock aspect_lock = OS_SPINLOCK_INIT;
    OSSpinLockLock(&aspect_lock);
    block();
    OSSpinLockUnlock(&aspect_lock);
}
```

但从 iOS10 开始系统就弃用了 `OSSpinLock`，原因在这[不再安全的 OSSpinLock](http://blog.ibireme.com/2016/01/16/spinlock_is_unsafe_in_ios/)


<br>

### 0x02 `aspect_isSelectorAllowedAndTrack`

`aspect_isSelectorAllowedAndTrack` 先定义一个黑名单，里面包含不允许被 hook 的函数名。如下：

```Objective-C
static NSSet *disallowedSelectorList;
static dispatch_once_t pred;
dispatch_once(&pred, ^{
disallowedSelectorList = [NSSet setWithObjects:@"retain", @"release", @"autorelease", @"forwardInvocation:", nil];
});

// Check against the blacklist.
NSString *selectorName = NSStringFromSelector(selector);
if ([disallowedSelectorList containsObject:selectorName]) {
    NSString *errorDescription = [NSString stringWithFormat:@"Selector %@ is blacklisted.", selectorName];
    AspectError(AspectErrorSelectorBlacklisted, errorDescription);
    return NO;
}
```

当检测到 `selector` 的函数名是黑名单里面的函数名，立即报错。

再检查如果要 hook  的是 `dealloc` 方法，则切片时间只能在 `dealloc` 之前，如果不是`AspectPositionBefore`，也要报错。如下：

```Objective-C
// Additional checks.
AspectOptions position = options&AspectPositionFilter;
if ([selectorName isEqualToString:@"dealloc"] && position != AspectPositionBefore) {
    NSString *errorDesc = @"AspectPositionBefore is the only valid position when hooking dealloc.";
    AspectError(AspectErrorSelectorDeallocPosition, errorDesc);
    return NO;
}
```

然后判断该方法是否存在。如果 `self` 和 `self.class` 里面都找不到该 `selector`，会报错找不到该方法。如下：

```Objective-C
if (![self respondsToSelector:selector] && ![self.class instancesRespondToSelector:selector]) {
    NSString *errorDesc = [NSString stringWithFormat:@"Unable to find selector -[%@ %@].", NSStringFromClass(self.class), selectorName];
    AspectError(AspectErrorDoesNotRespondToSelector, errorDesc);
    return NO;
}
```

若不是元类，且到此都没有报错，则此方法允许被 hook。

若是元类，则需要判断元类里面是否允许此方法被 hook。

首先调用 `subclassHasHookedSelectorName` 检查此方法是不是已经被 hook，若是则直接报错。

```Objective-C
AspectTracker *tracker = swizzledClassesDict[currentClass];
// 若当前方法已被 hook 则报错
if ([tracker subclassHasHookedSelectorName:selectorName]) {
    NSSet *subclassTracker = [tracker subclassTrackersHookingSelectorName:selectorName];
    NSSet *subclassNames = [subclassTracker valueForKey:@"trackedClassName"];
    NSString *errorDescription = [NSString stringWithFormat:@"Error: %@ already hooked subclasses: %@. A method can only be hooked once per class hierarchy.", selectorName, subclassNames];
    AspectError(AspectErrorSelectorAlreadyHookedInClassHierarchy, errorDescription);
    return NO;
}
```

在这个 do-while 循环中，`currentClass = class_getSuperclass(currentClass)`这个判断会从`currentClass`的`superclass`开始，一直往上找，直到这个类为根类`NSObject`。若找到 `tracker `，则比较类是否一样。如下：

```Objective-C
do {
    tracker = swizzledClassesDict[currentClass];
    if ([tracker.selectorNames containsObject:selectorName]) {
        if (klass == currentClass) {
            // Already modified and topmost!
            return YES;
        }
        NSString *errorDescription = [NSString stringWithFormat:@"Error: %@ already hooked in %@. A method can only be hooked once per class hierarchy.", selectorName, NSStringFromClass(currentClass)];
        AspectError(AspectErrorSelectorAlreadyHookedInClassHierarchy, errorDescription);
        return NO;
    }
} while ((currentClass = class_getSuperclass(currentClass)));
```

若 `tracker ` 为空或类不一样，则构造 `AspectTracker`，将 `class`、`seletor` 等存储到 `AspectTracker` 中。如下：

```Objective-C
// Add the selector as being modified.
currentClass = klass;
AspectTracker *subclassTracker = nil;
do {
    tracker = swizzledClassesDict[currentClass];
    if (!tracker) {
        tracker = [[AspectTracker alloc] initWithTrackedClass:currentClass];
        swizzledClassesDict[(id<NSCopying>)currentClass] = tracker;
    }
    if (subclassTracker) {
        [tracker addSubclassTracker:subclassTracker hookingSelectorName:selectorName];
    } else {
        [tracker.selectorNames addObject:selectorName];
    }
    
    // All superclasses get marked as having a subclass that is modified.
    subclassTracker = tracker;
}while ((currentClass = class_getSuperclass(currentClass)));
```

通过了 `selector` 是否能被 hook 合法性的检查之后，就要获取或者创建 `AspectsContainer` 容器了。

<br>

### 0x03 `aspect_getContainerForObject`

在读取或者创建 `AspectsContainer` 之前，先对 `selector` 加一个通用前缀 `aspects_`

```Objective-C
static NSString *const AspectsMessagePrefix = @"aspects_";

static SEL aspect_aliasForSelector(SEL selector) {
    NSCParameterAssert(selector);
	return NSSelectorFromString([AspectsMessagePrefix stringByAppendingFormat:@"_%@", NSStringFromSelector(selector)]);
}
```

然后用这个加了前缀的 `aliasSelector` 当关联对象中的 `key`, 来获取 `AspectsContainer` 。若获取不到就创建，并用关联对象存储。

```Objective-C
// Loads or creates the aspect container.
static AspectsContainer *aspect_getContainerForObject(NSObject *self, SEL selector) {
    NSCParameterAssert(self);
    SEL aliasSelector = aspect_aliasForSelector(selector);
    AspectsContainer *aspectContainer = objc_getAssociatedObject(self, aliasSelector);
    if (!aspectContainer) {
        aspectContainer = [AspectsContainer new];
        objc_setAssociatedObject(self, aliasSelector, aspectContainer, OBJC_ASSOCIATION_RETAIN);
    }
    return aspectContainer;
}
```
<br>

### 0x04 创建 `AspectIdentifier `

得到 `aspectContainer` 之后，就可以开始准备我们要 hook 方法的一些信息。这些信息都装在`AspectIdentifier` 中，所以我们需要新建一个 `AspectIdentifier`。

调用 `AspectIdentifier` 的类方法对其初始化，在这个类方法中会获取 `block` 的方法签名，并与原方法的方法签名对比。若失败则返回 `nil`，否则保存 `self`、`selector` 、`options`、`block` 等信息。如下：

```Objective-C
// 获取 block 的方法签名，并与原方法的方法签名对比
identifier = [AspectIdentifier identifierWithSelector:selector object:self options:options block:block error:error];
```

`aspectContainer` 容器会通过 `options` 选项将 `identifier` 分别添加到容器中的`beforeAspects`、`insteadAspects`、`afterAspects` 三个数组。如下

```Objective-C
if (identifier) {
    // identifier 放入对应的数组中
    [aspectContainer addAspect:identifier withOptions:options];
    ...
}
```

### 0x05 总结 `aspect_add`

- 首先调用 `aspect_performLocked` ，利用自旋锁，保证整个操作的线程安全

- 接着调用 `aspect_isSelectorAllowedAndTrack` 对传进来的参数进行强校验，保证参数合法性。
- 接着创建`AspectsContainer`容器，利用`AssociatedObject`关联对象动态添加到NSObject分类中作为属性的。
- 然后在 `AspectIdentifier` 实例验证 `block` 的方法签名。`AspectIdentifier` 主要包含了单个的  `Aspect` 的具体信息，包括执行时机，要执行 `block` 所需要用到的具体信息。
- 再然后 `aspectContainer` 容器会通过 `options` 选项将 `identifier` 分别添加到容器中的`beforeAspects`、`insteadAspects`、`afterAspects` 三个数组。
- 最后调用 `prepareClassAndHookSelector` 准备hook。

<br>


## 二、hook 过程详解



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