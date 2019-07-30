
# 内存管理 - MRC

<br>

## 一、内存分区

内存有五大区域，从低地址到高地址依次为：代码段、数据段、BBS 段、堆、栈。

![](https://images.gitee.com/uploads/images/2019/0725/111739_3310bed1_1355277.png "MemoryManage_image0201.png")


- 代码段：存放编译之后的代码

- 数据段：存放程序中已经初始化的全局变量和静态变量、字符串常量等

- BBS 段：存放程序中未初始化的全局变量和静态变量

- 堆：通过`alloc`、`malloc`、`calloc`等动态分配的空间，也就是我们创建的对象。堆区的地址从低到高分配，一般有程序员管理。

- 栈：用于函数调用开销，如局部变量，函数跳转时下条汇编指令存储等。栈去的地址由高到低分配，遵循先进后出的原则，由系统管理。

```
int global_var1 = 10;
int global_var2;

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 数据段
    NSString *str1 = @"123";
    static int static_var1 = 12;
    // BBS 段
    static int static_var2;
    // 堆
    NSObject *obj = [[NSObject alloc] init];
    // 栈
    int stack = 18;
    
    NSLog(@"已初始化：str1:%p, global_var:%p, static_var:%p", str1, &global_var1, &static_var1);
    NSLog(@"未初始化：global_var2:%p, static_var2:%p", &global_var2, &static_var2);
    NSLog(@"堆：object:%p", obj);
    NSLog(@"栈：stack:%p", &stack);
}

// 打印结果
2019-07-25 11:45:27.565315+0800 MemoryManagement[85867:6312085] 已初始化：str1:0x10d932150, global_var:0x10d933488, static_var:0x10d93348c
2019-07-25 11:45:27.565467+0800 MemoryManagement[85867:6312085] 未初始化：global_var2:0x10d933554, static_var2:0x10d933550
2019-07-25 11:45:27.565544+0800 MemoryManagement[85867:6312085] 堆：object:0x600003f6f050
2019-07-25 11:45:27.565632+0800 MemoryManagement[85867:6312085] 栈：stack:0x7ffee22cd86c
```

**通过上面可知在 iOS 中所说的内存管理，管理的是堆区的内存。**


## 二、引用计数

### 0x01 简介

Objective-C 提供了两种内存管理机制：MRC(Mannul Reference Counting) 和 ARC(Automatic Reference Counting)，从 WWDC2011 后苹果就开始推行 ARC 来进行内存管理，其原理是通过编译器的静态分析在合适的位置插入`retain/release/autorelease`，从而解放程序员。

无论是 MRC 还是 ARC 都是通过引用计数来判断一个对象是否需要从内存中释放。当我们通过`alloc/new/copy/mutableCopy`得到一个对象时，它的引用计数为1，当一个新对象指向这个对象时，引用计数加1。当我们不需要这个对象时，通过`release`将引用计数减1，若引用计数为0，则这个对象从内存中释放(不是从内存中清空，只是将这块内存标记为可用)。

![](https://images.gitee.com/uploads/images/2019/0725/154059_6f23510b_1355277.png "MemoryManage_image0202.png")

一个对象的引用计数可以调用`retainCount`获得。不能向已被释放的对象发送消息。


### 0x02 源码解读

由 [runtime(一) - isa](https://github.com/zhaoName/Notes/blob/master/iOS/runtime(%E4%B8%80)%20-%20isa.md) 可知从 64bit 开始苹果使用位域技术对 isa 进行优化，优化后的引用计数由`has_sidetable_rc`决定存储在哪。若`has_sidetable_rc `值为0，则存储在`extra_rc`中，若`has_sidetable_rc `值为1，则存储在`SideTable`中的属性中。


`retainCount`是`NSObject`中的方法，且`NSObject`中的方法在 runtime 中已开源。

```
// objc4-750 NSObject.mm

struct SideTable {
    // 实质是 os_unfair_lock
    spinlock_t slock;
    // 存放引用计数的 Map
    RefcountMap refcnts;
    weak_table_t weak_table;
}

- (NSUInteger)retainCount {
    return ((id)self)->rootRetainCount();
}

inline uintptr_t objc_object::rootRetainCount()
{
    // 若是 Tagged Pointer 类型的指针 则直接返回
    if (isTaggedPointer()) return (uintptr_t)this;
    
    // os_unfair_lock 加锁
    sidetable_lock();
    isa_t bits = LoadExclusive(&isa.bits);
    ClearExclusive(&isa.bits);
    // nonpointer 为1代表优化过，使用位域存储更多的信息
    if (bits.nonpointer) {
        //
        uintptr_t rc = 1 + bits.extra_rc;
        // 引用计数器是否过大无法存储在isa中，如果为1，那么引用计数会存储在一个叫SideTable的类的属性中
        if (bits.has_sidetable_rc) {
            // 获取 SideTable 中的引用计数
            rc += sidetable_getExtraRC_nolock();
        }
        // os_unfair_lock 解锁
        sidetable_unlock();
        return rc;
    }

    sidetable_unlock();
    // 若是指针 则直接从 sidetable 中取
    return sidetable_retainCount();
}

size_t objc_object::sidetable_getExtraRC_nolock()
{
    assert(isa.nonpointer);
    // 先找到SideTable，再找到 refcnts
    SideTable& table = SideTables()[this];
    RefcountMap::iterator it = table.refcnts.find(this);
    if (it == table.refcnts.end()) return 0;
    else return it->second >> SIDE_TABLE_RC_SHIFT;
}
```



## 三、MRC

苹果文档中给出两个由内存管理不正确导致的问题：

- 释放或覆盖仍要使用的数据。这可能会导致内存损坏、程序崩溃，甚至是用户数据损坏。

- 不释放已不再使用的数据会导致内存泄露。内存泄露会导致程序使用的内存不断增加，从而导致系统性能下降或程序被强制被杀死。



现在 Xcode 版本都是默认在 ARC 环境，需要将`Porject->Build Setting->Objective-C Auotmatic Reference Counting`置为`NO`，切换到 MRC 环境。也可以在`Porject->Build Phases->Compile Sources`中的文件后面添加`-fno-objc-arc`，将单个文件切换到 MRC 环境。下面通过人和狗解释下 MRC。

### 0x01 一个人

```
// ZNPerson.m
- (void)dealloc
{
    NSLog(@"%s", __func__);
    [super dealloc];
}

// ViewController.m
- (void)viewDidLoad {
    [super viewDidLoad];
    
    ZNPerson *per = [[ZNPerson alloc] init];
    NSLog(@"retainCount: %zd", per.retainCount);
    // do something
    [per release];
}

// 打印结果
2019-07-25 16:30:11.064917+0800 MemoryManagement[90102:6443669] retainCount: 1
2019-07-25 16:30:11.065104+0800 MemoryManagement[90102:6443669] -[ZNPerson dealloc]
```

对于单个对象，没有对象指向它，它也没指向别的对象。它的内存管理就很简单。但实际编程中基本遇不到这种情况。


### 0x02 一个人一条狗

```
// ZNDog.m
- (void)walkingDog
{
    NSLog(@"%s", __func__);
}

- (void)dealloc
{
    NSLog(@"%s", __func__);
    [super dealloc];
}

// ZNPerson.m
@synthesize dog = _dog;

- (void)setDog:(ZNDog *)dog
{
    _dog = dog;
}

- (ZNDog *)dog
{
    return _dog;
}

- (void)dealloc
{
    NSLog(@"%s", __func__);
    [super dealloc];
}

// ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    
    ZNPerson *per = [[ZNPerson alloc] init];
    ZNDog *dog = [[ZNDog alloc] init];
    
    [per setDog:dog];
    [dog release];
   	 
    [[per dog] walkingDog];
    [per release];
}
```

运行结果出行坏内存访问。

![](https://images.gitee.com/uploads/images/2019/0725/171400_21a4c761_1355277.png "MemoryManage_image0203.png")

我们来看下`per`和`dog`的引用计数。`per`和`dog`创建出来的引用计数都是1，当执行到`[dog release]`时，`dog`的引用计数为0，从内存中释放。

`[[per dog] walkingDog]`相当于先获取`dog`，再向`dog`发送`walkingDog `消息，此时`per`引用计数还是1，但`dog`已被释放，这就造成坏内存访问。

也就是说我们若想通过`per`去访问`ZNDog`中的方法，`per`必须要持有`dog`对象。我们可以在`dog`的`setter`方法上动手脚。

```
// ZNPerson.m
- (void)setDog:(ZNDog *)dog
{
    // 引用计数加1 持有 dog
    _dog = [dog retain];
}

// 打印结果
2019-07-25 17:36:37.708362+0800 MemoryManagement[93579:6489980] -[ZNDog walkingDog]
2019-07-25 17:36:37.709644+0800 MemoryManagement[93579:6489980] -[ZNPerson dealloc]
```

这样虽然不会造成坏内存访问，但程序结束时`per`仍持有`dog`对象，`dog`的引用计数为1，不会从内存中释放，这造成内存泄露。这时我们需要在`per`释放前，将`dog`的引用计数减1。

```
// ZNPerson.m
- (void)dealloc
{
    [_dog release];
    _dog = nil;
    NSLog(@"%s", __func__);
    [super dealloc];
}

// 打印结果
2019-07-25 17:43:46.309273+0800 MemoryManagement[93721:6494351] -[ZNDog walkingDog]
2019-07-25 17:43:46.309486+0800 MemoryManagement[93721:6494351] -[ZNDog dealloc]
2019-07-25 17:43:46.309596+0800 MemoryManagement[93721:6494351] -[ZNPerson dealloc]
```

### 0x03 一个人多条狗

这里的多条狗是指创建多条狗，而不是一个人同时持有多条狗。

```
- (void)viewDidLoad {
    [super viewDidLoad];
    
    ZNPerson *per = [[ZNPerson alloc] init];
    ZNDog *dog1 = [[ZNDog alloc] init]; // dog1 retainCount:1
    ZNDog *dog2 = [[ZNDog alloc] init]; // dog2 retainCount:1
    
    [per setDog:dog1]; // dog1 retainCount:2
    [per setDog:dog2]; // dog2 retainCount:2
    [dog1 release]; // dog1 retainCount:1
    [dog2 release]; // dog2 retainCount:1
    
    [[per dog] walkingDog];
    [per release]; // dog2 retainCount:0
    NSLog(@"dog1.retainCount: %zd", dog1.retainCount);
}

// 打印结果
2019-07-25 18:00:46.808638+0800 MemoryManagement[94075:6505084] -[ZNDog walkingDog]
2019-07-25 18:00:46.808831+0800 MemoryManagement[94075:6505084] -[ZNDog dealloc]
2019-07-25 18:00:46.808967+0800 MemoryManagement[94075:6505084] -[ZNPerson dealloc]
2019-07-25 18:00:46.809074+0800 MemoryManagement[94075:6505084] dog1.retainCount:1
```

可以看到程序结束时`dog2`被释放了，但`dog1`的引用计数仍为1，没有被释放。解决办法是`per`更换持有的`ZNDog`对象时，要将老的`dog1`对象的引用计数减1，新的`dog`对象的引用计数加1。

```
- (void)setDog:(ZNDog *)dog
{
    // 老的 dog 对象的引用计数减1，新的 dog 对象的引用计数加1
    [_dog release];
    _dog = [dog retain];
}

// 打印结果
2019-07-25 18:07:39.206459+0800 MemoryManagement[94213:6509941] -[ZNDog dealloc]
2019-07-25 18:07:39.206689+0800 MemoryManagement[94213:6509941] -[ZNDog walkingDog]
2019-07-25 18:07:39.206813+0800 MemoryManagement[94213:6509941] -[ZNDog dealloc]
2019-07-25 18:07:39.207118+0800 MemoryManagement[94213:6509941] -[ZNPerson dealloc]
```

### 0x04 多个人一条狗

```
- (void)viewDidLoad {
    [super viewDidLoad];
    
    ZNPerson *per1 = [[ZNPerson alloc] init]; // per1 1
    ZNPerson *per2 = [[ZNPerson alloc] init]; // per2 1
    ZNDog *dog = [[ZNDog alloc] init]; // dog1 retainCount:1
    
    [per1 setDog:dog]; // dog retainCount:2
    [per2 setDog:dog]; // dog retainCount:3
    [dog release]; // dog retainCount:2
    
    [[per1 dog] walkingDog];
    [per1 release]; // dog retainCount:1
    
    [[per2 dog] walkingDog];
    [per2 release]; // dog retainCount:0
}

// 打印结果
2019-07-25 22:31:01.042169+0800 MemoryManagement[94959:6601938] -[ZNDog walkingDog]
2019-07-25 22:31:01.042332+0800 MemoryManagement[94959:6601938] -[ZNPerson dealloc]
2019-07-25 22:31:01.042441+0800 MemoryManagement[94959:6601938] -[ZNDog walkingDog]
2019-07-25 22:31:01.042529+0800 MemoryManagement[94959:6601938] -[ZNDog dealloc]
2019-07-25 22:31:01.042603+0800 MemoryManagement[94959:6601938] -[ZNPerson dealloc]
```

代码优化到这，多个人同时持有一条狗是没有内存泄露的。


### 0x05 一个人重复持有一条狗

```
- (void)viewDidLoad {
    [super viewDidLoad];
    
    ZNPerson *per = [[ZNPerson alloc] init];
    ZNDog *dog = [[ZNDog alloc] init]; // dog retainCount:1
    
    [per setDog:dog]; // dog retainCount:2
    [dog release]; // dog retainCount:1
    
    [per setDog:dog];
    [per setDog:dog];
    
    [per release];
}
```

运行结果是程序崩溃，出现野指针访问。

![](https://images.gitee.com/uploads/images/2019/0725/225017_7a6fc4ad_1355277.png "MemoryManage_image0204.png")


对于一个人重复持有一条狗，我们完全没有必要先`release`再`retain`，就像养一条狗，丢了后重新找回，是不会将以前的养狗证销毁，重新办一张的。

```
- (void)setDog:(ZNDog *)dog
{
    if (_dog != dog) {
        [_dog release];
        _dog = [dog retain];
    }
}

// 打印结果
2019-07-25 22:58:37.065653+0800 MemoryManagement[95584:6776281] -[ZNDog dealloc]
2019-07-25 22:58:37.065826+0800 MemoryManagement[95584:6776281] -[ZNPerson dealloc]
```

### 0x06 总结

至此 MRC 环境下对象的内存管理已经成型，所有对象的`setter`方法都会写成这样(包括 ARC 环境下自动生成的代码)。

```
- (void)setXxx:(XXX *)xxx
{
    if (_xxx != xxx) {
        [_xxx release];
        _xxx = [xxx retain];
    }
}
```

这样写`setter`方法只能保证对象的`retainCount`最小值为1，不能被释放，所以还要注意一点在持有对象释放时，持有对象的`dealloc`方法中一定要调用被持有对象的`release`方法。

```
// ZNPerson.m
- (void)dealloc
{
    [_dog release];
    _dog = nil;
    NSLog(@"%s", __func__);
    [super dealloc];
}
```


<br>

**参考：**

- [Introducing Automatic Reference Counting](https://developer.apple.com/videos/play/wwdc2011/323/)

- [Advanced Memory Management Programming Guide](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/MemoryMgmt/Articles/MemoryMgmt.html#//apple_ref/doc/uid/10000011-SW1)

<br>

写于2019-07-25

<br>