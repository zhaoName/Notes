
# 逆向中获取Block的参数和返回值

<br>

 **本文测试所用设备: iPhone6, 10.2越狱** 

当我们逆向某个App的功能时，可以先用`Reveal`查看试图层级关系定位到相关的类，然后结合`class-dump`出的头文件就能大概猜出来某个类的功能或类中的某个方法的作用。再编写`Tweak`代码查看返回值和参数，确定其功能。但是有个例外`block`,`calss-dump`出的头文件是看不出`block`的声明。如下

```
- (void)getParam:(CDUnknownBlockType)arg1
```

这样也就看不到`block`有几个参数，有无返回值。在`hook`或`lldb`调试过程中都会很难受。这篇文章会从两个角度来分析获取`blcok`的方法签名。

<br>

用`Xcode`新建一个叫`AntiBlock`的工程，`ViewController.m`代码如下


```
- (void)getParam:(void(^)(int age, NSDictionary *dict, Person *per))block
{
    Person *per = [[Person alloc] init];
    per.name = @"zhaoName";
    block(12, @{@"key":@"value"}, per);
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self getParam:^(int age, NSDictionary *dict, Person *per) {

        NSLog(@"block:%d, %@, %@", age, dict, per.name);
    }];
}
```

## 一、 汇编


### 0x01 Block的内存结构

