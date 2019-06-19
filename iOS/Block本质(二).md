# Block本质(二)

<br>

没有特殊说明，默认都是在`ARC`环境下。


## 一、捕获对象类型变量

创建`ZNPeron`类, 下面代码`per`对象什么时候被释放？

```
ZNPerson.h
@property (nonatomic, copy) NSString *name;

ZNPeron.m
- (void)dealloc
{
    NSLog(@"%s", __func__);
    // MRC 下加下句代码
    //[super dealloc];
}

// main.m
int main(int argc, const char * argv[]) {
    @autoreleasepool {
    
        ZNBlock block;
		{
		    ZNPerson *per = [[ZNPerson alloc] init];
		    per.name = @"zhaoName";
		    
		    block = ^{
		         NSLog(@"per.age：%@", per.name);
		    };
		    // MRC 下加下句代码
		    //[per release];
		}
		block();
		NSLog(@"========");
    }
    return 0;
}

```

- `MRC` 下打印结果

```
2019-06-17 23:49:00.987411+0800 BlockNature[9555:9755205] -[ZNPerson dealloc]
2019-06-17 23:49:00.987758+0800 BlockNature[9555:9755205] per.age：zhaoName
2019-06-17 23:49:00.987913+0800 BlockNature[9588:9768138] ========
```

- `ARC` 下打印结果

```
2019-06-17 23:51:06.082531+0800 BlockNature[9588:9768138] per.age：zhaoName
2019-06-17 23:51:06.082731+0800 BlockNature[9588:9768138] ========
2019-06-17 23:51:06.082952+0800 BlockNature[9588:9768138] -[ZNPerson dealloc]
```

可以看出在`MRC`环境下，作为局部变量的`per`出了大括号作用域就被释放了。而在`ARC`环境下，作为局部变量的`per`在`ZNBlock`被释放后才从内存释放。这是为啥呢？

由上篇文章可知，在`MRC`环境下`ZNBlock`是`__NSStackBlock__ `类型的`Block`，存储在栈上，栈中的数据会在出作用域之后销毁。

在`ARC`环境下`ZNBlock`是`__NSMallocBlock__ `类型的`Block`，存储在堆上。`per `又被`ZNBlock `强引用着，所以`per`在`ZNBlock `销毁之后才被释放。


转换成`C++`代码看其底层实现

```
typedef void(*ZNBlock)(void);

struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    // 捕获的对象 并用 __strong 修饰
    ZNPerson *__strong per;
    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, ZNPerson *__strong _per, int flags=0) : per(_per) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself)
{
    ZNPerson *__strong per = __cself->per; // bound by copy

    NSLog((NSString *)&__NSConstantStringImpl__var_folders_cl_ztrmqfr12jd289xtmjfhsr340000gn_T_main_8b0473_mi_1, ((NSString *(*)(id, SEL))(void *)objc_msgSend)((id)per, sel_registerName("name")));
}

// 为管理捕获对象的内存而新增函数，相当于 retain 方法
static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src)
{
    _Block_object_assign((void*)&dst->per, (void*)src->per, 3/*BLOCK_FIELD_IS_OBJECT*/);
}

// 为管理捕获对象的内存而新增函数，相当于 release 方法
static void __main_block_dispose_0(struct __main_block_impl_0*src)
{
    _Block_object_dispose((void*)src->per, 3/*BLOCK_FIELD_IS_OBJECT*/);
}

static struct __main_block_desc_0 {
    size_t reserved;
    size_t Block_size;
    // 为管理捕获对象的内存而增加的两个成员
    void (*copy)(struct __main_block_impl_0*, struct __main_block_impl_0*);
    void (*dispose)(struct __main_block_impl_0*);
} __main_block_desc_0_DATA = { 0, sizeof(struct __main_block_impl_0), __main_block_copy_0, __main_block_dispose_0};

int main(int argc, const char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 

        ZNBlock block;
        {
            ZNPerson *per = ((ZNPerson *(*)(id, SEL))(void *)objc_msgSend)((id)((ZNPerson *(*)(id, SEL))(void *)objc_msgSend)((id)objc_getClass("ZNPerson"), sel_registerName("alloc")), sel_registerName("init"));
            
            ((void (*)(id, SEL, NSString * _Nonnull))(void *)objc_msgSend)((id)per, sel_registerName("setName:"), (NSString *)&__NSConstantStringImpl__var_folders_cl_ztrmqfr12jd289xtmjfhsr340000gn_T_main_8b0473_mi_0);

            block = ((void (*)())&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, per, 570425344));
        }
        ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)((__block_impl *)block);
        NSLog((NSString *)&__NSConstantStringImpl__var_folders_cl_ztrmqfr12jd289xtmjfhsr340000gn_T_main_8b0473_mi_2);
    }
    return 0;
}
```

