
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

`AutoreleasePool`没有单独的结构，是通过一个或多个`AutoreleasePoolPage `类以双向链表的形式组合而成的栈结构。

![](https://images.gitee.com/uploads/images/2019/0814/114118_1d59eafa_1355277.png "AutoreleasePool_image0101.png")

每个`AutoreleasePoolPage`对象占用 4096 字节内存，除了用来存放它内部的成员变量，剩下的空间以压栈的方式存放`autorelease`对象的地址。指针`parent` 和指针`child` 使用于双向链表的指针。

![](https://images.gitee.com/uploads/images/2019/0814/114454_3c64c325_1355277.png "AutoreleasePool_image0102.png")

指针`next`指向下一个`autorelease`对象将要存放的地址。如果`next `指向的地址加入一个 `object`，它就会移动到下一个为空的内存地址中。

`begin()` 和 `end()`这两个函数会帮助我们快速获取`0x01038 ~ 0x02000`这一范围的边界地址。

### 0x02 `POOL_BOUNDARY`


`POOL_BOUNDARY `其实是`nil`的宏定义，用于标记每个`@autoreleasepool{}`的边界。

由上面知道每次初始化自动释放池`@autoreleasepool{}`，会调用构造方法`objc_autoreleasePoolPush()`，在这时会先把`POOL_BOUNDARY `压入栈，然后再将对象依次压入栈。

![](https://images.gitee.com/uploads/images/2019/0814/145130_d9fe4449_1355277.png "AutoreleasePool_image0103.png")

而当自动释放池销毁时会调用析构函数`objc_autoreleasePoolPop()`，这时会向自动释放池中的对象发送`release`消息，直到遇到`POOL_BOUNDARY `，这时说明此自动释放池中的对象已释放完毕。

4096 个字节除了用来存放它内部的成员变量，若剩下的内存空间不够存储一个自动释放池`@autoreleasepool{}`中的`autorelease`对象，则会新创建一个`AutoreleasePoolPage `对象，构成双向链表，在新的`AutoreleasePoolPage `对象中继续存储。

```
extern void _objc_autoreleasePoolPrint(void);

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        for (int i=0; i<600; i++) {
            NSObject *obj3 = [[[NSObject alloc] init] autorelease];
        }
        @autoreleasepool {
            NSObject *obj1 = [[[NSObject alloc] init] autorelease];
            NSObject *obj2 = [[[NSObject alloc] init] autorelease];
            _objc_autoreleasePoolPrint();
        }
    }
    return 0;
}

// 打印结果
objc[28420]: ##############
objc[28420]: AUTORELEASE POOLS for thread 0x1000a95c0
objc[28420]: 604 releases pending.
objc[28420]: [0x102006000]  ................  PAGE (full)  (cold)
objc[28420]: [0x102006038]  ################  POOL 0x102006038
objc[28420]: [0x102006040]       0x100603840  NSObject
objc[28420]: [0x102006048]       0x10060c240  NSObject
...
objc[28037]: [0x103002000]  ................  PAGE  (hot) 
objc[28037]: [0x103002038]       0x1005548b0  NSObject
...
objc[28420]: [0x101007338]  ################  POOL 0x101007338
objc[28420]: [0x101007340]       0x10062b3b0  NSObject
objc[28420]: [0x101007348]       0x10062b3c0  NSObject
```

![](https://images.gitee.com/uploads/images/2019/0814/145733_89c9ba5b_1355277.png "AutoreleasePool_image0104.png")

若剩下的内存空间足够存储一个自动释放池`@autoreleasepool{}`中的`autorelease`对象，则存储下一个自动释放池`@autoreleasepool{}`中的`autorelease`对象不会创建新的`AutoreleasePoolPage `对象。

```
int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSObject *obj3 = [[[NSObject alloc] init] autorelease];
        @autoreleasepool {
            NSObject *obj1 = [[[NSObject alloc] init] autorelease];
            NSObject *obj2 = [[[NSObject alloc] init] autorelease];
            
            @autoreleasepool {
                NSObject *obj4 = [[[NSObject alloc] init] autorelease];
                _objc_autoreleasePoolPrint();
            }
        }
    }
    return 0;
}

// 打印结果
objc[28465]: ##############
objc[28465]: AUTORELEASE POOLS for thread 0x1000a95c0
objc[28465]: 7 releases pending.
objc[28465]: [0x100801000]  ................  PAGE  (hot) (cold)
objc[28465]: [0x100801038]  ################  POOL 0x100801038
objc[28465]: [0x100801040]       0x100544240  NSObject
objc[28465]: [0x100801048]  ################  POOL 0x100801048
objc[28465]: [0x100801050]       0x10053a280  NSObject
objc[28465]: [0x100801058]       0x100544660  NSObject
objc[28465]: [0x100801060]  ################  POOL 0x100801060
objc[28465]: [0x100801068]       0x100544050  NSObject
objc[28465]: ##############
```

![](https://images.gitee.com/uploads/images/2019/0814/150111_0af09bb0_1355277.png "AutoreleasePool_image0105.png")

### 0x03 `push()`

```
static inline void *push()
{
    id *dest;
    if (DebugPoolAllocation) {
        // Each autorelease pool starts on a new pool page.
        dest = autoreleaseNewPage(POOL_BOUNDARY);
    } else {
        // 调用 objc_autoreleasePoolPush() 传入 POOL_BOUNDARY 当边界
        dest = autoreleaseFast(POOL_BOUNDARY);
    }
    assert(dest == EMPTY_POOL_PLACEHOLDER || *dest == POOL_BOUNDARY);
    return dest;
}

static inline id *autoreleaseFast(id obj)
{
    // hotPage 可以理解为当前正在使用的 AutoreleasePoolPage
    AutoreleasePoolPage *page = hotPage();
    if (page && !page->full()) {
        // 若 hotPage 存在且 hotPage 不满，则将 obj 压入 AutoreleasePoolPage 的栈中
        return page->add(obj);
    } else if (page) {
        // 若 hotPage 存在且 hotPage 已满，则新创建一个 AutoreleasePoolPage
        // 再调用 page->add(obj) 将 obj 压入 AutoreleasePoolPage 的栈中
        return autoreleaseFullPage(obj, page);
    } else {
        // 若 hotPage 不存在，则新创建一个 AutoreleasePoolPage
        // 再调用 page->add(obj) 将 obj 压入 AutoreleasePoolPage 的栈中
        return autoreleaseNoPage(obj);
    }
}
```

### 0x04 `autoreleaseFullPage()`

```
static __attribute__((noinline)) id *autoreleaseFullPage(id obj, AutoreleasePoolPage *page)
{
    // The hot page is full. 
    // Step to the next non-full page, adding a new page if necessary.
    // Then add the object to that page.
    assert(page == hotPage());
    assert(page->full()  ||  DebugPoolAllocation);
    
    // 从传入的 page 开始遍历双向链表，查找未满的 AutoreleasePoolPage
    do {
        // 若有子节点 将取子节点 AutoreleasePoolPage
        if (page->child) page = page->child;
        // 否则创建新的 AutoreleasePoolPage
        else page = new AutoreleasePoolPage(page);
    } while (page->full());
    
    // 将新建或已有未满的 AutoreleasePoolPage 设置为 hotPage
    setHotPage(page);
    // 将 obj 压入 AutoreleasePoolPage 的栈中
    return page->add(obj);
}
```

### 0x05 `autoreleaseNoPage()`

```
static __attribute__((noinline)) id *autoreleaseNoPage(id obj)
{
    // "No page" could mean no pool has been pushed
    // or an empty placeholder pool has been pushed and has no contents yet
    assert(!hotPage());

    bool pushExtraBoundary = false;
    if (haveEmptyPoolPlaceholder()) {
        // We are pushing a second pool over the empty placeholder pool
        // or pushing the first object into the empty placeholder pool.
        // Before doing that, push a pool boundary on behalf of the pool 
        // that is currently represented by the empty placeholder.
        pushExtraBoundary = true;
    }
    else if (obj != POOL_BOUNDARY  &&  DebugMissingPools) {
        // We are pushing an object with no pool in place, 
        // and no-pool debugging was requested by environment.
        _objc_inform("MISSING POOLS: (%p) Object %p of class %s "
                     "autoreleased with no pool in place - "
                     "just leaking - break on "
                     "objc_autoreleaseNoPool() to debug", 
                     pthread_self(), (void*)obj, object_getClassName(obj));
        objc_autoreleaseNoPool(obj);
        return nil;
    }
    else if (obj == POOL_BOUNDARY  &&  !DebugPoolAllocation) {
        // We are pushing a pool with no pool in place,
        // and alloc-per-pool debugging was not requested.
        // Install and return the empty pool placeholder.
        return setEmptyPoolPlaceholder();
    }

    // We are pushing an object or a non-placeholder'd pool.

    // Install the first page.
    //
    AutoreleasePoolPage *page = new AutoreleasePoolPage(nil);
    // 将 page 设置为 hotPage
    setHotPage(page);
    
    // Push a boundary on behalf of the previously-placeholder'd pool.
    if (pushExtraBoundary) {
        page->add(POOL_BOUNDARY);
    }
    
    // Push the requested object or pool.
    return page->add(obj);
}
```

## 三、`autorelease`


OC 对象调用`autorelease `方法就会将对象加入到自动释放池中。我们来看看`autorelease `的内部实现。

```
// objc4-750 

NSObject.mm
- (id)autorelease {
    return ((id)self)->rootAutorelease();
}

// objc-object.h
inline id objc_object::rootAutorelease()
{
    if (isTaggedPointer()) return (id)this;
    if (prepareOptimizedReturn(ReturnAtPlus1)) return (id)this;

    return rootAutorelease2();
}

// NSObject.mm
__attribute__((noinline,used)) id objc_object::rootAutorelease2()
{
    assert(!isTaggedPointer());
    // this 指调用 autorelease 的对象
    return AutoreleasePoolPage::autorelease((id)this);
}

// NSObject.mm
static inline id autorelease(id obj)
{
    assert(obj);
    assert(!obj->isTaggedPointer());
    id *dest __unused = autoreleaseFast(obj);
    assert(!dest  ||  dest == EMPTY_POOL_PLACEHOLDER  ||  *dest == obj);
    return obj;
}

// NSObject.mm
static inline id *autoreleaseFast(id obj)
{
    // hotPage 可以理解为当前正在使用的 AutoreleasePoolPage
    AutoreleasePoolPage *page = hotPage();
    if (page && !page->full()) {
        // 若 hotPage 存在且 hotPage 不满，则将 obj 压入 AutoreleasePoolPage 的栈中
        return page->add(obj);
    } else if (page) {
        // 若 hotPage 存在且 hotPage 已满，则新创建一个 AutoreleasePoolPage
        // 再调用 page->add(obj) 将 obj 压入 AutoreleasePoolPage 的栈中
        return autoreleaseFullPage(obj, page);
    } else {
        // 若 hotPage 不存在，则新创建一个 AutoreleasePoolPage
        // 再调用 page->add(obj) 将 obj 压入 AutoreleasePoolPage 的栈中
        return autoreleaseNoPage(obj);
    }
}
```




<br>

写于2019-08-11

<br>