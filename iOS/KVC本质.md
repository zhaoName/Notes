# KVC

<br>

`KVC`全称`Key-Value Coding`,也称键值编码。可以通过一个`key`来设置或访问某个属性值


## 一、基本使用

### 0x01 常用API

```
- (id)valueForKey:(NSString *)key;
- (void)setValue:(id)value forKey:(NSString *)key;

- (id)valueForKeyPath:(NSString *)keyPath;
- (void)setValue:(id)value forKeyPath:(NSString *)keyPath;
```

### 0x02 key和keyPath区别

```
// Person.h

@interface Student : NSObject
@property (nonatomic, assign) NSInteger num;/**< */
@end

// Person有个Student属性
@interface Person : NSObject
@property (nonatomic, assign) NSInteger age;
@property (nonatomic, strong) Student *stu;/**< */
@end


// kvc设置age
[self.person1 setValue:@100 forKey:@"age"];
// kvc设置
[self.person1 setValue:@123456 forKeyPath:@"stu.num"];

NSLog(@"KVC改变属性值:%@ %@", [self.person1 valueForKey:@"age"], [self.person1 valueForKeyPath:@"stu.num"]);

// 打印信息
2019-04-17 23:08:00.993126+0800 KVC[41445:1582018] KVC改变属性值:100 123456
```


## 二、KCV设置原理

`setValue:forKey:`原理

![](../Images/KVC本质/kvc_image1.png)

- 先通过`key`顺序查找`setKey:`和`_setKey:`方法

优先查找`setKey:`方法

![](../Images/KVC本质/kvc_image2.png)

`setKey:`方法找不到会`_setKey:`方法

![](../Images/KVC本质/kvc_image3.png)


- 若`setKey:`和`_setKey:`都没找到,且`accessInstanceVariablesDirectly`返回为`NO`，则程序报`setValue:forUndefinedKey:`错误崩溃

![](../Images/KVC本质/kvc_image4.png)


- 若`accessInstanceVariablesDirectly`返回为`YES`,则顺序查找`_key` `_isKey` `key` `isKey`四个成员变量


`_age`成员

![](../Images/KVC本质/kvc_image5.png)


`_isAge`成员

![](../Images/KVC本质/kvc_image6.png)


`age`成员

![](../Images/KVC本质/kvc_image7.png)

`isAge`成员

![](../Images/KVC本质/kvc_image8.png)

- 若四个成员变量都没找到，则程序报`setValue:forUndefinedKey:`错误崩溃

![](../Images/KVC本质/kvc_image9.png)

## 三、KVC取值原理

`valueForKey:`原理

![](../Images/KVC本质/kvc_image10.png)


- 优先按照`getKey` `key` `isKey` `_key`顺序查找方法，设置新值

- 若都没找到会查看`accessInstanceVariablesDirectly`的返回值，为`NO`则直接调用`valueForUndefinedKey:`方法抛出异常

- 若`accessInstanceVariablesDirectly`的返回值为`YES`,则按顺序查找`_key` `_isKey` `key` `isKey`成员变量，找到直接取值

- 若四个成员变量都没找到则调用`valueForUndefinedKey:`方法抛出异常

## 四、面试题

### 0x01 通过KVC设置的属性值能触发KVO吗？

从`KVC`设值原理可以看到就算没有属性的`setKey:`方法，`KVC`也可能设值成功。那还能触发`KVO`吗？ 这里给予肯定答案：只要`KVC`的流程能走通(无论是调方法成功还是查找到四个成员变量中任意一个)且添加`KVO`监听，就能触发。

`KVC`触发`KVO`流程

- 调用`willChangeValueForKey:`

- 调`setKey: / _setKey`方法或查找到四个成员变量中任意一个，设置新的属性值

- 调用`didChangeValueForKey:`方法

- 在`didChangeValueForKey:`方法中触发监听器的`observeValueForKeyPath:ofObject:change:context:`方法


<br>
写于2019-04-17
<br>

