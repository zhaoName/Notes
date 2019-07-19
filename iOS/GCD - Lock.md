
# GCD - Lock

<br>

`GCD`适用于多线程开发的`API`，而提到多线程开发就不可避免的会遇到资源抢夺的问题。如多个线程访问同一个对象、同一个变量、同一个文件，这些都可能会导致数据错乱。

下面以卖票为例，总共有15张票，3个窗口一起买票

```
- (void)ticket
{
    self.totalTicketCount = 15;
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        for (int i=0; i<5; i++) {
            [self saleTicket];
        }
    });
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        for (int i=0; i<5; i++) {
            [self saleTicket];
        }
    });
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        for (int i=0; i<5; i++) {
            [self saleTicket];
        }
    });
}

- (void)saleTicket
{
    // 这里不能直接用 self.totalTicketCount--
    NSInteger count = self.totalTicketCount;
    count--;
    sleep(.2);
    self.totalTicketCount = count;
    NSLog(@"还剩余%zd张票 %@", self.totalTicketCount, [NSThread currentThread]);
}

// 打印结果
2019-07-19 11:43:32.524106+0800 GCD-Lock[2764:78991] 还剩余14张票 <NSThread: 0x600001a52dc0>{number = 4, name = (null)}
2019-07-19 11:43:32.524117+0800 GCD-Lock[2764:78995] 还剩余14张票 <NSThread: 0x600001a65c40>{number = 3, name = (null)}
2019-07-19 11:43:32.524116+0800 GCD-Lock[2764:78989] 还剩余14张票 <NSThread: 0x600001a60600>{number = 5, name = (null)}
2019-07-19 11:43:32.524438+0800 GCD-Lock[2764:78989] 还剩余13张票 <NSThread: 0x600001a60600>{number = 5, name = (null)}
2019-07-19 11:43:32.524438+0800 GCD-Lock[2764:78991] 还剩余13张票 <NSThread: 0x600001a52dc0>{number = 4, name = (null)}
2019-07-19 11:43:32.524442+0800 GCD-Lock[2764:78995] 还剩余13张票 <NSThread: 0x600001a65c40>{number = 3, name = (null)}
2019-07-19 11:43:32.524555+0800 GCD-Lock[2764:78989] 还剩余12张票 <NSThread: 0x600001a60600>{number = 5, name = (null)}
2019-07-19 11:43:32.524568+0800 GCD-Lock[2764:78991] 还剩余11张票 <NSThread: 0x600001a52dc0>{number = 4, name = (null)}
2019-07-19 11:43:32.524584+0800 GCD-Lock[2764:78995] 还剩余10张票 <NSThread: 0x600001a65c40>{number = 3, name = (null)}
2019-07-19 11:43:32.524701+0800 GCD-Lock[2764:78989] 还剩余9张票 <NSThread: 0x600001a60600>{number = 5, name = (null)}
2019-07-19 11:43:32.525957+0800 GCD-Lock[2764:78991] 还剩余8张票 <NSThread: 0x600001a52dc0>{number = 4, name = (null)}
2019-07-19 11:43:32.526448+0800 GCD-Lock[2764:78995] 还剩余7张票 <NSThread: 0x600001a65c40>{number = 3, name = (null)}
2019-07-19 11:43:32.526540+0800 GCD-Lock[2764:78989] 还剩余7张票 <NSThread: 0x600001a60600>{number = 5, name = (null)}
2019-07-19 11:43:32.526893+0800 GCD-Lock[2764:78991] 还剩余6张票 <NSThread: 0x600001a52dc0>{number = 4, name = (null)}
2019-07-19 11:43:32.527395+0800 GCD-Lock[2764:78995] 还剩余5张票 <NSThread: 0x600001a65c40>{number = 3, name = (null)}
```

这是典型的多线程同时访问同一变量的问题，这种问题可以用线程同步解决。

> 线程同步：即当有一个线程在对内存进行操作时，其他线程都不可以对这个内存地址进行操作，直到该线程完成操作， 其他线程才能对该内存地址进行操作，而其他线程又处于等待状态，实现线程同步的方法有很多，加锁就是其中一种。

下面来介绍 iOS 中常用的锁。

