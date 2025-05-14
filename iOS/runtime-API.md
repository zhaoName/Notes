
# runtime(五) - API

<br>

## 一、类

- `object_getClass`

```Objective-C
ZNPerson *per = [[ZNPerson alloc] init];
NSLog(@"类对象：%p==%p，元类对象：%p", [ZNPerson class], object_getClass(per), object_getClass([NSObject class]));

// 打印结果
2019-07-08 11:14:04.572979 runtime-super[14284:1421422] 类对象：0x10009d098==0x10009d098，元类对象：0x1b3ee6ec8
```

- `object_isClass`、`class_isMetaClass`

```Objective-C
NSLog(@"是否是类对象:%d==%d, 是否是元类对象：%d",
          object_isClass(per),
          object_isClass(object_getClass(per)),
          class_isMetaClass(object_getClass([NSObject class])));

// 打印结果
2019-07-08 11:14:04.573401 runtime-super[14284:1421422] 是否是类对象:0==1, 是否是元类对象：1
```

- `object_setClass`

```Objective-C
// 将实例对象 per 的 isa 指向 ZNStudent
object_setClass(per, [ZNStudent class]);
// ZNStudent 必须要有和 ZNPerson 同名的方法
[per testPerson];
 
// 打印结果
2019-07-08 11:23:44.074476 runtime-super[14322:1423939] -[ZNStudent testPerson]
```

- 动态创建类

```Objective-C
- (void)testAddMethod
{
    NSLog(@"%@==%@", self, NSStringFromSelector(_cmd));
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 动态创建类 参数:父类 类名 额外内存空间(常写0)
    Class ZNTeacher = objc_allocateClassPair([NSObject class], "ZNTeacher", 0);
    
    // 动态添加成员变量
    // This function may only be called after objc_allocateClassPair and before objc_registerClassPair.
    // Adding an instance variable to an existing class is not supported
    class_addIvar(ZNTeacher, "_age", 4, 1, @encode(int));
    class_addIvar(ZNTeacher, "_teach", 8, 1, @encode(NSString *));
    
    // 动态添方法
    class_addMethod(ZNTeacher, @selector(testAddMethod), class_getMethodImplementation([self class], @selector(testAddMethod)), "v@:");
    
    // 注册一个类
    objc_registerClassPair(ZNTeacher);
    
    id tea = [[ZNTeacher alloc] init];
    [tea setValue:@18 forKey:@"_age"];
    [tea setValue:@"English" forKey:@"_teach"];
    
    [tea testAddMethod];
    NSLog(@"age:%d, teach:%@", [[tea valueForKey:@"_age"] intValue], [tea valueForKey:@"_teach"]);
    
    // 释放类
    objc_disposeClassPair(ZNTeacher);
}

// 打印结果
2019-07-08 14:23:53.674931 runtime-super[14867:1455487] <ZNTeacher: 0x17002eba0>==testAddMethod
2019-07-08 14:23:57.630018 runtime-super[14867:1455487] age:18, teach:English
```


## 二、成员变量

- `class_getInstanceVariable`

```Objective-C
// 获取特定的成员变量
Ivar nameIvar = class_getInstanceVariable([ZNPerson class], "_name");
Ivar ageIvar = class_getInstanceVariable([ZNPerson class], "_age");
    
NSLog(@"nameIvar:%s====%s", ivar_getName(nameIvar), ivar_getTypeEncoding(nameIvar));
NSLog(@"ageIvar:%s====%s", ivar_getName(ageIvar), ivar_getTypeEncoding(ageIvar));

// 打印结果
2019-07-08 15:40:29.970840 runtime-super[15081:1469699] nameIvar:_name====@"NSString"
2019-07-08 15:40:29.970947 runtime-super[15081:1469699] ageIvar:_age====i
```


- `object_setIvar`、`object_getIvar`