在`OC`中，`C`语言的结构体中是不能含有带有`__strong`修饰符的变量。因为编译器不知道什么进行`C`语言结构体的初始化和释放操作，不能很好的管理内存。

但从上面源代码可以看出`__main_block_desc_0`结构体中增加了`copy`和`dispose`两个成员变量，以及作为指针赋值给这两个变量的`__main_block_copy_0`和`__main_block_dispose_0`函数。这样即使`Block`中含有`__strong`或`_weak`修饰符，运行时也能准确的把握结构体的初始化和释放时机。

`__main_block_copy_0`函数内调用`_Block_object_assign`函数将`per`对象赋值给`ZNBlock`结构体中的成员变量`ZNPerson *__strong per`,并持有该对象。相对于`MRC`环境下调用`retain`方法。

```
static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src)
{
    _Block_object_assign((void*)&dst->per, (void*)src->per, 3/*BLOCK_FIELD_IS_OBJECT*/);
}
```

`__main_block_dispose_0`函数内调用`_Block_object_dispose`函数将`ZNBlock`结构体中的成员释放掉。相对于`MRC`环境下调用`release`方法。

```
static void __main_block_dispose_0(struct __main_block_impl_0*src)
{
    _Block_object_dispose((void*)src->per, 3/*BLOCK_FIELD_IS_OBJECT*/);
}
```

那`__main_block_copy_0 `和`__main_block_dispose_0 `函数什么时候调用呢？

