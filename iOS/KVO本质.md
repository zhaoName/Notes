# KVO本质


<br>

`KVO`全称:`Key-Value Observing`,也称键值监听，用来监听某个对象的属性值的改变。


## 一、准备工作

- 创建工程，新建`KVOPerson`类


```
// KVOPerson.h
@property (nonatomic, assign) NSInteger age;

// KVOPerson.m
- (void)setAge:(NSInteger)age
{
    _age = age;
    NSLog(@"当前Person:%@, age:%zd", self, age);
}
```

- `ViewController`


```
// ViewController.m
- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.person1 = [[KVOPerson alloc] init];
    self.person1.age = 10;
    
    self.person2 = [[KVOPerson alloc] init];
    self.person2.age = 20;
    
    [self.person1 addObserver:self forKeyPath:@"age" options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew context:nil];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    self.person1.age = 11;
    self.person2.age = 21;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context
{
    NSLog(@"%@的%@的属性值a发送变动:%@", object, keyPath, change);
}

- (void)dealloc
{
    [self.person1 removeObserver:self forKeyPath:@"age"];
}
```

## 二、猜测

- 执行上述工程，可得到如下打印

```
KVO[61199:5633209] 当前Person:<KVOPerson: 0x60000283a290>, age:10

KVO[61199:5633209] 当前Person:<KVOPerson: 0x60000283a2b0>, age:20
```

- 点击屏幕，触发`KVO` 打印如下

```
KVO[61199:5633209] 当前Person:<KVOPerson: 0x60000283a290>, age:11

KVO[61199:5633209] <KVOPerson: 0x60000283a290>的age属性值发送变动:{
    kind = 1;
    new = 11;
    old = 10;
}

KVO[61199:5633209] 当前Person:<KVOPerson: 0x60000283a2b0>, age:21
```

从上述打印可以看到，点击屏幕前后都走了`- (void)setAge:(NSInteger)age`方法，不同的是点击屏幕后添加键值监听的`perosn1`在执行`setAge:`方法后，会触发`observeValueForKeyPath:ofObject:change:context:`方法，打印键值监听信息；而没添加键值监听的`person2`在执行`setAge:`方法后，就没反应了。

查看代码可以看到，类对象`KVOPerson`只有一个属性，实例对象`person1`和`person2`除了添加键值监听与否之外，并没有什么差别。而且都走的是同一个`setAge:`方法，那么`KVO`的底层实现就和属性的`setter`方法没关系。

**这里可以大胆猜测下，`KVO`的底层实现应该和类本身有关。**


## 三、本质验证

我们知道






<br>

<br>
