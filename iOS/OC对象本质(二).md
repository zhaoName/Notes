# OC对象本质(二)

<br>

## 一、OC对象的分类

### 0x01 实例(instance)对象

- `instance`对象是类`alloc`出来的对象，每次`alloc`都会产生新的对象。

- `instance`对象在内存中存储的信息包括：`isa`、其他成员变量

- `instance`对象中存储的是成员变量的值

### 0x02 类(class)对象

- 每个类在内存中只有一个`class`对象

```
NSObject *obj1 = [[NSObject alloc] init];
NSObject *obj2 = [[NSObject alloc] init];
    
// 三种方法获取一个类的类对象
Class obj1Class = [obj1 class];
Class obj2Class = [obj2 class];

Class obj4Class = object_getClass(obj1);
Class obj3Class = object_getClass(obj1);

Class objClass = [NSObject class];
    
NSLog(@"%p, %p, %p, %p, %p", obj1Class, obj2Class, obj3Class, obj4Class, objClass);

// 打印出的内存地址都是一个，所以类对象只有一个
2019-04-22 22:03:30.180237+0800 ZZNSObject[9395:593736] 0x7fffa33ec140, 0x7fffa33ec140, 0x7fffa33ec140, 0x7fffa33ec140, 0x7fffa33ec140
```

- `class`对象在内存中包含的信息

	- `isa`指针

	- `spuerclass`指针

	- 类的属性列表(`@property`)

	- 类的实例方法列表(`instace method`)

	- 类的协议信息(`protocol`)

	- 类的成员变量信息(`ivar`) 等等

### 0x03 元类(meta-class)对象

- 每个类在内存中只有一个`meta-class`对象

```
// 获取元类对象
// 将NSObject类对象传进去，元类对象只能用object_getClass获取
Class metaClass1 = object_getClass([NSObject class]);
Class metaClass2 = object_getClass([NSObject class]);
NSLog(@"元类对象:%p, %p", metaClass1, metaClass2);

// 不能用如下方法获取元类对象
Class cls = [[obj1 class] class];
NSLog(@"类对象:%p %p", obj1Class, cls);

2019-04-22 22:14:05.559608+0800 ZZNSObject[9525:655133] 元类对象:0x7fffa33ec0f0, 0x7fffa33ec0f0
2019-04-22 22:22:32.687416+0800 ZZNSObject[9635:704199] 类对象:0x7fffa33ec140 0x7fffa33ec140
```

- `meta-class`对象和`class`对象的内存结构一样，但用途不同。在内存中存储的信息包括

	- `isa`指针

	- `spuerclass`指针

	- 类的类方法(`class method`) 等等

	
## 二、isa、superclass


### 0x01 isa指针作用

`OC`调用的方法仅限于本类时

