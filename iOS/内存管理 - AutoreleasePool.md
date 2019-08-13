
# 内存管理 - AutoreleasePool

<br>


`AutoreleasePool`(自动释放池)是 OC 中的一种内存自动回收机制，它可以延迟加入`AutoreleasePool`中的变量`release`的时机。在正常情况下，创建的变量会在超出其作用域的时候`release`，但是如果将变量加入`AutoreleasePool`，那么`release`将延迟执行。

本篇文章介绍`AutoreleasePool`的实现原理和`autorelease`的释放时机。


## 一、`__AtAutoreleasePool`

用`xcrun -sdk iphoneos clang -arch arm64 -rewrite-objc  ViewController.m`将如下代码转成 C++ 的实现。

```
- (void)viewDidLoad {
    [super viewDidLoad];
    
    @autoreleasepool {
        NSObject *obj = [[NSObject alloc] init];
    }
}
```

C++ 代码：

```
static void _I_ViewController_viewDidLoad(ViewController * self, SEL _cmd)
{
    objc_msgSendSuper(self, class_getSuperclass(objc_getClass("ViewController"))}, sel_registerName("viewDidLoad"));

    {
        __AtAutoreleasePool __autoreleasepool;
        NSObject *obj = objc_msgSend(objc_msgSend(objc_getClass("NSObject"), sel_registerName("alloc")), sel_registerName("init"));
    }
}
```

对比代码，我们可以看到`@autoreleasepool{}`最终转成了`__AtAutoreleasePool `类型的结构体。

```
struct __AtAutoreleasePool {
    // 构造函数，创建此结构体变量时调用
    __AtAutoreleasePool() {
        atautoreleasepoolobj = objc_autoreleasePoolPush();
    }
    // 析构函数，销毁结构体时调用
    ~__AtAutoreleasePool() {
        objc_autoreleasePoolPop(atautoreleasepoolobj);
    }
    void * atautoreleasepoolobj;
};
```

也就是说`@autoreleasepool { NSObject *obj = [[NSObject alloc] init]; }`最终会转成如下代码：

```
void *atautoreleasepoolobj = objc_autoreleasePoolPush();

NSObject *obj = [[NSObject alloc] init];
    
objc_autoreleasePoolPop(atautoreleasepoolobj);
```

那我们要想搞清楚`AutoreleasePool`的底层实现，就要从`objc_autoreleasePoolPush()`和`objc_autoreleasePoolPop()`入手。这两个函数的实现在 runtime 源码中都能找到。


## 二、`AutoreleasePoolPage`


在 runtime4-750 的 NSObject.mm 中找到其实现。可以看到这两个函数其内部会调用 C++ 实现的类`AutoreleasePoolPage `中的方法。也就是说加入到 `@autoreleasepool`中的对象，都是借助`AutoreleasePoolPage `管理的。

```
void *objc_autoreleasePoolPush(void)
{
    return AutoreleasePoolPage::push();
}

void objc_autoreleasePoolPop(void *ctxt)
{
    AutoreleasePoolPage::pop(ctxt);
}
```


### 0x01 `AutoreleasePoolPage `结构

`AutoreleasePool`没有单独的结构，是通过一个或多个`AutoreleasePoolPage `类以双向链表的形式组合而成的栈结构。

```
// objc4-750 NSObject.mm

#define POOL_BOUNDARY  nil
#define I386_PGBYTES   4096 
#define PAGE_SIZE      I386_PGBYTES
#define PAGE_MAX_SIZE  PAGE_SIZE

class AutoreleasePoolPage 
{
	// 对当前 AutoreleasePoolPage 完整性的校验
    magic_t const magic;
    // 指向下一个 autorelease 对象将要存放的地址
    id *next;
    // 当前 AutoreleasePoolPage 所在的线程
    pthread_t const thread;
    // 双向链表 父节点 指向前一个 Page 对象
    AutoreleasePoolPage * const parent;
    // 双向链表 子节点 指向下一个 Page 对象
    AutoreleasePoolPage *child;
    // 链表深度 节点个数
    uint32_t const depth;
    // high water mark 数据容纳的一个上限
    uint32_t hiwat;
}
```




<br>

写于2019-08-11

<br>