![](https://images.gitee.com/uploads/images/2019/0618/233640_655815ca_1355277.png "1111.png")


**总结: 当`Block `内部访问了对象类型的`auto `变量时**

- 若`Block`在栈上，则不会对捕获的对象类型`auto`变量强引用

- 若`Block`被拷贝到堆上

	- 会调用`Block`内部的`__main_block_copy_0 `函数

	- `__main_block_copy_0 `函数内部调用`_Block_object_assign `函数

	- `_Block_object_assign `函数会根据被捕获对象类型变量的修饰符(`__strong`、`__weak`、`__unsafe_unretained`)做出相应操作 - 对对象类型`auto `变量强引用或弱引用。

	
- 若`Block`从堆上移除

	- 会调用`Block`内部的`__main_block_dispose_0 `函数

	- `__main_block_dispose_0 `函数内部调用`_Block_object_dispose `函数

 	- `_Block_object_dispose `函数会自动释放引用的`auto `变量（相当于`release `）


<br>

## 二、`__block`修饰符


### 0x01 为什么`Block`内部无法修改`auto`变量的值

有个很经典的错误，在`Block`内直接修改`auto`变量的值会直接报错。

```
int age = 10;
NSLog(@"block外：%p", &age);

ZNBlock block = ^{
	NSLog(@"block内：%p", &age);
	// 下句代码报错：variable is not assignable (missing __block type specifier)
    age = 20;
};
block();
```

想要看清楚为什么不能改值，还是要看底层源码(去掉报错代码)

```
struct __main_block_impl_0 {
	struct __block_impl impl;
	struct __main_block_desc_0* Desc;
	int age;
	__main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, int _age, int flags=0) : age(_age) {
		impl.isa = &_NSConcreteStackBlock;
		impl.Flags = flags;
		impl.FuncPtr = fp;
		Desc = desc;
	}
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself)
{
    int age = __cself->age; // bound by copy
    NSLog((NSString *)&__NSConstantStringImpl__var_folders_cl_ztrmqfr12jd289xtmjfhsr340000gn_T_main_240b87_mi_0, age);
}

int main(int argc, const char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 

        int age = 10;
        ZNBlock block = ((void (*)())&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, age));
        
        ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)((__block_impl *)block);
    }
    return 0;
}
```

我们实际想要的是在`Block`内部修改`main`函数中局部变量`age`的值，但从底层实现来看写在`Block`内部的代码在`__main_block_func_0 `函数中，已经不在`main`函数中，已经超出`main`函数中局部变量`age`的作用域，当然会出错。

另外从打印结果也可以看到`Block`内外，变量`age`内存地址不一样。

```
2019-06-18 15:17:03.805430+0800 BlockNature[3095:144777] block外：0x7ffeefbff57c
2019-06-18 15:17:03.805877+0800 BlockNature[3095:144777] block内：0x1005335e0
```

`Block`内的变量`age`是其捕获的变量，是`__main_block_impl_0 `结构体中的成员变量`age`。想要访问结构体内的成员，要先得到结构体对象实例。而`__main_block_impl_0 `结构体在实际编程中是看不到的。


### 0x02 使用`C`语言中的变量在`Block`内修改其值

上面说到不能在`Block`内部修改局部`auto`变量是因为局部`auto`变量在栈上，超出其作用域就会销毁。那我们将`auto`变放到数据段不就可以了吗！`C`语言中有三种变量放在数据段

- 静态局部变量

- 静态全局变量

- 全局变量


我们看下下面这段源代码

```
int global_age = 10;
static int static_global_num =15;

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        static int static_height = 12;
        ZNBlock block = ^{
            global_age++;
            static_global_num++;
            static_height++;
        };
        block();
    }
    return 0;
}
```

转换后部分代码如下

```
int global_age = 10;
static int static_global_num =15;

struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    // 只捕获静态局部变量
    int *static_height;
    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, int *_static_height, int flags=0) : static_height(_static_height) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself)
{
    int *static_height = __cself->static_height; // bound by copy
    global_age++;
    static_global_num++;
    (*static_height)++;
}
```

这个结果在上篇文章已经介绍过了，大家应该都很熟悉了。静态全局变量和全局变量都是直接访问，而局部静态变量是通过指针访问的。

但是这样有点不好地方在于每次想修改`Block`内部的变量都会在数据段创建一个变量，直到程序运行结束才会被销毁。有点浪费内存的感觉。。。

在`OC`中苹果提供另外一个修饰符`__block`来在`Block`内部修改变量值。


### 0x03 `__block`修饰的基本数据类型变量

在前面编译报错的代码中的基本数据类型变量`age`声明前加上`__block`修饰符

```
__block int age = 12;
ZNBlock block = ^{
    age++;
};
block();
```

转换后代码如下

```
typedef void(*ZNBlock)(void);

// __block 修饰变量转换成的结构体，内部有个成员 age
struct __Block_byref_age_0 {
     void *__isa;
   	__Block_byref_age_0 *__forwarding;
     int __flags;
     int __size;
     int age;
};

struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    __Block_byref_age_0 *age; // by ref
    // 注意这里是将 _age->__forwarding 赋值给了结构体变量 age！！！ 为什么这么做？ 后面会给出解释
    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, __Block_byref_age_0 *_age, int flags=0) : age(_age->__forwarding) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself)
{
    __Block_byref_age_0 *age = __cself->age; // bound by ref
    (age->__forwarding->age)++;
}

// 为了持有 __Block_byref_age_0 *age 而新增代码
static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src)
{
    _Block_object_assign((void*)&dst->age, (void*)src->age, 8/*BLOCK_FIELD_IS_BYREF*/);
}

// 为了释放 __Block_byref_age_0 *age 而新增代码
static void __main_block_dispose_0(struct __main_block_impl_0*src)
{
    _Block_object_dispose((void*)src->age, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static struct __main_block_desc_0 {
	size_t reserved;
	size_t Block_size;
	// __Block_byref_age_0 *age 是个对象 也需要内存管理，所以新增 copy 和 dispose 成员变量
	void (*copy)(struct __main_block_impl_0*, struct __main_block_impl_0*);
	void (*dispose)(struct __main_block_impl_0*);
} __main_block_desc_0_DATA = { 0, sizeof(struct __main_block_impl_0), __main_block_copy_0, __main_block_dispose_0};


int main(int argc, const char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 

        // __block 修饰的 age 变量转换成 __Block_byref_age_0 类型的结构体变量 age
		__Block_byref_age_0 age = {(void*)0,(__Block_byref_age_0 *)&age, 0, sizeof(__Block_byref_age_0), 12};
        // block 定义
        ZNBlock block = ((void (*)())&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, (__Block_byref_age_0 *)&age, 570425344));
        // 调用 block
        ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)((__block_impl *)block);
    }
    return 0;
}
```

- `__Block_byref_age_0`结构体

```
struct __Block_byref_age_0 {
	void *__isa;
	__Block_byref_age_0 *__forwarding;
	int __flags;
	int __size;
	int age;
};

// __block int age = 12;
// __block 修饰的变量会包装成一个 __Block_byref_age_0 类型的结构体，其内有个同名成员变量 age 值为12
// main 函数中调用其构造函数,可以看出其成员变量 __forwarding  指针指向自己
__Block_byref_age_0 age = {(void*)0,(__Block_byref_age_0 *)&age, 0, sizeof(__Block_byref_age_0), 12};
```



可以看到`__block`修饰的变量会变成`__Block_byref_age_0 `类型的结构体，其内部也有个成员变量`age`。

![](https://images.gitee.com/uploads/images/2019/0618/233826_505764f4_1355277.png "222.png")

- 下面来解释下为什么能在`Block`内部修改带有`__block `修饰符的变量？

```
__block int age = 12;
NSLog(@"进入block前：%p", &age);
ZNBlock block = ^{
    age = 15;
    NSLog(@"block中 %p", &age);
    age++;
};
struct __main_block_impl_0 *ipl = (__bridge struct __main_block_impl_0 *)block;
    
NSLog(@"__block结构体中： %p",  &(ipl->age->__forwarding->age));
NSLog(@"block后：%p",  &age);
block();


// 在 ARC 环境下打印结果
2019-06-18 21:13:51.735833+0800 BlockNature[5174:254082] 进入block前：0x7ffeefbff578
2019-06-18 21:13:51.736109+0800 BlockNature[5174:254082] __block结构体中： 0x100515728
2019-06-18 21:13:51.736167+0800 BlockNature[5174:254082] block后：0x100515728
2019-06-18 21:13:51.736198+0800 BlockNature[5174:254082] block中 0x100515728

// 在 MRC 环境下打印结果
2019-06-18 21:18:45.386727+0800 BlockNature[5213:256398] 进入block前：0x7ffeefbff578
2019-06-18 21:18:45.387022+0800 BlockNature[5213:256398] __block结构体中： 0x7ffeefbff578
2019-06-18 21:18:45.387037+0800 BlockNature[5213:256398] block后：0x7ffeefbff578
2019-06-18 21:18:45.387082+0800 BlockNature[5213:256398] block中 0x7ffeefbff578
```

可以看到`__block`修饰的变量`age`在被捕获到`Block`后，无论是在`Block`内还是在`__Block_byref_age_0 `结构体中还是`Block`外，都是同一个变量。所以能在`Block`内部改值。


- 为什么单独定义`__block`变量的`__Block_byref_age_0 `结构体。


有两个`block`访问同一个`__block`修饰的变量

```
__block int age = 12;
ZNBlock block_first = ^{
	age = 13;
};
ZNBlock block_sec = ^{
	age = 14;
};

```

转换代码

```
 ZNBlock block_first = &__main_block_impl_0(__main_block_func_0, &__main_block_desc_0_DATA, (__Block_byref_age_0 *)&age, 570425344));

ZNBlock block_sec = &__main_block_impl_1(__main_block_func_1, &__main_block_desc_1_DATA, (__Block_byref_age_0 *)&age, 570425344));
```

`block_first `和`block_sec `使用同一个`__Block_byref_age_0 `结构体类型的变量`age`。这也是单独定义`__block`变量的`__Block_byref_age_0 `结构体，而不是将其放在`Block`变量的`__main_block_impl_0`结构体中的原因。


### 0x04 `__block`的内存管理


- `Block`在栈上

若`Block`在栈上，当超出其作用域后，`Block`和`__block`变量都会被释放。

![](https://images.gitee.com/uploads/images/2019/0618/233901_65103114_1355277.png "333.png")

- `Block`在堆上

若`Block`从栈上拷贝到堆上，则`__block`变量也会拷贝到堆上。复制上面代码

```
// 为了持有 __Block_byref_age_0 *age 而新增代码
static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src)
{
    _Block_object_assign((void*)&dst->age, (void*)src->age, 8/*BLOCK_FIELD_IS_BYREF*/);
}

// 为了释放 __Block_byref_age_0 *age 而新增代码
static void __main_block_dispose_0(struct __main_block_impl_0*src)
{
    _Block_object_dispose((void*)src->age, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static struct __main_block_desc_0 {
	size_t reserved;
	size_t Block_size;
	// __Block_byref_age_0 *age 是个对象 也需要内存管理，所以新增 copy 和 dispose 成员变量
	void (*copy)(struct __main_block_impl_0*, struct __main_block_impl_0*);
	void (*dispose)(struct __main_block_impl_0*);
} __main_block_desc_0_DATA = { 0, sizeof(struct __main_block_impl_0), __main_block_copy_0, __main_block_dispose_0};
```

为了管理`__block`变量内存，也会像`Block`捕获对象类型变量一样新增`__main_block_copy_0 `和`__main_block_dispose_0 `函数。

当`Block`被拷贝到堆上时，会调用`__main_block_copy_0 `函数，`__main_block_copy_0 `函数内部调用`_Block_object_assign `函数对`__block`进行强引用。

![](https://images.gitee.com/uploads/images/2019/0619/100205_a21fb68d_1355277.png "Screen Shot 2019-06-19 at 10.01.19 AM.png")

当`Block`从堆上销毁时，会调用`__main_block_dispose_0 `函数，`__main_block_dispose_0 `函数内部调用`_Block_object_dispose `函数，自动释放`__block`变量。

![](https://images.gitee.com/uploads/images/2019/0618/234018_76d121f4_1355277.png "555.png")


- 捕获对象类型的`auto `变量和`__block `变量的区别

通过`BLOCK_FIELD_IS_BYREF`和`BLOCK_FIELD_IS_OBJECT`参数来区分`__main_block_copy_0 `和`__main_block_dispose_0 `函数的对象类型是对象还是`__block`变量。

`Block`对`__block`变量肯定是强引用，但对捕获的对象类型变量是强引用还是弱引用 取决于当初对对象类型变量是强引用还是弱引用。


- `__block`变量用结构体成员`__forwarding`的原因

`__block`变量从栈上复制到堆上时，会将成员`__forwarding `的值替换成复制到目标堆上的`__block`变量的值。这样无论`__block`在栈上还是在堆上都能正确的访问该变量。


![](https://images.gitee.com/uploads/images/2019/0618/234050_80445b62_1355277.png "666.png")


在`MRC`环境下测试：

![输入图片说明](https://images.gitee.com/uploads/images/2019/0618/235054_e1e69d88_1355277.jpeg "WechatIMG113.jpeg")



### 0x05 `__block`修饰的对象类型


```
__block ZNPerson *per = [[ZNPerson alloc] init];
ZNBlock block = ^{
    NSLog(@"ZNPerson： %@",  per);
};
block();
```

转成`C++`代码

```
typedef void(*ZNBlock)(void);

// 为持有 ZNPerson *__strong per 而新增 copy 函数,相当于 retain
static void __Block_byref_id_object_copy_131(void *dst, void *src)
{
    _Block_object_assign((char*)dst + 40, *(void * *) ((char*)src + 40), 131);
}

// 为释放 ZNPerson *__strong per 而新增 dispose函数，相当于release
static void __Block_byref_id_object_dispose_131(void *src)
{
    _Block_object_dispose(*(void * *) ((char*)src + 40), 131);
}

// __block 变量
struct __Block_byref_per_0 {
    void *__isa;
    __Block_byref_per_0 *__forwarding;
    int __flags;
    int __size;
    // 为管理 ZNPerson *__strong per 内存 而新增两个成员
    void (*__Block_byref_id_object_copy)(void*, void*);
    void (*__Block_byref_id_object_dispose)(void*);
    ZNPerson *__strong per;
};

// Block 结构体
struct __main_block_impl_0 {
    struct __block_impl impl;
    struct __main_block_desc_0* Desc;
    __Block_byref_per_0 *per; // by ref
    __main_block_impl_0(void *fp, struct __main_block_desc_0 *desc, __Block_byref_per_0 *_per, int flags=0) : per(_per->__forwarding) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};

static void __main_block_func_0(struct __main_block_impl_0 *__cself)
{
    __Block_byref_per_0 *per = __cself->per; // bound by ref
    NSLog((NSString *)&__NSConstantStringImpl__var_folders_cl_ztrmqfr12jd289xtmjfhsr340000gn_T_main_f3641e_mi_0, (per->__forwarding->per));
}

// 为持有 __Block_byref_per_0 *per 而新增 copy 函数,相当于 retain
static void __main_block_copy_0(struct __main_block_impl_0*dst, struct __main_block_impl_0*src)
{
    _Block_object_assign((void*)&dst->per, (void*)src->per, 8/*BLOCK_FIELD_IS_BYREF*/);
}

// 为释放 __Block_byref_per_0 *per 而新增 dispose函数，相当于release
static void __main_block_dispose_0(struct __main_block_impl_0*src)
{
    _Block_object_dispose((void*)src->per, 8/*BLOCK_FIELD_IS_BYREF*/);
}

static struct __main_block_desc_0 {
    size_t reserved;
    size_t Block_size;
    // 为管理  __Block_byref_per_0 *per 内存 而新增两个成员
    void (*copy)(struct __main_block_impl_0*, struct __main_block_impl_0*);
    void (*dispose)(struct __main_block_impl_0*);
} __main_block_desc_0_DATA = { 0, sizeof(struct __main_block_impl_0), __main_block_copy_0, __main_block_dispose_0};


int main(int argc, const char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 

        __Block_byref_per_0 per = {
            (void*)0,(__Block_byref_per_0 *)&per,
            33554432,
            sizeof(__Block_byref_per_0),
            __Block_byref_id_object_copy_131,
            __Block_byref_id_object_dispose_131,
            [[ZNPerson alloc] init]};
        }
    ...
}
```

当`__block`修饰的是对象类型的变量，在`__block`变量结构体`__Block_byref_per_0`中新增`__Block_byref_id_object_copy`和`__Block_byref_id_object_dispose`两个成员变量, 以及作为指针赋值给这两个成员的`__Block_byref_id_object_copy_131 `和`__Block_byref_id_object_dispose_131 `函数，来管理`ZNPerson *__strong per`的内存。

```
// 持有 ZNPerson *__strong per 而新增 copy 函数,相当于 retain
static void __Block_byref_id_object_copy_131(void *dst, void *src)
{
	// 由 __Block_byref_per_0 结构体可知 dst + 40 就是 per 的地址
    _Block_object_assign((char*)dst + 40, *(void * *) ((char*)src + 40), 131);
}

// 释放 ZNPerson *__strong per 而新增 dispose函数，相当于release
static void __Block_byref_id_object_dispose_131(void *src)
{
    _Block_object_dispose(*(void * *) ((char*)src + 40), 131);
}

struct __Block_byref_per_0 {
    void *__isa;
    __Block_byref_per_0 *__forwarding;
    int __flags;
    int __size;
    // 为管理 ZNPerson *__strong per 而新增两个成员
    void (*__Block_byref_id_object_copy)(void*, void*);
    void (*__Block_byref_id_object_dispose)(void*);
    ZNPerson *__strong per;
};
```

对`__block`修饰的对象类型的内存管理 大致如下图

![](https://images.gitee.com/uploads/images/2019/0619/105410_52f8eb08_1355277.jpeg "1560912785999.jpg")

- 当强引用`per `时

![](https://images.gitee.com/uploads/images/2019/0619/141103_694c3074_1355277.png "1560913593390.png")

- 当弱引用`per `时

![](https://images.gitee.com/uploads/images/2019/0619/141119_cd4ff984_1355277.png "1560913737268.png")


但在`MRC`环境下，`Block`永远不会强引用`__block`修饰的对象类型变量。

![](https://images.gitee.com/uploads/images/2019/0619/144515_d5178052_1355277.png "1560926668259.png")

这也是在`MRC`环境下，`__block`能解决循环引用的原因！

<br>

## 三、循环引用


若在`Block`内使用`__strong`修饰的对象类型变量，那么当`Block`从栈上复制到堆上时，`Block`会强引用该对象。这样很容易产生循环引用

```
// ZNPerson.h
typedef void(^ZNBlock)(void);

@interface ZNPerson : NSObject

@property (nonatomic, copy) ZNBlock block;/**< */
@property (nonatomic, copy) NSString *name;/**< */

- (void)test;
@end

// ZNPeron.m
- (void)test
{
    self.block = ^{
        NSLog(@"name is %@", self.name);
    };
    self.block();
}

- (void)dealloc
{
    NSLog(@"%s", __func__);
    // MRC 加上下面代码
    //[super dealloc];
}

// main.m
int main(int argc, const char * argv[]) {
        
    ZNPerson *per = [[ZNPerson alloc] init];
    per.name = @"zhao";
    [per test];
}
```

上面代码中的`dealloc`方法一定没有被调用，这就是循环运用导致的。

`ZNBlock`是对象`per`的成员，被`per`持有。在`ZNBlock`中访问了成员变量`name`，由上篇文章可知，`ZNBlock`内部会捕获`self (per)`变量，并持有它。`per`持有`ZNBlock`， `ZNBlock`持有`per`就导致了循环引用。

![](https://images.gitee.com/uploads/images/2019/0619/184519_e0b9de3d_1355277.png "1111.png")


### 0x01 `__weak ` 

由前面`Block`捕获对象类型变量的总结可知，`ZNBlock`是否持有对象类型变量取决于对象类型变量的修饰符是`__strong` 还是`__weak`、`__unsafe_unretained`。

```
// ZNPreson.m
- (void)test
{
    __weak typeof (self) weakSelf = self;
    self.block = ^{
        NSLog(@"name is %@", weakSelf.name);
    };
    self.block();
}

// 打印结果
2019-06-19 16:59:43.846754+0800 BlockNature[14379:2181645] name is zhao
2019-06-19 17:00:22.600143+0800 BlockNature[14379:2181645] -[ZNPerson dealloc]
```

### 0x02 `__unsafe_unretained ` 


```
- (void)test
{
	__unsafe_unretained ZNPerson *weakSelf = self;
    self.block = ^{
        NSLog(@"name is %@", weakSelf.name);
    };
    self.block();
}

// 打印结果
2019-06-19 17:08:20.955747+0800 BlockNature[14508:2185864] name is zhao
2019-06-19 17:08:24.310285+0800 BlockNature[14508:2185864] -[ZNPerson dealloc]
```


### 0x03 `__block ` 

```
// ZNPerson.m
- (void)test
{
    __block ZNPerson *weakSelf = self;
    self.block = ^{
        NSLog(@"name is %@", weakSelf.name);
        weakSelf = nil;
    };
    self.block();
}

// 打印结果
2019-06-19 18:35:24.183795+0800 BlockNature[17091:2230005] name is zhao
2019-06-19 18:35:24.184131+0800 BlockNature[17091:2230005] -[ZNPerson dealloc]
```

当没有在`test`方法中执行`self.block();`时，`ZNPerson`对象持有`ZNBlock`， `ZNBlock`内部持有`__block`变量，`__block`变量又持有`ZNPerson`对象。这样就造成循环引用。

![](https://images.gitee.com/uploads/images/2019/0619/184340_b64b6b8d_1355277.png "Screen Shot 2019-06-19 at 6.42.08 PM.png")

当在`test`方法中执行`self.block();`时，`weakSelf = nil`， `__block`变量对`ZNPerson`对象的强引用失效。那就不会构成循环引用，`ZNPerson`就能正常释放。

![](https://images.gitee.com/uploads/images/2019/0619/184324_d67fc97f_1355277.png "Screen Shot 2019-06-19 at 6.42.15 PM.png")


**总结**

- 用`__weak`修饰对象变量被释放后会自动将对象变量的值置为`nil`。

- 用`__unsafe_unretained `修饰的对象被释放后，对象所指向的内存被回收，但对象的值不变。这样是不安全的，容易造成野指针访问。

- 用`__block`解决循环引用，必须要调用`block`且在`block`内部将对象置为`nil`。


<br>

**参考文献：**

- 《Objective-C高级编程iOS与OS+X多线程和内存管理》

- 视频

<br>