![输入图片说明](https://gitee.com/uploads/images/2019/0424/100403_2e6f721f_1355277.png "Snip20190424_1.png")

- `instance`的`isa`指向`class`对象，`class `对象的`isa`指向`meta-class`对象

- 调用对象方法时，先通过`instance`的`isa`找到`class`对象，然后找到存储在`class`对象中的对象方法进行调用；

- 调用类方法时，先通过`class `的`isa`找到`meta-class`对象，然后找到存储在`meta-class`对象中的类方法进行调用。


### 0x02 superclass作用

`OC`调用父类方法时

![输入图片说明](https://gitee.com/uploads/images/2019/0424/102107_6fa60ad9_1355277.png "Snip20190424_6.png")

- 调用父类的对象方法时，先通过`Student`的`instance`对象的`isa`找到`Student`的`class`对象，再通过`Student`的`class`对象的`superclass`找到父类`Person`的`class`对象，调用对象方法。以此类推。

![输入图片说明](https://gitee.com/uploads/images/2019/0424/101246_e3a1b5f2_1355277.png "Snip20190424_5.png")

- 调用父类的类方法时，先通过`Student`的`class`对象的`isa`找到`Student`的`meta-class`对象，再通过`Student`的`meta-class`的`superclass`找到父类`Person`的`meta-class`对象，调用类方法。以此类推。


### 0x03 总结

上一张网上流传的很经典的图

![输入图片说明](https://gitee.com/uploads/images/2019/0424/100531_bf01a068_1355277.png "class.png")

- `instance`对象的`isa`指向`class`对象，`class `对象的`isa`指向`meta-class`对象，`meta-class`对象的`isa`指向基类的`meta-class`，基类的`meta-class`对象的`isa`指向它自己

- `class`对象的`superclass`指向父类的`class`对象，若父类不存在则`superclass`为`nil`

- `meta-class`对象的`superclass`指向父类的`meta-class`对象，基类的`meta-class`对象的`superclass`指向基类的`class`对象


## 三、证明

声明两个类`ZZStudent`和`ZZPerson`，`ZZStudent`继承自`ZZPerson`

```
@interface ZZPerson : NSObject
+ (void)test;
@end
@implementation ZZPerson
@end


@interface ZZStudent : ZZPerson
@end
@implementation ZZStudent
@end
```

### 0x01 证明`instance`对象的`isa`指向`class`对象

打印`ZZPerson`的实例对象、类对象、元类对象

```
ZZPerson *p = [[ZZPerson alloc] init];
Class pClass = object_getClass(p);
Class pMetaClass = object_getClass(pClass);
    
NSLog(@"instance:%p class:%p meta-class:%p", p, pClass, pMetaClass);

// 打印信息
2019-04-24 10:57:47.998584+0800 ZZNSObject[5715:921429] instance:0x10280e260 class:0x1000023c0 meta-class:0x100002398
```

在`NSLog`处下断点

```
(lldb) p/x (long)p->isa
(long) $0 = 0x001d8001000023c1
```
可以看到`ZZPerson`的实例对象的`isa`指针`0x001d8001000023c1`并没有指向类对象`0x1000023c0 `，这是为啥呢？

这是因为苹果从`64bit`开始，`isa`要经过一次位运算才能得到真实地址

```
// objc4源码 isa.h

# if __arm64__
# define ISA_MASK   0x0000000ffffffff8ULL
	...
# elif __x86_64__
# define ISA_MASK   0x00007ffffffffff8ULL
	...
# endif
```

将`p->isa`经过位运算,得到的值刚好和类对象`0x1000023c0`一致

```
(lldb) p/x 0x00007ffffffffff8 & 0x001d8001000023c1
(long) $1 = 0x00000001000023c0
```

### 0x02 证明`class `对象的`isa`指向`meta-class`对象

`ZZPerson`继承自`NSObject`,`NSObject`有`isa`成员，所以能直接打印实例对象的`isa`指针。但`Class`类型的类对象和元类对象是不能直接打印`isa`指针，需要取巧强制转换下

`class `、`meta-class`对象的本质结构都是`struct objc_class`

```
// 自己构造一个和 struct objc_class 类似的结构体
struct zz_objc_class {
    Class isa;
    Class superclass;
};

// 将 Class 类型的对象强转成 struct zz_objc_class 类型的结构体
ZZPerson *p = [[ZZPerson alloc] init];
struct zz_objc_class *perClass = (__bridge struct zz_objc_class *)(object_getClass(p));
struct zz_objc_class *pMetaClass = (__bridge struct zz_objc_class *)object_getClass([ZZPerson class]);

// 进入断点状态 打印地址
(lldb) p/x perClass->isa
(Class) $0 = 0x001d800100002399
(lldb) p/x pMetaClass
(zz_objc_class *) $1 = 0x0000000100002398
(lldb) p/x pMetaClass

// 类对象的isa指向元类对象
(lldb) p/x 0x001d800100002399 & 0x00007ffffffffff8
(long) $3 = 0x0000000100002398
```

### 0x03 证明`meta-class`对象的`isa`指向基类的`meta-class`，基类的`meta-class`对象的`isa`指向它自己

```
ZZPerson *p = [[ZZPerson alloc] init];
NSObject *obj = [[NSObject alloc] init];

struct zz_objc_class *pMetaClass = (__bridge struct zz_objc_class *)object_getClass([ZZPerson class]);
struct zz_objc_class *objMetaClass = (__bridge struct zz_objc_class *)object_getClass([NSObject class]);

// meta-class 对象的 isa 指向基类的 meta-class
(lldb) p/x pMetaClass->isa
(Class) $0 = 0x001dffffa45fe0f1
(lldb) p/x objMetaClass
(zz_objc_class *) $3 = 0x00007fffa45fe0f0
(lldb) p/x 0x001dffffa45fe0f1 & 0x00007ffffffffff8
(long) $2 = 0x00007fffa45fe0f0

// 基类的 meta-class 对象的 isa 指向它自己
(lldb) p/x objMetaClass
(zz_objc_class *) $3 = 0x00007fffa45fe0f0
(lldb) p/x objMetaClass->isa
(Class) $1 = 0x001dffffa45fe0f1
(lldb) p/x 0x001dffffa45fe0f1 & 0x00007ffffffffff8
(long) $2 = 0x00007fffa45fe0f0
```

### 0x04 证明`class`对象的`superclass`指向父类的`class`对象，若父类不存在则`superclass`为`nil`

```
// 强转
struct zz_objc_class *stuClass = (__bridge struct zz_objc_class *)([ZZStudent class]);
struct zz_objc_class *perClass = (__bridge struct zz_objc_class *)([ZZPerson class]);
struct zz_objc_class *objClass = (__bridge struct zz_objc_class *)([NSObject class]);

NSLog(@"stuClass:%p claperClassss:%p objClass:%p", stuClass, perClass, objClass);
// 打印
2019-04-24 14:11:27.911900+0800 ZZNSObject[1000:32800] stuClass:0x100002418 claperClassss:0x1000023c8 objClass:0x7fff9b046140

(lldb) p/x stuClass->superclass
(Class) $0 = 0x00000001000023c8 ZZPerson
(lldb) p/x perClass->superclass
(Class) $1 = 0x00007fff9b046140 NSObject
(lldb) p/x objClass->superclass
(Class) $2 = nil
```

### 0x05 证明`meta-class`对象的`superclass`指向父类的`meta-class`对象，基类的`meta-class`对象的`superclass`指向基类的`class`对象


```
// 强转
struct zz_objc_class *objClass = (__bridge struct zz_objc_class *)([NSObject class]);
struct zz_objc_class *stuMetaClass = (__bridge struct zz_objc_class *)(object_getClass([ZZStudent class]));
struct zz_objc_class *perMetaClass = (__bridge struct zz_objc_class *)object_getClass([ZZPerson class]);
struct zz_objc_class *objMetaClass = (__bridge struct zz_objc_class *)object_getClass([NSObject class]);

// 打印
2019-04-24 14:25:53.071819+0800 ZZNSObject[1044:39229] objClass:0x7fff9b046140, stuMetaClass:0x1000023f0 perMetaClass:0x1000023a0 objMetaClass:0x7fff9b0460f0

(lldb) p/x stuMetaClass->superclass
(Class) $0 = 0x00000001000023a0
(lldb) p/x perMetaClass->superclass
(Class) $1 = 0x00007fff9b0460f0
(lldb) p/x objMetaClass->superclass
(Class) $2 = 0x00007fff9b046140 NSObject
```

这里还有个比较有意思的事...

新建`NSObject`的分类`NSObject+Add`

```
// NSObject+Add.h
- (void)test;

// NSObject+Add.m
- (void)test
{
    NSLog(@"-[NSObject test]===%p", self);
}

// 调用父类的类方法
NSLog(@"%p", [ZZPerson class]);
[ZZPerson test];
```

这样能调用成功吗？按照以前认知`ZZPerson`中只有`test`类方法的声明，没有`test`类方法的实现，`[ZZPerson test]`会导致程序崩溃！

运行程序

```
2019-04-24 14:43:22.845794+0800 ZZNSObject[1135:47507] 0x1000023b0
2019-04-24 14:43:22.846132+0800 ZZNSObject[1135:47507] -[NSObject test]===0x1000023b0
```

可以看到`ZZPerson`的类对象调用了`NSObject+Add`中的实例方法`test`。是不是很奇怪。。。其实这可以用上述学的知识解释。

- `OC`调用方法在底层会转成`obj_msgSend()`函数，`obj_msgSend()`函数中是没有指出调用的是实例方法还是类方法

- 当`ZZPerson`调用类方法时，先根据`ZZPerson`的`class`对象的`isa`指针找到`meta-class`对象，发现`meta-class`对象中没有存储`test`类方法；再根据`meta-class`对象的`superclass`找到父类`NSObject`的`meta-class`对象，发现也没有存储`test`类方法；最后由`NSObject`的`meta-class`对象的`superclass`找到`NSObject`的`class`对象，发现`class`对象中存储着实例方法`test`。调用成功。

语言总是苍白无力，解释起来有点绕人，上图

![输入图片说明](https://gitee.com/uploads/images/2019/0424/155049_16c5777f_1355277.png "Snip20190424_2.png")


**相关内容**

- [OC对象本质(一)](https://gitee.com/zhaoName0x01/Notes/blob/master/iOS/OC对象本质(一).md)

- [OC对象本质(三)](https://gitee.com/zhaoName0x01/Notes/blob/master/iOS/OC对象本质(三).md)


<br>

写于2019-04-22

<br>
