
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


### 0x01 准备工作

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

- 找到基地址

```
(lldb) image list -o -f

# 0x00000000000e4000就是要找的基地址
[  0] 0x00000000000e4000 /var/containers/Bundle/Application/FC7B787B-97A2-4BDD-A9BE-EDE2C00EBA7A/AntiBlock.app/AntiBlock(0x00000001000e4000)
[  1] 0x0000000100140000 /Users/chuangqi/Library/Developer/Xcode/iOS DeviceSupport/10.1.1 (14B100)/Symbols/usr/lib/dyld
[  2] 0x0000000100100000 /Library/MobileSubstrate/MobileSubstrate.dylib(0x0000000100100000)
...
```

- 查看偏移地址

将上述工程安装到手机，并把其`Mach-O`文件放到`IDA`中，找到`-[ViewController touchesBegan:withEvent:]`



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