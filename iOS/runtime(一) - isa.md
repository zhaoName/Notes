
# runtime(一) - isa


<br>

在[OC对象本质(二)](https://github.com/zhaoName/Notes/blob/master/iOS/OC%E5%AF%B9%E8%B1%A1%E6%9C%AC%E8%B4%A8(%E4%BA%8C).md)中说到在`arm64`架构中，实例对象的`isa`并没有直接指向类对象的地址，而是要经过一次位运算`& ISA_MASK `才能得到真实的类对象地址。

本篇文章介绍为什么要经过一次位运算才能得到真实的类对象地址？


## 一、位运算

### 0x01 简介

`OC`中的位运算和`C/C++`是一样的，常用的有：

- `&`：按位与，两个值都为1 结果为1

- `|`：按位或，有一个值1 结果为1

- `~`：按位取反 

- `^`：按位异或，相同为0 不同为1 

- `<<`：左位移，将操作数的二进制码整体左移指定位数，右边空出来的位数补0

- `>>`：右位移，将将操作数的二进制码整体右移指定位数，左边空出来的位按原来符号补充。即原来是正数补0，原来是负数补1。

这里特殊介绍下`~`按位取反。

所有的数字在计算机中都是以二进制形式存在的，原码是直接将一个数字换算成二进制的结果，但计算机中以补码形式保存所有整数。补码的计算规则：

- 正数的补码和原码完全相同；

- 负数的补码是其反码加1，反码是对原码按位取反 只是其最高位(符号位)不变。

```
int a = 2;
int b = ~a;
NSLog(@"%d", b);

// 打印结果
2019-06-23 15:32:00.187427+0800 runtime-isa[976:62360] -3
```

我们知道在`arm64`架构中整形数据占4个字节，将变量`a`转成二进制并取反

```
~ 00000000 00000000 00000000 00000010
  11111111 11111111 11111111 11111101
```

那`11111111 11111111 11111111 11111101`转成十进制是谁呢？我们看下-3在计算机中的补码是什么？

```
10000000 00000000 00000000 00000011  // -3的原码
11111111 11111111 11111111 11111100	 // -3的反码
11111111 11111111 11111111 11111101  // -3的补码 = -3的反码 + 1
```

可以看到`~2`的结果和`-3`在计算机中存储的值一样，所以`~2 = -3`

### 0x02 `OC`常用的位运算`|`

在`OC`常用的位运算符应该是在使用枚举类型中常用到的`|`。如`KVO`中监听某个属性值变化时

```
// NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld 可以知道新老属性值
[@"xxx" addObserver:self forKeyPath:@"xxx" options:NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld context:nil];

// NSKeyValueObservingOptionNew 只能知道属性的新值
[@"xxx" addObserver:self forKeyPath:@"xxx" options:NSKeyValueObservingOptionNew context:nil];
```

那它内部是怎么知道我设置了那些值？又是怎么取出对应的值呢？ 

我们自己来实现一个枚举类型。

```
typedef NS_OPTIONS(NSInteger, ZNOrientation) {
    
    ZNOrientationUnknow = 0,   // 0b0000
    ZNOrientationEast = 1<<0,  // 0b0001
    ZNOrientationSouth = 1<<1, // 0b0010
    ZNOrientationWest = 1<<2,  // 0b0100
    ZNOrientationNorth = 1<<3  // 0b1000
};

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self testOrienta:ZNOrientationEast | ZNOrientationNorth];
}

- (void)testOrienta:(ZNOrientation)ori
{
    
}
```

`ZNOrientationEast | ZNOrientationNorth` 的结果换成二进制结果为`0b0001 | 0b1000 = 0b1001`。那我们在`testOrienta `方法中怎么知道传进来的具体值是多少呢？

比如说我想知道传进来的值包不包含`ZNOrientationNorth `，那我们将结果`& ZNOrientationNorth `看看会有什么情况。

```
// 包含 其他位不管 &0 结果肯定为0
0b1000 & 0b1000 = 0b1000

// 不包含
0b0000 & 0b1000 = 0b0000
```

也就是说我们只要将结果`&`上某个枚举值就可以知道传进来的值，到底包不包含这个枚举值。

```
- (void)testOrienta:(ZNOrientation)ori
{
    if (ori & ZNOrientationEast) {
        NSLog(@"ori中包含ZNOrientationEast");
    }
    if (ori & ZNOrientationSouth) {
        NSLog(@"ori中包含ZNOrientationSouth");
    }
    if (ori & ZNOrientationWest) {
        NSLog(@"ori中包含ZNOrientationWest");
    }
    if (ori & ZNOrientationNorth) {
        NSLog(@"ori中包含ZNOrientationNorth");
    }
}

// 打印结果
2019-06-23 16:35:08.053900+0800 runtime-isa[1219:80499] ori中包含ZNOrientationEast
2019-06-23 16:35:08.054018+0800 runtime-isa[1219:80499] ori中包含ZNOrientationNorth
```

**总结：**

- 想要取某个(或几个)二进制位的值只要`& `上一个对应二进制位为1 其余二进制位为0的二进制数

```
// 取 0b10100011 第三位和第五位的值

  1010 0011
& 0010 1000
--------------
  0010 0000
```

- 想设置某个二进制位的值

	- 设置为1，只要`| `上一个对应二进制位为1 其余二进制位为0的二进制数

	```
	// 设置0b00000000 第三位和第五位的值为1
	   1000 0100
	 | 1010 1100
	 -------------
	   0010 1000
	```

	- 设置为0，只要`& `上一个对应二进制位为0 其余二进制位为1的二进制数
	
	```
	// 设置0b00101000 第三位和第五位的值位0
	   1010 1010
	 & 1101 0111
	 -------------
	   1000 0010
	```
	

## 二、共用体

共用体也称联合体，它使几种不同类型的变量存放到同一块内存中。你可以定义一个带有多成员的共用体，但是任何时候只能有一个成员带有值。

### 0x01 初始化
 

```
union ZNData {
    int year;
    short int month;
    long day;
};

union ZNData date = {10};
```

共用体的初始化，只需要一个初始化值就行了，其类型必须和共用体的第一个成员类型匹配。


### 0x02 大小

```
NSLog(@"sizeof：%ld", sizeof(date));

// 打印结果
2019-06-23 17:47:59.297924+0800 runtime-isa[1555:106198] sizeof：8
```

共用体的大小等于其内成员最大内存长度的大小。

### 0x03 修改值

```
union ZNData date = {10};
NSLog(@"初始化==year:%d, month:%d, day:%ld", date.year, date.month, date.day);
    
date.month = 15;
NSLog(@"改值后==year:%d, month:%d, day:%ld", date.year, date.month, date.day);

// 打印结果
2019-06-23 17:53:05.022790+0800 runtime-isa[1579:107767] 初始化==year:10, month:10, day:10
2019-06-23 17:53:05.022996+0800 runtime-isa[1579:107767] 改值后==year:15, month:15, day:15
```

## 三、位域


位域`(Bit field)`也称位段，是一种数据结构，可以把数据以位的形式紧凑的储存，并允许程序员对此结构的位进行操作。为什么会有这种结构呢？我们存储开关的状态时，只有开和关两种状态，用一个布尔类型的变量能办到。但这样会有点浪费空间，因为我们也可以用一个二进制位来表示。

### 0x01 声明

```
struct {
    char x1:2;
    char x2:2;
    char x3:2;
    char x4:2;
} test;
```

- 位域的声明和结构体类似，`:`后面的数字用来限定成员变量占用的位数。

- 成员变量都是有类型的，这个类型限制了成员变量的最大位宽，`:`后面的数字不能超过这个长度。

![](https://images.gitee.com/uploads/images/2019/0624/005530_d334ea16_1355277.png "runtime_image0101.png")

### 0x02 位域成员的类型

`C`语言标准规定，只有有限的几种数据类型可以用于位域。在`ANSIC `中，这几种数据类型是`int`和`unsigned int`， 到了`C99 ``_Bool`也被支持了。但编译器在具体实现时都进行了扩展，额外支持了`char`、`signed char`、`unsigned char`以及`enum`类型。

位域不能是静态类型。不能使用`&`对位域做取地址运算，因此不存在位域的指针。


### 0x03 存储

`C`语言标准并没有规定位域的具体存储方式，不同的编译器有不同的实现，但它们都尽量压缩存储空间。

- 先写的成员变量存储在最右边，然后依次往左排。

- 若位域结构所有成员的位宽之和 小于等于最大成员的类型长度时，位域结构的大小等于该类型长度。且每个成员都紧挨着存储。

```
struct {
    char  x1:2;
    short x2:4;
    char  x3:2;
    char  x4:4;
} test;
test.x1 = 0b01;
test.x2 = 0b0110;
test.x3 = 0b00;
test.x4 = 0b0111;
NSLog(@"========%d", sizeof(test));

// 打印结果
2019-06-23 22:19:43.110143+0800 runtime-isa[2496:173094] ========2
```

![](https://images.gitee.com/uploads/images/2019/0624/005620_871c733e_1355277.png "runtime_image0102.png")


- 若位域结构所有成员的位宽之和 大于最大成员的类型长度时，位域结构的大小等于该类型长度的倍数。

```
struct {
    char  x1:2;
    short x2:4;
    char  x3:8;
    char  x4:6;
} test;

test.x1 = 0b01;
test.x2 = 0b0011;
test.x3 = 0b00001111;
test.x4 = 0b010101;
NSLog(@"========%d", sizeof(test));

// 打印结果
2019-06-23 22:32:20.928969+0800 runtime-isa[2529:175243] ========4
```

`x1``x2``x3``x4`的位宽之和为`2+4+8+6 = 20`，大于`short`类型的长度16。所以在存储`x4`的时候会新开辟两个字节长度。 

![](https://images.gitee.com/uploads/images/2019/0624/005653_66361168_1355277.png "runtime_image0103.png")


### 0x04 使用

自定义一个`ZNGirl`类，它有三个`BOOL`类型的成员`white`、`rich`、`beautiful`。按照以前的做法是直接声明三个`BOOL`的属性变量，学过位域后我们可以尝试下。

```
// ZNGirl.h
- (void)setWhite:(BOOL)white;
- (void)setRich:(BOOL)rich;
- (void)setBeautiful:(BOOL)beautiful;

- (BOOL)white;
- (BOOL)rich;
- (BOOL)beautiful;

// ZNGirl.m
struct {
    char white : 1;
    char rich : 1;
    char beautiful :1;
}_whiteRichBeautiful;

- (void)setWhite:(BOOL)white
{
    _whiteRichBeautiful.white = white;
}

- (void)setRich:(BOOL)rich
{
    _whiteRichBeautiful.rich = rich;
}

- (void)setBeautiful:(BOOL)beautiful
{
    _whiteRichBeautiful.beautiful = beautiful;
}

- (BOOL)white
{
    return !!_whiteRichBeautiful.white;
}

- (BOOL)rich
{
    return !!_whiteRichBeautiful.rich;
}

- (BOOL)beautiful
{
    return _whiteRichBeautiful.beautiful;
}

// ViewController.m
ZNGirl *girl = [[ZNGirl alloc] init];
girl.white = YES;
girl.rich = NO;
girl.beautiful = YES;

NSLog(@"white:%d, rich:%d, beautiful:%d",  girl.white, girl.rich, girl.beautiful);

// 打印结果
2019-06-23 23:15:36.628183+0800 runtime-isa[2726:188584] white:1, rich:0, beautiful:1
```


可以看到用了一个字节长度的内存，存储了三个`BOOL`类型的成员变量。


### 0x05 共用体和位域结合

我们还可以用共用体和位域结合的方式解决上面的问题。

```
// ZNGirl.h
- (void)setWhite:(BOOL)white;
- (void)setRich:(BOOL)rich;
- (void)setBeautiful:(BOOL)beautiful;

- (BOOL)white;
- (BOOL)rich;
- (BOOL)beautiful;

// ZNGirl.m
#define ZN_WHITE_MASK (1 << 0)
#define ZN_RICH_MASK (1 << 1)
#define ZN_BEAUTIFUL_MASK (1 << 2)

union {
	char bits;
	// 这个结构体只是为了增加代码可读性，去掉后代码也能正常运行
	struct {
	    char white : 1;
	    char rich : 1;
	    char beautiful :1;
	    
	};
}_whiteRichBeautiful;

- (void)setWhite:(BOOL)white
{
    if (white) {
        _whiteRichBeautiful.bits |= ZN_WHITE_MASK;
    } else {
        _whiteRichBeautiful.bits &= ~ZN_WHITE_MASK;
    }
}

- (void)setRich:(BOOL)rich
{
    if (rich) {
        _whiteRichBeautiful.bits |= ZN_RICH_MASK;
    } else {
        _whiteRichBeautiful.bits &= ~ZN_RICH_MASK;
    }
}

- (void)setBeautiful:(BOOL)beautiful
{
    if (beautiful) {
        _whiteRichBeautiful.bits |= ZN_BEAUTIFUL_MASK;
    } else {
        _whiteRichBeautiful.bits &= ~ZN_BEAUTIFUL_MASK;
    }
}

- (BOOL)white
{
    return !!(_whiteRichBeautiful.bits & ZN_WHITE_MASK);
}

- (BOOL)rich
{
    return !!(_whiteRichBeautiful.bits & ZN_RICH_MASK);
}

- (BOOL)beautiful
{
    return !!(_whiteRichBeautiful.bits & ZN_BEAUTIFUL_MASK);
}

// ViewController.m
ZNGirl *girl = [[ZNGirl alloc] init];
girl.white = NO;
girl.rich = YES;
girl.beautiful = NO;

NSLog(@"white:%d, rich:%d, beautiful:%d",  girl.white, girl.rich, girl.beautiful);

// 打印结果
2019-06-23 23:58:24.426001+0800 runtime-isa[2903:200763] white:0, rich:1, beautiful:0
```


## 四、`isa`


在`arm64`架构之前，`isa`就是一个普通的指针，存储着`Class`、`Meta-Class`对象的内存地址。但从`arm64 `架构开始，苹果对`isa`进行了优化，变成了一个共用体（`union`）结构，还使用位域来存储更多的信息。

我们知道`OC`中的类本质上是个`objc_class`类型的结构体，`isa`指针存储在其中，但没有声明。`isa`声明在`objc_class `的父类`objc_object`结构体中。在`objc4-750`源码中搜索`objc_object {`就可查看`isa`指针的结构。

```
//  objc-private.h
struct objc_object {
private:
    isa_t isa;
...
}

union isa_t {
    isa_t() { }
    isa_t(uintptr_t value) : bits(value) { }

    Class cls;
    uintptr_t bits;
#if defined(ISA_BITFIELD)
    struct {
        ISA_BITFIELD;  // defined in isa.h
    };
#endif
};

// isa.h
# if __arm64__
#   define ISA_MASK        0x0000000ffffffff8ULL
#   define ISA_MAGIC_MASK  0x000003f000000001ULL
#   define ISA_MAGIC_VALUE 0x000001a000000001ULL
#   define ISA_BITFIELD                                                      \
      uintptr_t nonpointer        : 1;                                       \
      uintptr_t has_assoc         : 1;                                       \
      uintptr_t has_cxx_dtor      : 1;                                       \
      uintptr_t shiftcls          : 33; /*MACH_VM_MAX_ADDRESS 0x1000000000*/ \
      uintptr_t magic             : 6;                                       \
      uintptr_t weakly_referenced : 1;                                       \
      uintptr_t deallocating      : 1;                                       \
      uintptr_t has_sidetable_rc  : 1;                                       \
      uintptr_t extra_rc          : 19
#   define RC_ONE   (1ULL<<45)
#   define RC_HALF  (1ULL<<18)

# elif __x86_64__
#   define ISA_MASK        0x00007ffffffffff8ULL
#   define ISA_MAGIC_MASK  0x001f800000000001ULL
#   define ISA_MAGIC_VALUE 0x001d800000000001ULL
#   define ISA_BITFIELD                                                        \
      uintptr_t nonpointer        : 1;                                         \
      uintptr_t has_assoc         : 1;                                         \
      uintptr_t has_cxx_dtor      : 1;                                         \
      uintptr_t shiftcls          : 44; /*MACH_VM_MAX_ADDRESS 0x7fffffe00000*/ \
      uintptr_t magic             : 6;                                         \
      uintptr_t weakly_referenced : 1;                                         \
      uintptr_t deallocating      : 1;                                         \
      uintptr_t has_sidetable_rc  : 1;                                         \
      uintptr_t extra_rc          : 8
#   define RC_ONE   (1ULL<<56)
#   define RC_HALF  (1ULL<<7)

# else
#   error unknown architecture for packed isa
# endif
```

简化下：去掉`x86`架构、替换掉宏定义

```
typedef unsigned long  uintptr_t;

union isa_t {
    uintptr_t bits;
    struct {
      uintptr_t nonpointer        : 1;                                       
      uintptr_t has_assoc         : 1;                                       
      uintptr_t has_cxx_dtor      : 1;                                       
      uintptr_t shiftcls          : 33; 
      uintptr_t magic             : 6;                                       
      uintptr_t weakly_referenced : 1;                                       
      uintptr_t deallocating      : 1;                                       
      uintptr_t has_sidetable_rc  : 1;                                       
      uintptr_t extra_rc          : 19
    };
};
```

可以看到在`arm64`架构下`isa`实际是个`union isa_t `类型的共用体，占8个字节64位。

- `nonpointer `：0 代表普通的指针，存储着`Class`、`Meta-Class`对象的内存地址； 1 代表优化过，使用位域存储更多的信息

- `has_assoc `：是否有设置过关联对象，如果没有，释放时会更快

- `has_cxx_dtor `：是否有C++的析构函数（`.cxx_destruct`），如果没有，释放时会更快

- `shiftcls `：存储着`Class`、`Meta-Class`对象的内存地址信息

- `magic `：用于在调试时分辨对象是否未完成初始化

- `weakly_referenced `：是否有被弱引用指向过，如果没有，释放时会更快

- `deallocating `：对象是否正在释放

- `has_sidetable_rc `：里面存储的值是引用计数器减1

- `extra_rc `：引用计数器是否过大无法存储在`isa`中，如果为1，那么引用计数会存储在一个叫`SideTable`的类的属性中


`Class`或`Meta-Class`对象的内存地址存储在共用体的第3到第35位，共33位。想要取出这个值就必须`&` 上对应位为1 其余位为0 的二进制数。

将`ISA_MASK `换成二进制

```
0b 00000000 00000000 00000000 00001111 11111111 11111111 11111111 11111000
```

刚好第3到第35位都是1，其余位都是0。那`isa & ISA_MASK `就能取出`Class`对象或`Meta-Class`对象的内存地址。

`isa & ISA_MASK `得到的结果最后三位肯定是0，那`Class`对象或`Meta-Class`对象的内存地址换成16进制，要么以8结尾，要么以0结尾。


<br>

写于2019-06-23

<br>