## 一、`OSSpinLock`

`OSSpinLock `自旋锁，等待锁的线程会处于忙等 (busy-wait) 状态，一直占用着 CPU 资源。

```
#import <libkern/OSAtomic.h>

// 初始化
self.spinLock = OS_SPINLOCK_INIT;

- (void)saleTicket
{
    // 加锁
    OSSpinLockLock(&_spinLock);
    NSInteger count = self.totalTicketCount;
    count--;
    sleep(.2);
    self.totalTicketCount = count;
    NSLog(@"还剩余%zd张票 %@", self.totalTicketCount, [NSThread currentThread]);
    // 解锁
    OSSpinLockUnlock(&_spinLock);
}
```

但`OSSpinLock`在 iOS10 之后被弃用，因为它不再安全，可能会出现优先级反转的问题。

具体来说就是当一个优先级比较低的线程已获得锁并访问共享资源，同时还有一个优先级较高的线程也尝试获得这个锁访问共享资源。低优先级线程无法与高优先级线程争夺 CPU 时间，CPU 会一直调用高优先级的线程，但锁已被低优先级占用，执行到加锁代码就会暂停。这样就会导致任务迟迟无法完成，低优先级线程中的锁也无法释放，线程卡死。


## 二、`os_unfair_lock`


`os_unfair_lock`是 iOS10 之后推出替代`OSSpinLock`的锁。从底层调用看，等待`os_unfair_lock`锁的线程会处于休眠状态，并非忙等。使用方式和`OSSpinLock`类似

```
#import <os/lock.h>

self.unfairLock = OS_UNFAIR_LOCK_INIT;

- (void)saleTicket
{
    // 加锁
    os_unfair_lock_lock(&_unfairLock);
    NSInteger count = self.totalTicketCount;
    count--;
    sleep(.2);
    self.totalTicketCount = count;
    NSLog(@"还剩余%zd张票 %@", self.totalTicketCount, [NSThread currentThread]);
    // 解锁
    os_unfair_lock_unlock(&_unfairLock);
}
```


## 三、`pthread_mutex`


`pthread_mutex` 互斥锁，等待锁的线程处于休眠状态。使用起来相对麻烦点。

### 0x01 常规使用

```
#import <pthread.h>

// 初始化 pthread_mutex 属性
pthread_mutexattr_t mutexattr;
pthread_mutexattr_init(&mutexattr);

// #define PTHREAD_MUTEX_NORMAL：不提供死锁检测,尝试重新锁定互斥锁会导致死锁。如果某个线程尝试解除锁定的互斥锁不是由该线程锁定或未锁定，则将产生不确定的行为。
// #define PTHREAD_MUTEX_ERRORCHECK：提供错误检查。如果某个线程尝试重新锁定的互斥锁已经由该线程锁定，则将返回错误。如果某个线程尝试解除锁定的互斥锁不是由该线程锁定或者未锁定，则将返回错误
// #define PTHREAD_MUTEX_RECURSIVE：线程首次成功获取互斥锁时，锁定计数会设置为 1。线程每重新锁定该互斥锁一次，锁定计数就增加 1。线程每解除锁定该互斥锁一次，锁定计数就减小 1。 锁定计数达到 0 时，该互斥锁即可供其他线程获取
// #define PTHREAD_MUTEX_DEFAULT：等同 PTHREAD_MUTEX_NORMAL
pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
// 初始化 pthread_mutex
pthread_mutex_init(&_mutex, &mutexattr);

- (void)saleTicket
{
    // 加锁
    pthread_mutex_lock(&_mutex);
    NSInteger count = self.totalTicketCount;
    count--;
    sleep(.2);
    self.totalTicketCount = count;
    NSLog(@"还剩余%zd张票 %@", self.totalTicketCount, [NSThread currentThread]);
    // 解锁
    pthread_mutex_unlock(&_mutex);
}
```

### 0x02 `PTHREAD_MUTEX_RECURSIVE`

`pthread_mutex`有个类型为`PTHREAD_MUTEX_RECURSIVE`，在解释前我们先看段代码

