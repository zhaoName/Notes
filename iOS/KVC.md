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

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/100921_07ac55a0_1355277.png "Snip20190418_16.png")

- 先通过`key`顺序查找`setKey:`和`_setKey:`方法

优先查找`setKey:`方法

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/093745_8cd6025e_1355277.png "Snip20190418_13.png")

`setKey:`方法找不到会`_setKey:`方法

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/093430_e4f521bb_1355277.png "Snip20190417_11.png")


- 若`setKey:`和`_setKey:`都没找到,且`accessInstanceVariablesDirectly`返回为`NO`，则程序报`setValue:forUndefinedKey:`错误崩溃

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/093922_33ea157d_1355277.png "Snip20190417_12.png")


- 若`accessInstanceVariablesDirectly`返回为`YES`,则顺序查找`_key` `_isKey` `key` `isKey`四个成员变量


`_age`成员

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/093957_bdc20463_1355277.png "Snip20190417_2.png")


`_isAge`成员

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/094144_232e62a3_1355277.png "Snip20190417_3.png")


`age`成员

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/094221_af1fea62_1355277.png "Snip20190417_4.png")

`isAge`成员

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/094302_c51e302e_1355277.png "Snip20190417_6.png")

- 若四个成员变量都没找到，则程序报`setValue:forUndefinedKey:`错误崩溃

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/094325_2e4cf8cf_1355277.png "Snip20190417_7.png")


## 三、KVC取值原理


`valueForKey:`原理

![输入图片说明](https://images.gitee.com/uploads/images/2019/0418/101613_6e09a744_1355277.png "Snip20190418_17.png")


- 优先按照`getKey` `key` `isKey` `_key`顺序查找方法，设置新值

- 若都没找到会查看`accessInstanceVariablesDirectly`的返回值，为`NO`则直接调用`valueForUndefinedKey:`方法抛出异常

- 若`accessInstanceVariablesDirectly`的返回值为`YES`,则按顺序查找`_key` `_isKey` `key` `isKey`成员变量，找到直接取值

- 若四个成员变量都没找到则调用`valueForUndefinedKey:`方法抛出异常

<br>
写于2019-04-17
<br>