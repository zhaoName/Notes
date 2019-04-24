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



- `instance`的`isa`指向`class`对象，`class `的`isa`指向`meta-class`对象

- 调用对象方法时，先通过`instance`的`isa`找到`class`对象，然后找到存储在`class`对象中的对象方法进行调用；

- 调用类方法时，先通过`class `的`isa`找到`meta-class`对象，然后找到存储在`meta-class`对象中的类方法进行调用。


### 0x02 superclass作用

`OC`调用父类方法时

- 调用父类的对象方法时，先通过`Student`的`instance`对象的`isa`找到`Student`的`class`对象，再通过`Student`的`superclass`找到父类`Person`的`class`对象，调用对象方法。

- 调用父类的类方法时，先通过`Student`的`class`对象的`isa`找到`Student`的`meta-class`对象，再通过`Student`的`superclass`找到父类`Person`的`meta-class`对象，调用类方法。


### 0x03 总结

- `instance`对象的`isa`指向`class`对象，`class `对象的`isa`指向`meta-class`对象，`meta-class`对象的`isa`指向基类的`meta-class`，基类的`meta-class`对象的`isa`指向它自己

- `class`对象的`superclass`指向父类的`class`对象，若父类不存在则`superclass`为`nil`

- `meta-class`对象的`superclass`指向父类的`meta-class`对象，基类的`meta-class`对象的`superclass`指向`class`对象


## 三、证明




<br>

写于2019-04-22

<br>
