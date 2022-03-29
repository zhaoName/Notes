# 给分类添加 weak 属性


<br>

我们可以用关联对象给分类添加的属性实现 `setter` 和 `getter` 方法。但关联对象的策略没有关于 `weak` 的效果。`objc_AssociationPolicy` 定义如下：

```Objective-C
typedef OBJC_ENUM(uintptr_t, objc_AssociationPolicy) {
    OBJC_ASSOCIATION_ASSIGN = 0,           /**< Specifies a weak reference to the associated object. */
    OBJC_ASSOCIATION_RETAIN_NONATOMIC = 1, /**< Specifies a strong reference to the associated object. 
                                            *   The association is not made atomically. */
    OBJC_ASSOCIATION_COPY_NONATOMIC = 3,   /**< Specifies that the associated object is copied. 
                                            *   The association is not made atomically. */
    OBJC_ASSOCIATION_RETAIN = 01401,       /**< Specifies a strong reference to the associated object.
                                            *   The association is made atomically. */
    OBJC_ASSOCIATION_COPY = 01403          /**< Specifies that the associated object is copied.
                                            *   The association is made atomically. */
};
```

若我们用 `OBJC_ASSOCIATION_ASSIGN` 去实现 `weak` 效果，会因为野指针访问而崩溃。如下定义分类 `ZZPerosn (weak)`

```Objective-C
@interface ZZPerosn (weak)
@property (nonatomic, strong) id weakObj;
@end


@implementation ZZPerosn (weak)

- (void)setWeakObj:(id)weakObj
{
    objc_setAssociatedObject(self, @selector(weakObj), weakObj, OBJC_ASSOCIATION_ASSIGN);
}

- (id)weakObj
{
    return objc_getAssociatedObject(self, @selector(weakObj));
}

- (void)dealloc
{
    NSLog(@"%s----%@", __func__, self.weakObj);
}
@end
```

然后执行如下代码就会导致崩溃

```Objective-C
NSObject* obj = [NSObject new];

ZZPerosn *per = [ZZPerosn new];
per.weakObj = obj;
NSLog(@"%p---%p", per.weakObj, obj);  // 0x6000008904d0---0x6000008904d0
obj = nil;

// ❌ Thread 1: EXC_BAD_ACCESS (code=1, address=0x423f0fc704f0)
NSLog(@"%p---%p", per.weakObj, obj);
```

那如何给分类添加 `weak` 效果的属性，下面介绍两种方案。

### 0x01 借助 block

使用 `OBJC_ASSOCIATION_COPY` 关联策略将 block copy 到堆上，这样对象就强引用 block。获取 block 持有的 `weak` 对象返回属性值。

若 block 被释放，则 block 持有的 `weak` 对象也会被释放，且 `__weak` 本身就会把指针指向 `nil`。

```Objective-C
- (void)setWeakObj:(id)weakObj
{
    __weak id weak = weakObj;
    id(^block)(void) = ^{ return weak; };
    objc_setAssociatedObject(self, @selector(weakObj), block, OBJC_ASSOCIATION_COPY);
}

- (id)weakObj
{
    id(^block)(void) = objc_getAssociatedObject(self, @selector(weakObj));
    return block ? block() : nil;
}
```

再执行崩溃代码，打印如下

```Objective-C
// 0x600002ef0540---0x600002ef0540
// 0x0---0x0
// -[ZZPerosn(weak) dealloc]----(null)
```

<br>

### 0x02 使用容器

实际上使用支持弱引用的容器如`NSHashTable`、`NSMapTable`、`NSPointerArray`都是可以实现的。原理很简单，使用容器持有关联的对象，当该对象不存在时，容器自身便有自动移除已销毁对象的特性，这样就实现了`weak` 属性。

- `NSMapTable` 可以持有键和值的弱引用，当键或者值当中的一个被释放时，整个这一项就会被移除掉。

- `NSHashTable` 可以持有成员的弱引用。
- `NSPointerArray` 可以持有成员的弱引用，当成员不存在时自动把所在 `index` 置为`NULL`。

```Objective-C
- (void)setWeakObj:(id)weakObj
{
    if (!weakObj) return;
    
    if (!pointerArray) {
        pointerArray = [NSPointerArray weakObjectsPointerArray];
    }
    
    [pointerArray addPointer:(__bridge void * _Nullable)(weakObj)];
    objc_setAssociatedObject(self, @selector(weakObj), pointerArray, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (id)weakObj
{
    if (!pointerArray) return nil;
    
    [pointerArray compact];
    for (id pointer in pointerArray) {
        if (!pointer) continue;
        return objc_getAssociatedObject(self, @selector(weakObj));
    }
    
    pointerArray = nil;
    return nil;
}
```

<br>


<br>

**Reference**

- [Weak Associated Object](https://blog.chenyalun.com/2019/01/20/Weak%20Associated%20Object/)

- [如何使用 Runtime 给现有的类添加 weak 属性](https://www.jianshu.com/p/ed65d71554d8)
<br>