```
- (void)ticket
{ 
    // 初始化 pthread_mutex 属性
    pthread_mutexattr_init(&_mutexattr);
    pthread_mutexattr_settype(&_mutexattr, PTHREAD_MUTEX_NORMAL);
    // 初始化 pthread_mutex
    pthread_mutex_init(&_mutex, &_mutexattr);
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [self testRecursive];
    });
}

- (void)testRecursive
{
    pthread_mutex_lock(&_mutex);
    NSLog(@"%s %@", __func__, [NSThread currentThread]);
    [self testRecursive];
    pthread_mutex_unlock(&_mutex);
}

// 打印结果
2019-07-19 16:47:32.419311+0800 GCD-Lock[7690:232453] -[ViewController testRecursive] <NSThread: 0x600001676dc0>{number = 3, name = (null)}
```

打印的结果和我们想的不一样，没有产生递归调用。这是因为死锁的缘故。像这种递归调用需要加递归锁。

递归锁：允许同一线程对一把锁进行重复加锁。

```
// pthread_mutexattr_settype(&_mutexattr, PTHREAD_MUTEX_NORMAL);

pthread_mutexattr_settype(&_mutexattr, PTHREAD_MUTEX_RECURSIVE);

// 打印结果
2019-07-19 16:54:56.750545+0800 GCD-Lock[7864:237882] -[ViewController testRecursive] <NSThread: 0x600000ad1e40>{number = 3, name = (null)}
2019-07-19 16:54:56.750804+0800 GCD-Lock[7864:237882] -[ViewController testRecursive] <NSThread: 0x600000ad1e40>{number = 3, name = (null)}
2019-07-19 16:54:56.752339+0800 GCD-Lock[7864:237882] -[ViewController testRecursive] <NSThread: 0x600000ad1e40>{number = 3, name = (null)}
...
```

### 0x03 `pthread_cond_t`

引用百度百科中一句话解释`pthread_cond_t `

> 条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。为了防止竞争，条件变量的使用总是和一个互斥锁结合在一起。

```
- (void)ticket
{   
    // 初始化 pthread_mutex 属性
    pthread_mutexattr_init(&_mutexattr);
    pthread_mutexattr_settype(&_mutexattr, PTHREAD_MUTEX_RECURSIVE);
    // 初始化 pthread_mutex
    pthread_mutex_init(&_mutex, &_mutexattr);
    // 初始化 pthred_cond_t
    pthread_cond_init(&_cond, NULL);
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [self removeObject];
    });
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [self addObject];
    });
}

- (void)addObject
{
    NSLog(@"addObject---begin");
    pthread_mutex_lock(&_mutex);
    NSLog(@"%s===%@", __func__, [NSThread currentThread]);
    [_datas addObject:@"1111"];
    pthread_cond_signal(&_cond);
    // 广播 唤醒多个 wait
    //pthread_cond_broadcast(&_cond);
    pthread_mutex_unlock(&_mutex);
}

- (void)removeObject
{
    NSLog(@"removeObject---begin");
    pthread_mutex_lock(&_mutex);
    NSLog(@"%s===%@", __func__, [NSThread currentThread]);
    if (_datas.count == 0) {
        pthread_cond_wait(&_cond, &_mutex);
    }
    [_datas removeLastObject];
    pthread_mutex_unlock(&_mutex);
}

// 打印结果
2019-07-19 17:46:25.776023+0800 GCD-Lock[8984:268147] removeObject---begin
2019-07-19 17:46:25.776023+0800 GCD-Lock[8984:268146] addObject---begin
2019-07-19 17:46:25.776756+0800 GCD-Lock[8984:268147] -[ViewController removeObject]===<NSThread: 0x600002505000>{number = 3, name = (null)}
2019-07-19 17:46:25.778982+0800 GCD-Lock[8984:268146] -[ViewController addObject]===<NSThread: 0x600002538500>{number = 4, name = (null)}
```

可以看到是先执行`removeObject `，但是删除元素需要数组里有元素，这时删除元素线程挂起，直到添加元素线程给数组添加元素后再执行。


当然最后不要忘记销毁

```
- (void)dealloc
{
    pthread_cond_destroy(&_cond);
    pthread_mutexattr_destroy(&_mutexattr);
    pthread_mutex_destroy(&_mutex);
}
```

