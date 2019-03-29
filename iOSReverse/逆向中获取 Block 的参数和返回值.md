
# 逆向中获取Block的参数和返回值


<br>

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

- `shh`连接到设备并开启`debugserver`

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

可以看到第二个参数就是我们要找的 Block，但只打印出他的地址。接下来，就靠我们自己来找出它的函数体地址和函数签名了。


### 0x04 找出 Block 的函数体地址




### 0x05 Block 的方法签名

```
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