在`LLVM`[官方文档](http://clang.llvm.org/docs/Block-ABI-Apple.html)中可以查看`Block`的内存结构

```
struct Block_literal_1 {
    void *isa; // initialized to &_NSConcreteStackBlock or &_NSConcreteGlobalBlock
    int flags;
    int reserved;
    void (*invoke)(void *, ...);
    struct Block_descriptor_1 {
    	unsigned long int reserved;         // NULL
        unsigned long int size;         // sizeof(struct Block_literal_1)
        // optional helper functions
        void (*copy_helper)(void *dst, void *src);     // IFF (1<<25)
        void (*dispose_helper)(void *src);             // IFF (1<<25)
        // required ABI.2010.3.16
        const char *signature;                         // IFF (1<<30)
    } *descriptor;
    // imported variables
};
```

我们重点要关注的就是`void (*invode)(void *, ...);`和`descriptor`中的`const char *signature`，前者指向了 Block 具体实现的地址，后者是表示 Block 函数签名的字符串。


### 0x02 准备工作

- `usbmuxd`开启端口映射

```
$ python tcprelay.py -t 22:2222 1234:1234

Forwarding local port 2222 to remote port 22
Forwarding local port 1234 to remote port 1234
......
```

- `ssh`连接到设备并开启`debugserver`

```
$ ssh root@localhost -p 2222

$ debugserver *:1234 -a AntiBlock
debugserver-@(#)PROGRAM:debugserver  PROJECT:debugserver-360.0.26.1
 for arm64.
Attaching to process AntiBlock...
Listening to port 1234 for a connection from *...
```

- 开启`lldb`，进入动态调试状态

```
$ lldb

(lldb) process connect connect://localhost:1234
Process 853 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = signal SIGSTOP
    frame #0: 0x00000001928fc16c libsystem_kernel.dylib`mach_msg_trap + 8
...
```

### 0x03 下断点

- 找到基地址

```
(lldb) image list -o -f

# 0x00000000000e4000就是要找的基地址
[  0] 0x00000000000e4000 /var/containers/Bundle/Application/FC7B787B-97A2-4BDD-A9BE-EDE2C00EBA7A/AntiBlock.app/AntiBlock(0x00000001000e4000)
[  1] 0x0000000100140000 /Users/chuangqi/Library/Developer/Xcode/iOS DeviceSupport/10.1.1 (14B100)/Symbols/usr/lib/dyld
[  2] 0x0000000100100000 /Library/MobileSubstrate/MobileSubstrate.dylib(0x0000000100100000)
...
```

- 将编译好的`AntiBlock`的`Mach-O`文件放到`IDA`中，找到`-[ViewController touchesBegan:withEvent:]`的偏移地址

```
text:0000000100007ADC ; Attributes: bp-based frame
__text:0000000100007ADC
__text:0000000100007ADC ; void __cdecl -[ViewController touchesBegan:withEvent:](ViewController *self, SEL, id, id)
__text:0000000100007ADC __ViewController_touchesBegan_withEvent__
__text:0000000100007ADC                                         ; DATA XREF: __objc_const:000000010000C6F8↓o
__text:0000000100007ADC
__text:0000000100007ADC var_28          = -0x28
__text:0000000100007ADC var_20          = -0x20
__text:0000000100007ADC var_18          = -0x18
__text:0000000100007ADC var_10          = -0x10
__text:0000000100007ADC var_8           = -8
__text:0000000100007ADC var_s0          =  0
__text:0000000100007ADC
...
__text:0000000100007B28                 ADRP            X8, #___block_literal_global@PAGE
__text:0000000100007B2C                 ADD             X8, X8, #___block_literal_global@PAGEOFF
__text:0000000100007B30                 ADRP            X9, #selRef_getParam_@PAGE
__text:0000000100007B34                 ADD             X9, X9, #selRef_getParam_@PAGEOFF
# 我们都知道OC中 方法调用的第一个参数是类(对象)本身，第二个参数是方法名 接下来才是真正的参数，
# 下面三句汇编就是在为调方法（_objc_msgSend）做准备 - 将参数放到对应的寄存器中
__text:0000000100007B38                 LDUR            X0, [X29,#var_8]
__text:0000000100007B3C                 LDR             X1, [X9] ; "getParam:"
__text:0000000100007B40                 MOV             X2, X8
__text:0000000100007B44                 BL              _objc_msgSend
__text:0000000100007B48                 MOV             X8, #0
...
__text:0000000100007B78                 RET
__text:0000000100007B78 ; End of function -[ViewController touchesBegan:withEvent:]
```

- 下断点

```
(lldb) br s -a 0x00000000000e4000+0x0000000100007ADC
Breakpoint 1: where = AntiBlock`-[ViewController touchesBegan:withEvent:] + 96 at ViewController.m:121, address = 0x00000001000ebb3c
```

- 然后终端中输入`(lldb) c`让程序跑起来，点击手机屏幕触发断点。

```
(lldb) Process 853 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x00000001000ebb3c AntiBlock`-[ViewController touchesBegan:withEvent:](self=0x00000001002122e0, _cmd="touchesBegan:withEvent:", touches=1 element, event=0x00000001740ed480) at ViewController.m:121
   118 	- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
   119 	{
   120 	    
-> 121 	    [self getParam:^(int age, NSDictionary *dict, Person *per) {
   122 	
   123 	        NSLog(@"block:%d, %@, %@", age, dict, per.name);
   124 	    }];
Target 0: (AntiBlock) stopped.
```

- 一直执行`ni`直到`__text:0000000100007B44 BL _objc_msgSend`,到这说明将要调用`getParam:`方法。

```
(lldb) ni
Process 853 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = instruction step over
    frame #0: 0x00000001000ebb44 AntiBlock`-[ViewController touchesBegan:withEvent:](self=0x00000001002122e0, _cmd="touchesBegan:withEvent:", touches=1 element, event=0x00000001740ed480) at ViewController.m:121
Target 0: (AntiBlock) stopped.

(lldb) po $x0
<ViewController: 0x1002122e0>

(lldb) po (char *)$x1
getParam:

(lldb) po (char *)$x2
<__NSGlobalBlock__: 0x1000f00c8>
```

可以看到第二个参数就是我们要找的`Block`，但只打印出`Block`指针的地址。接下来，就靠我们自己来找出它的函数体地址和函数签名了。


### 0x04 找出 Block 的函数体地址

从`Block`的内存结构可以看出，我们要找到`invoke`这个函数的指针地址，它指向这个`Block`的函数实现。在`arm64`架构中，指针占用8个字节，`int`类型占用4个字节。所以`invoke`函数的指针地址在第16个字节后，占用8个字节

```
(lldb) memory read --size 8 --format x 0x1000f00c8
0x1000f00c8: 0x00000001b8a01698 0x0000000050000000
0x1000f00d8: 0x00000001000ebb7c 0x00000001000f00a8
0x1000f00e8: 0x00000001b8a08778 0x00000000000007c8
0x1000f00f8: 0x00000001000ef08d 0x0000000000000002
```

可以看到在第16个字节后，占用8个字节的地址为`0x00000001000ebb7c`。然后你可以对这个地址反汇编:`disassemble --start-address 0x00000001000dd770`， 或者给这个内存地址下断点`br s -a 0x00000001000dd770`，以便进入`Block`函数体内。但是，大多数情况下，我们并不需要进到 Block 函数体内。在写 tweak 的时候，我们更需要的是知道这个 Block 回调给了我们哪些参数。

### 0x05 Block 的方法签名


`Block`的方法签名需要通过`descriptor`结构体中的`signature`成员，然后通过它得到一个`NSMethodSignature`对象。由`Block`的内存结构可知，`descriptor`在`invoke`后面的8个字节即`0x00000001000f00a8`。


文档中指出不是每一个`Block`都有其方法签名，我们需要通过 flags 与 block 中定义的枚举掩码进行与判断。还是在刚刚的 llvm 文档中，我们可以看到掩码的定义如下：

```
enum {
    BLOCK_HAS_COPY_DISPOSE =  (1 << 25),
    BLOCK_HAS_CTOR =          (1 << 26), // helpers have C++ code
    BLOCK_IS_GLOBAL =         (1 << 28),
    BLOCK_HAS_STRET =         (1 << 29), // IFF BLOCK_HAS_SIGNATURE
    BLOCK_HAS_SIGNATURE =     (1 << 30),
};
```

再次使用`memory`命令打印出`flags`的值：

```
(lldb) memory read --size 4 --format x 0x1000f00c8
0x16fd8a958: 0xb8a01698 0x00000001 0x00000000 0x00000005 
0x16fd8a968: 0x000ebb7c 0x00000001 0x000f00a8 0x00000001

(lldb) expr -- ((0x00000000 & (1 << 30)) != 0)
(bool) $4 = true

(lldb) expr -- ((0x000000005 & (1 << 30)) != 0)
(bool) $5 = false
```
由于`expr -- ((0x00000000 & (1 << 30)) != 0)`我们可以确定这个`Block`是有签名的。


为了找出`signature`的地址，我们还需要确认这个`Block`是否拥有`copy_helper`和`disponse_helper`这两个可选的函数指针。由于`expr -- ((0x000000005 & (1 << 30)) == 0)`，因此我们可以确认这个 Block 没有刚刚提到的两个函数指针。

所以`signature`的地址在`descriptor`下偏移两个`unsiged long`即16个字节后的地址后，占用8个字节。查看`descriptor`的内存

```
(lldb) memory read --size 8 --format x 0x00000001000f00a8
0x1000f00a8: 0x0000000000000000 0x0000000000000020
0x1000f00b8: 0x00000001000ef142 0x0000000000000000
0x1000f00c8: 0x00000001b8a01698 0x0000000050000000
0x1000f00d8: 0x00000001000ebb7c 0x00000001000f00a8

(lldb) p (char *)0x00000001000ef142
(char *) $7 = 0x00000001000ef142 "v28@?0i8@"NSDictionary"12@"Person"20"
```


`"v28@?0i8@"NSDictionary"12@"Person"20"`就是我们要找的函数签名，是个`NSMethodSignature`类型。


```
# 双引号注意转义
(lldb) po [NSMethodSignature signatureWithObjCTypes:"v28@?0i8@\"NSDictionary\"12@\"Person\"20"]
<NSMethodSignature: 0x17007e5c0>
    number of arguments = 4
    frame size = 224
    is special struct return? NO
    return value: -------- -------- -------- --------
        type encoding (v) 'v'
        flags {}
        modifiers {}
        frame {offset = 0, offset adjust = 0, size = 0, size adjust = 0}
        memory {offset = 0, size = 0}
    argument 0: -------- -------- -------- --------
        type encoding (@) '@?'
        flags {isObject, isBlock}
        modifiers {}
        frame {offset = 0, offset adjust = 0, size = 8, size adjust = 0}
        memory {offset = 0, size = 8}
    argument 1: -------- -------- -------- --------
        type encoding (i) 'i'
        flags {isSigned}
        modifiers {}
        frame {offset = 8, offset adjust = 0, size = 8, size adjust = -4}
        memory {offset = 0, size = 4}
    argument 2: -------- -------- -------- --------
        type encoding (@) '@"NSDictionary"'
        flags {isObject}
        modifiers {}
        frame {offset = 16, offset adjust = 0, size = 8, size adjust = 0}
        memory {offset = 0, size = 8}
            class 'NSDictionary'
    argument 3: -------- -------- -------- --------
        type encoding (@) '@"Person"'
        flags {isObject}
        modifiers {}
        frame {offset = 24, offset adjust = 0, size = 8, size adjust = 0}
        memory {offset = 0, size = 8}
            class 'Person'
```

对我们最有用的`type encoding`字段，这些符号对应的解释可以参考[Type Encoding官方文档](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtTypeEncodings.html)

从`Type Encoding`官方文档可以看出，这个`Block`没有返回值，第一个参数是`block`(即本身的引用)，第二个参数是`int`类型，第三个参数是`NSDictionary`类型，第四个参数是自定义的`Person`类型。

所以最终得到的完整函数和自己写出函数一样

```
- (void)getParam:(void(^)(int age, NSDictionary *dict, Person *per))block
```

### 0x06 通过hook得到block的参数具体内容

在实际破解中,我们需要手动实现一个一样结构的`block`来调用，以达到查看具体参数内容的目的

```
%hook ViewController

- (void)getParam:(CDUnknownBlockType)arg1
{
    void(^hookBlock)(int a, NSDictionary *dict, Person *p) = ^(int a, NSDictionary *dict, Person *p) {
        NSLog(@"hook======block:%d, %@, %@", a, dict, p.name);
    };
    
    %orig(hookBlock);
}

%end
```


## 二、Frida


<br>
<br>