### 四、`NSLock`


`NSLock`是对`PTHREAD_MUTEX_ERRORCHECK 类型的``pthread_mutex`的封装。遵守`NSLocking`协议。

```
// NSLock.m

pthread_mutexattr_init(&attr_reporting);
pthread_mutexattr_settype(&attr_reporting, PTHREAD_MUTEX_ERRORCHECK);
      
- (id) init
{
  if (nil != (self = [super init])) {
      if (0 != pthread_mutex_init(&_mutex, &attr_reporting)) {
          DESTROY(self);
      }
  }
  return self;
}
```

用法很简单

```
self.lock = [[NSLock alloc] init];

- (void)saleTicket
{
    [self.lock lock];
    NSInteger count = self.totalTicketCount;
    count--;
    sleep(.2);
    self.totalTicketCount = count;
    NSLog(@"还剩余%zd张票 %@", self.totalTicketCount, [NSThread currentThread]);
    // 解锁
    [self.lock unlock];
}
```

## 五、`NSRecursiveLock `

`NSRecursiveLock `是对`PTHREAD_MUTEX_ERRORCHECK 类型的``pthread_mutex`的封装。遵守`NSLocking`协议。

```
// NSLock.m

pthread_mutexattr_init(&attr_recursive);
pthread_mutexattr_settype(&attr_recursive, PTHREAD_MUTEX_RECURSIVE);

- (id) init
{
  if (nil != (self = [super init])) {
      if (0 != pthread_mutex_init(&_mutex, &attr_recursive)) {
          DESTROY(self);
      }
  }
  return self;
}
```

用法很简单 和`NSLock`很相似。

```
- (void)ticket
{
    self.recursiveLock = [[NSRecursiveLock alloc] init];
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [self testRecursive];
    });
}

- (void)testRecursive
{
    [self.recursiveLock lock];
    NSLog(@"%s %@", __func__, [NSThread currentThread]);
    [self testRecursive];
    [self.recursiveLock unlock];
}

// 打印结果
2019-07-19 22:28:54.031941+0800 GCD-Lock[13120:345153] -[ViewController testRecursive] <NSThread: 0x600001230140>{number = 3, name = (null)}
2019-07-19 22:28:54.032290+0800 GCD-Lock[13120:345153] -[ViewController testRecursive] <NSThread: 0x600001230140>{number = 3, name = (null)}
2019-07-19 22:28:54.032504+0800 GCD-Lock[13120:345153] -[ViewController testRecursive] <NSThread: 0x600001230140>{number = 3, name = (null)}
...
```


## 六、`NSCondition`

`NSCondition`是对`pthread_mutex`和`cond`的封装。

```
// NSLock.m

- (id) init
{
  if (nil != (self = [super init])) {
      if (0 != pthread_cond_init(&_condition, NULL)) {
          DESTROY(self);
      }
      else if (0 != pthread_mutex_init(&_mutex, &attr_reporting)) {
          pthread_cond_destroy(&_condition);
          DESTROY(self);
      }
  }
  return self;
}

- (void) broadcast
{
   pthread_cond_broadcast(&_condition);
}

- (void)signal
{
   pthread_cond_signal(&_condition);
}

- (void)wait
{
   pthread_cond_wait(&_condition, &_mutex);
}
```

```
- (void)addObject
{
    NSLog(@"addObject---begin");
    [self.condition lock];
    NSLog(@"%s===%@", __func__, [NSThread currentThread]);
    [_datas addObject:@"1111"];
    [self.condition signal];
    // 广播 通知多个 wait
    //[self.condition broadcast];
    [self.condition unlock];
}

- (void)removeObject
{
    NSLog(@"removeObject---begin");
    [self.condition lock];
    NSLog(@"%s===%@", __func__, [NSThread currentThread]);
    if (_datas.count == 0) {
        [self.condition wait];
    }
    [_datas removeLastObject];
    [self.condition unlock];
}
```





<br>

**参考：**

- [不再安全的 OSSpinLock](https://blog.ibireme.com/2016/01/16/spinlock_is_unsafe_in_ios/)



<br>

写于 2019-07-19

<br>