```Objective-C
// 给成员变量赋值 取值
object_setIvar(per, nameIvar, @"zhao");
object_setIvar(per, ageIvar, @10);
NSLog(@"%@===%@", object_getIvar(per, nameIvar), object_getIvar(per, ageIvar));

// 打印结果
2019-07-08 15:43:24.474779 runtime-super[15092:1470263] name：zhao===age：10
```

- `class_copyIvarList`

```Objective-C
// 获取所有成员变量
NSInteger count = 0;
Ivar *ivarList = class_copyIvarList([ZNPerson class], &count);
    
for (int i=0; i<count; i++) {
    NSLog(@"%s====%s", ivar_getName(ivarList[i]), ivar_getTypeEncoding(ivarList[i]));
}
free(ivarList);

// 打印结果
2019-07-08 15:43:24.474834 runtime-super[15092:1470263] _age====i
2019-07-08 15:43:24.474879 runtime-super[15092:1470263] _name====@"NSString"
```

## 三、方法


- `class_replaceMethod`

```Objective-C
// ViewController.m
- (void)testAddMethod
{
    NSLog(@"%@==%@", self, NSStringFromSelector(_cmd));
}

// 替换方法实现
// 参数: 要替换的Class、要替换的方法名、新的方法实现、新方法的方法签名
class_replaceMethod([ZNPerson class], @selector(testPerson), class_getMethodImplementation([self class], @selector(testAddMethod)), "v@:");
    
ZNPerson *per = [[ZNPerson alloc] init];
[per testPerson];

// 打印结果
2019-07-08 16:42:54.272942 runtime-super[15197:1478496] <ZNPerson: 0x174035040>==testPerson
```

- `class_copyMethodList`

```Objective-C
// 获取某个类的所有方法
NSInteger count = 0;
Method *methodList = class_copyMethodList([ZNPerson class], &count);
for (int i=0; i<count; i++) {
    NSLog(@"方法名：%s-----方法签名：%s-----方法实现：%p",
          method_getName(methodList[i]),
          method_getTypeEncoding(methodList[i]),
          method_getImplementation(methodList[i]));
}

// 打印结果
2019-07-08 16:51:47.777642 runtime-super[15219:1480004] 方法名：testPerson-----方法签名：v16@0:8-----方法实现：0x10004570c
2019-07-08 16:51:47.777713 runtime-super[15219:1480004] 方法名：.cxx_destruct-----方法签名：v16@0:8-----方法实现：0x100045650
2019-07-08 16:51:47.777770 runtime-super[15219:1480004] 方法名：setName:-----方法签名：v24@0:8@16-----方法实现：0x1000455ac
2019-07-08 16:51:47.777827 runtime-super[15219:1480004] 方法名：name-----方法签名：@16@0:8-----方法实现：0x100045570
2019-07-08 16:51:47.778032 runtime-super[15219:1480004] 方法名：setAge:-----方法签名：v20@0:8i16-----方法实现：0x10004561c
2019-07-08 16:51:47.778091 runtime-super[15219:1480004] 方法名：age-----方法签名：i16@0:8-----方法实现：0x1000455f0
2019-07-08 16:51:47.778404 runtime-super[15219:1480004] 方法名：test-----方法签名：v16@0:8-----方法实现：0x1000454c4
```

- `method_exchangeImplementations`

```Objective-C
// UIControl+Extension.m

+ (void)load
{
    // 拦截所有 UIButton 的点击事件
    Method method1 = class_getInstanceMethod(self, @selector(sendAction:to:forEvent:));
    Method method2 = class_getInstanceMethod(self, @selector(zn_sendAction:to:forEvent:));
    method_exchangeImplementations(method1, method2);
}

- (void)mj_sendAction:(SEL)action to:(id)target forEvent:(UIEvent *)event
{
    NSLog(@"%@-%@-%@", self, target, NSStringFromSelector(action));
    
    // 调用系统原来的实现
    [self zn_sendAction:action to:target forEvent:event];
    
    if ([self isKindOfClass:[UIButton class]]) {
        // 拦截了所有按钮的事件

    }
}
```



<br>

写于2019-07-08

<br>