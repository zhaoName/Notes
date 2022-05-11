# NSOperation & NSOperationQueue

<br>

本文主要介绍 `NSOperation` 和 `NSOperationQueue` 的基础用法。

`NSOperation` 和 `NSOperationQueue` 是苹果提供一套基于 GCD 封装的，面向对象的多线程 API。相比较 GCD 而言，它使用起来更简单，容易控制。

- 操作（operation）：

	- 操作，换句话说就是你在线程中执行的那段代码。
	
	- 在 `NSOperation` 中，我们使用 `NSOperation`的子类`NSInvocationOperation`、`NSBlockOperation`，或者自定义继承自 `NSOperation `的子类来执行任务。

- 操作队列（operation queue）：
	- 用来存放操作的队列。默认情况调度队列遵循 FIFO（先进先出）的原则。但已入队的 `operation` 执行顺序受到 `queuePriority` 和依赖关系的影响。
	
	- 操作队列通过设置最大并发操作数（`maxConcurrentOperationCount`）来控制并发、串行。
	- `NSOperationQueue` 为我们提供了两种不同类型的队列：主队列和自定义队列。主队列运行在主线程之上，而自定义队列在子线程执行。

<br>

## 一、`NSOperationQueue`

`NSOperationQueue` 是规范 `NSOperation` 执行的队列。

`NSOperationQueue` 根据优先级和准备情况调用其当前已入队的 `NSOperation` 对象。将操作添加到队列后，它会一直保留在队列中，直到操作完成其任务。添加操作后，您不能手动从队列中执行删除操作。

`NSOperationQueue` 保留操作直到操作完成，并且队列本身保留到所有操作完成。使用未完成的操作挂起操作队列可能会导致内存泄漏。

#### 确定执行顺序

操作队列根据其准备情况、优先级和相互依赖性来组织和调用已入队的 `operation`。 如果所有已入队的 `operation` 都具有相同的 `queuePriority` 并且 `ready` 属性返回 `YES`，则队列会按照您添加它们的顺序调用它们。 否则，操作队列总是调用相对于其他准备就绪操作具有最高优先级的操作。

但是不要依赖队列语义来确保操作的执行顺序，因为操作准备情况的变化可能会改变结果执行顺序。 `operation` 的相互依赖关系为 `operation` 提供了绝对的执行顺序，即使这些 `operation` 位于不同的操作队列中。 一个 `operation` 对象在其所依赖 `operation` 完成运行之后才会准备运行。

#### 响应操作取消

完成它的任务并不一定意味着该操作完成了该任务，也可以取消操作。 取消操作对象会使对象留在队列中，但会通知对象它应该尽快停止其任务。 对于当前正在执行的操作，这意味着操作对象的工作代码必须检查取消状态，停止正在执行的操作，并将自身标记为已完成。 对于已排队但尚未执行的操作，队列仍必须调用操作对象的 `start` 方法，以便它可以处理取消事件并将自己标记为已完成。

取消操作会导致操作忽略它可能具有的任何依赖项。这种行为使得队列可以尽快调用操作的 `start` 方法。反过来， `start` 方法将操作移动到完成状态，以便可以将其从队列中删除。

#### 线程安全

您可以安全地使用来自多个线程的单个 `NSOperationQueue` 对象，而无需创建额外的锁来同步对该对象的访问。

操作队列使用 `Dispatch` 框架来启动其操作的执行。 因此，队列总是在单独的线程上调用操作，而不管操作是同步的还是异步的。

`NSOperationQueue` 就是个队列，单独使用没有意义。队列里需要放"东西"才有其意思，而这个"东西"就是接下来要介绍的 `NSOperation `。




<br>

## 二、NSOperation

`NSOperation` 是一个抽象的基类，不能直接使用它，可以使用系统提供的两个子类:`NSInvocationOperation`、`NSBlockOperation`，也可自定义继承自 `NSOperation `的子类。

### 0x01 `NSInvocationOperation`

当单独使用 `NSInvocationOperation` 时，`operation` 会在当前线程执行，不会开启新的线程。如下：

```Objective-C
- (void)useInvocationOperationOnly
{
    NSInvocationOperation *invocaOperation = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(operationTask1) object:nil];
    [invocaOperation start];
}

- (void)operationTask1
{
    for (int i=0; i<2; i++) {
        sleep(2);
        NSLog(@"operationTask1---%@", [NSThread currentThread]);
    }
}
```

打印结果如下：

```Objective-C
operationTask1---<NSThread: 0x600002b84880>{number = 1, name = main}
operationTask1---<NSThread: 0x600002b84880>{number = 1, name = main}
```


### 0x02 `NSBlockOperation`

单独使用 `NSBlockOperation`,并且只用 `blockOperationWithBlock` 创建的 `operation`，只会在当前线程中执行，不会开启新的线程,即使当前线程是子线程。

```Objective-C
- (void)useBlockOperationOnly
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSBlockOperation *blockOperation = [NSBlockOperation blockOperationWithBlock:^{
            for (int i=0; i<2; i++) {
                sleep(2);
                NSLog(@"operationTask2---%@", [NSThread currentThread]);
            }
        }];
        [blockOperation start];
    });
}
```

打印结果如下：

```Objective-C
operationTask2---<NSThread: 0x600003f9d6c0>{number = 5, name = (null)}
operationTask2---<NSThread: 0x600003f9d6c0>{number = 5, name = (null)}
```

`NSBlockOperation` 还提供一个添加额外操作的方法：`addExecutionBlock:`, 这些`operation`（包括`blockOperationWithBlock:` 创建的 `operation`）可以再不同线程中并发执行，当所有 `operation` 都执行完成才算完成。

```Objective-C
- (void)useBlockOperationAndAddExecutionBlock
{
    NSBlockOperation *blockOpe = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"operationTask3---%@", [NSThread currentThread]);
        }
    }];
    
    [blockOpe addExecutionBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"operationTask4---%@", [NSThread currentThread]);
        }
    }];
    [blockOpe addExecutionBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"operationTask5---%@", [NSThread currentThread]);
        }
    }];
    [blockOpe addExecutionBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"operationTask6---%@", [NSThread currentThread]);
        }
    }];
    [blockOpe addExecutionBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"operationTask7---%@", [NSThread currentThread]);
        }
    }];
    [blockOpe addExecutionBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"operationTask8---%@", [NSThread currentThread]);
        }
    }];
    [blockOpe addExecutionBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"operationTask9---%@", [NSThread currentThread]);
        }
    }];
    [blockOpe start];
}
```

如果添加的操作多的话，`blockOperationWithBlock:` 中的操作也可能会在其他线程（非当前线程）中执行，这是由系统决定的，并不是说添加到 `blockOperationWithBlock:` 中的操作一定会在当前线程中执行。

```Objective-C
// `blockOperationWithBlock:` 中的操作在其他线程,即不是在主线程中执行
2022-05-03 23:35:25.659277+0800 ZZFoundation[7834:3901084] operationTask3---<NSThread: 0x600001002940>{number = 4, name = (null)}
2022-05-03 23:35:25.659281+0800 ZZFoundation[7834:3901079] operationTask6---<NSThread: 0x600001008980>{number = 6, name = (null)}
2022-05-03 23:35:25.659275+0800 ZZFoundation[7834:3901019] operationTask5---<NSThread: 0x60000100c1c0>{number = 1, name = main}
2022-05-03 23:35:25.659275+0800 ZZFoundation[7834:3901080] operationTask4---<NSThread: 0x600001045780>{number = 5, name = (null)}
2022-05-03 23:35:27.659871+0800 ZZFoundation[7834:3901079] operationTask6---<NSThread: 0x600001008980>{number = 6, name = (null)}
2022-05-03 23:35:27.659871+0800 ZZFoundation[7834:3901019] operationTask5---<NSThread: 0x60000100c1c0>{number = 1, name = main}
2022-05-03 23:35:27.659871+0800 ZZFoundation[7834:3901080] operationTask4---<NSThread: 0x600001045780>{number = 5, name = (null)}
2022-05-03 23:35:27.659918+0800 ZZFoundation[7834:3901084] operationTask3---<NSThread: 0x600001002940>{number = 4, name = (null)}
2022-05-03 23:35:29.660307+0800 ZZFoundation[7834:3901019] operationTask7---<NSThread: 0x60000100c1c0>{number = 1, name = main}
2022-05-03 23:35:29.660307+0800 ZZFoundation[7834:3901079] operationTask8---<NSThread: 0x600001008980>{number = 6, name = (null)}
2022-05-03 23:35:29.660428+0800 ZZFoundation[7834:3901080] operationTask9---<NSThread: 0x600001045780>{number = 5, name = (null)}
2022-05-03 23:35:31.662021+0800 ZZFoundation[7834:3901080] operationTask9---<NSThread: 0x600001045780>{number = 5, name = (null)}
2022-05-03 23:35:31.662022+0800 ZZFoundation[7834:3901079] operationTask8---<NSThread: 0x600001008980>{number = 6, name = (null)}
2022-05-03 23:35:31.662022+0800 ZZFoundation[7834:3901019] operationTask7---<NSThread: 0x60000100c1c0>{number = 1, name = main}
```

单独使用 `NSOperation ` 基本都是在当前线程执行操作 (`addExecutionBlock:` 除外)，若需要在子线程中执行操作，需要 `NSOperation` & `NSOperationQueue` 配合使用。

<br>

## 三、`NSOperation` & `NSOperationQueue`

### 0x01 `NSOperation` 和 `NSOperationQueue` 配合使用

添加到主队列中的`operation`都会放到主线程中执行，添加到自定义队列中的 `operation` 都是在子线程中执行，且默认是并发执行。

```Objective-C
- (void)addOperationToQueue
{
    // 1、创建队列
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    // 2、创建operation
    NSInvocationOperation *invaOperation = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(operationTask1) object:nil];
    
    NSBlockOperation *blockOpe1 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"NSOperationQueue1---%@", [NSThread currentThread]);
        }
    }];
    NSBlockOperation *blockOpe2 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"NSOperationQueue2---%@", [NSThread currentThread]);
            [queue cancelAllOperations];
        }
    }];
    // 3、将operation添加到queue中
    [queue addOperation:invaOperation];
    [queue addOperation:blockOpe1];
    [queue addOperation:blockOpe2];
}
```

打印结果如下：

```Objective-C
2022-05-03 23:04:34.308751+0800 ZZFoundation[7284:3885668] operationTask1---<NSThread: 0x600002e5cdc0>{number = 5, name = (null)}
2022-05-03 23:04:34.308752+0800 ZZFoundation[7284:3885670] NSOperationQueue1---<NSThread: 0x600002e59680>{number = 4, name = (null)}
2022-05-03 23:04:34.308752+0800 ZZFoundation[7284:3885672] NSOperationQueue2---<NSThread: 0x600002e274c0>{number = 9, name = (null)}
2022-05-03 23:04:36.310176+0800 ZZFoundation[7284:3885672] NSOperationQueue2---<NSThread: 0x600002e274c0>{number = 9, name = (null)}
2022-05-03 23:04:36.310178+0800 ZZFoundation[7284:3885670] NSOperationQueue1---<NSThread: 0x600002e59680>{number = 4, name = (null)}
2022-05-03 23:04:36.310177+0800 ZZFoundation[7284:3885668] operationTask1---<NSThread: 0x600002e5cdc0>{number = 5, name = (null)}
```

使用`addOperationWithBlock:`，可不用创建`operation`，直接在 block 中添加操作。但是这样不能添加依赖、不能获取 `operation` 的状态如 `finished`、`executing`等。

```Objective-C
- (void)addOperationWithBlockToQueue
{
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    
    [queue addOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"addOperationWithBlock1---%@", [NSThread currentThread]);
        }
    }];
    
    [queue addOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"addOperationWithBlock2---%@", [NSThread currentThread]);
        }
    }];
    
    [queue addOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"addOperationWithBlock3---%@", [NSThread currentThread]);
        }
    }];
}
```

打印结果如下：

```Objective-C
2022-05-03 23:03:45.521514+0800 ZZFoundation[7259:3884816] addOperationWithBlock2---<NSThread: 0x600001e82500>{number = 6, name = (null)}
2022-05-03 23:03:45.521514+0800 ZZFoundation[7259:3884819] addOperationWithBlock1---<NSThread: 0x600001e80b00>{number = 7, name = (null)}
2022-05-03 23:03:45.521514+0800 ZZFoundation[7259:3884823] addOperationWithBlock3---<NSThread: 0x600001ea6a40>{number = 3, name = (null)}
2022-05-03 23:03:47.526798+0800 ZZFoundation[7259:3884816] addOperationWithBlock2---<NSThread: 0x600001e82500>{number = 6, name = (null)}
2022-05-03 23:03:47.526798+0800 ZZFoundation[7259:3884819] addOperationWithBlock1---<NSThread: 0x600001e80b00>{number = 7, name = (null)}
2022-05-03 23:03:47.526808+0800 ZZFoundation[7259:3884823] addOperationWithBlock3---<NSThread: 0x600001ea6a40>{number = 3, name = (null)}
```

### 0x02 `maxConcurrentOperationCount`

`maxConcurrentOperationCount`：做最大并发操作数，控制一个队列中可以有多少个`operation`并发执行。默认值为 `NSOperationQueueDefaultMaxConcurrentOperationCount ` 即 -1。这个值会根据系统来动态确定最大并发操作数。

`maxConcurrentOperationCount = 1` 时，所有`operation`串行执行

`maxConcurrentOperationCount > 1`时，`operation`并发执行，当然这个值不能超过系统限定，过大系统会自动调整 `min(系统限定值, 自定义值)`

```Obejctive-C
- (void)setMaxConcurrentOperationCount
{
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    queue.maxConcurrentOperationCount = 2;
    
    [queue addOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"maxConcurrentOperationCount1---%@", [NSThread currentThread]);
        }
    }];
    
    [queue addOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"maxConcurrentOperationCount2---%@", [NSThread currentThread]);
        }
    }];
    
    [queue addOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"maxConcurrentOperationCount3---%@", [NSThread currentThread]);
        }
    }];
    [queue addOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"maxConcurrentOperationCount4---%@", [NSThread currentThread]);
        }
    }];
}
```

打印结果如下：这样也能看出 `addOperation:` 默认 `operation` 是 FIFO（若未添加依赖关系）

```Objective-C
2022-05-04 00:09:12.307023+0800 ZZFoundation[8616:3925530] maxConcurrentOperationCount2---<NSThread: 0x6000016d7a80>{number = 6, name = (null)}
2022-05-04 00:09:12.307018+0800 ZZFoundation[8616:3925527] maxConcurrentOperationCount1---<NSThread: 0x600001690d40>{number = 7, name = (null)}
2022-05-04 00:09:14.310257+0800 ZZFoundation[8616:3925527] maxConcurrentOperationCount1---<NSThread: 0x600001690d40>{number = 7, name = (null)}
2022-05-04 00:09:14.310272+0800 ZZFoundation[8616:3925530] maxConcurrentOperationCount2---<NSThread: 0x6000016d7a80>{number = 6, name = (null)}
2022-05-04 00:09:16.310826+0800 ZZFoundation[8616:3925526] maxConcurrentOperationCount3---<NSThread: 0x6000016cfbc0>{number = 5, name = (null)}
2022-05-04 00:09:16.310826+0800 ZZFoundation[8616:3925527] maxConcurrentOperationCount4---<NSThread: 0x600001690d40>{number = 7, name = (null)}
2022-05-04 00:09:18.316436+0800 ZZFoundation[8616:3925526] maxConcurrentOperationCount3---<NSThread: 0x6000016cfbc0>{number = 5, name = (null)}
2022-05-04 00:09:18.316436+0800 ZZFoundation[8616:3925527] maxConcurrentOperationCount4---<NSThread: 0x600001690d40>{number = 7, name = (null)}
```

### 0x03 操作依赖

操作依赖可以控制操作的执行顺序。 默认情况下，具有依赖关系的操作对象在其所依赖的所有操作对象都完成执行之前不会被视为准备就绪状态。 然而，一旦最后一个所依赖的操作完成，操作对象就准备好并能够执行。

`NSOperation` 支持的依赖项不区分依赖操作是否成功完成。换句话说，已取消操作也会将其标记为已完成。

可以使用 `addDependency:` 和 `removeDependency:` 方法为操作添加和删除依赖项。还可以用 `dependencies` 属性来获取当前 `operation` 所依赖的所有操作。

操作依赖对不同队列的 `operation` 同样有效。

```Objective-C
- (void)operationAddDependency
{
    NSOperationQueue *queue1 = [[NSOperationQueue alloc] init];
    NSBlockOperation *ope1 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope1---%@", [NSThread currentThread]);
        }
    }];
    NSBlockOperation *ope2 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope2---%@", [NSThread currentThread]);
        }
        
    }];
    
    NSOperationQueue *queue2 = [[NSOperationQueue alloc] init];
    NSBlockOperation *ope3 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope3---%@", [NSThread currentThread]);
        }
    }];
    NSBlockOperation *ope4 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope4---%@", [NSThread currentThread]);
        }
    }];
    
    [queue1 addOperation:ope4];
    [queue1 addOperation:ope3];
    [queue2 addOperation:ope2];
    [queue2 addOperation:ope1];
}
```

如上代码，操作 `op4`、`op3`、`op2`、`op1` 并发执行。打印结果如下：

```Objective-C
2022-05-05 22:07:56.583949+0800 ZZFoundation[44496:4797239] ope1---<NSThread: 0x600001424740>{number = 8, name = (null)}
2022-05-05 22:07:56.583929+0800 ZZFoundation[44496:4797240] ope3---<NSThread: 0x600001429940>{number = 4, name = (null)}
2022-05-05 22:07:56.583928+0800 ZZFoundation[44496:4797241] ope4---<NSThread: 0x600001420b00>{number = 7, name = (null)}
2022-05-05 22:07:56.583933+0800 ZZFoundation[44496:4797242] ope2---<NSThread: 0x60000142d740>{number = 3, name = (null)}
2022-05-05 22:07:58.584999+0800 ZZFoundation[44496:4797241] ope4---<NSThread: 0x600001420b00>{number = 7, name = (null)}
2022-05-05 22:07:58.584999+0800 ZZFoundation[44496:4797242] ope2---<NSThread: 0x60000142d740>{number = 3, name = (null)}
2022-05-05 22:07:58.584999+0800 ZZFoundation[44496:4797239] ope1---<NSThread: 0x600001424740>{number = 8, name = (null)}
2022-05-05 22:07:58.584999+0800 ZZFoundation[44496:4797240] ope3---<NSThread: 0x600001429940>{number = 4, name = (null)}
```

添加依赖后, 能控制 `op1`、`op2`、`op3`、`op4` 执行顺序

```Objective-C
- (void)operationAddDependency
{
    ...
    
    [ope2 addDependency:ope1];
    [ope3 addDependency:ope1];
    NSLog(@"%@ -- %@", ope1.dependencies, ope2.dependencies);

    [ope4 addDependency:ope2];
    [ope4 addDependency:ope3];
    NSLog(@"%@", ope4.dependencies);
    
    [queue1 addOperation:ope4];
    [queue1 addOperation:ope3];
    [queue2 addOperation:ope2];
    [queue2 addOperation:ope1];
}
```

打印结果如下：

```Objective-C
2022-05-05 22:09:31.934086+0800 ZZFoundation[44535:4798414] (
) -- (
    "<NSBlockOperation: 0x7fb12c606750>"
)
2022-05-05 22:09:31.934345+0800 ZZFoundation[44535:4798414] (
    "<NSBlockOperation: 0x7fb12c608180>",
    "<NSBlockOperation: 0x7fb12c608490>"
)
2022-05-05 22:09:33.939431+0800 ZZFoundation[44535:4798481] ope1---<NSThread: 0x60000009d500>{number = 3, name = (null)}
2022-05-05 22:09:35.940097+0800 ZZFoundation[44535:4798481] ope1---<NSThread: 0x60000009d500>{number = 3, name = (null)}
2022-05-05 22:09:37.942762+0800 ZZFoundation[44535:4798485] ope2---<NSThread: 0x6000000cb980>{number = 6, name = (null)}
2022-05-05 22:09:37.942762+0800 ZZFoundation[44535:4798484] ope3---<NSThread: 0x600000094740>{number = 4, name = (null)}
2022-05-05 22:09:39.943689+0800 ZZFoundation[44535:4798485] ope2---<NSThread: 0x6000000cb980>{number = 6, name = (null)}
2022-05-05 22:09:39.943731+0800 ZZFoundation[44535:4798484] ope3---<NSThread: 0x600000094740>{number = 4, name = (null)}
2022-05-05 22:09:41.944432+0800 ZZFoundation[44535:4798485] ope4---<NSThread: 0x6000000cb980>{number = 6, name = (null)}
2022-05-05 22:09:43.945972+0800 ZZFoundation[44535:4798485] ope4---<NSThread: 0x6000000cb980>{number = 6, name = (null)}
```

### 0x04 `queuePriority`

`NSOperation` 提供了`queuePriority`（优先级）属性，`queuePriority` 属性适用于同一操作队列中的操作，不适用于不同操作队列中的操作。默认情况下，所有新创建的操作对象优先级都是`NSOperationQueuePriorityNormal`。

```Objective-C
typedef NS_ENUM(NSInteger, NSOperationQueuePriority) {
	NSOperationQueuePriorityVeryLow = -8L,
	NSOperationQueuePriorityLow = -4L,
	NSOperationQueuePriorityNormal = 0,
	NSOperationQueuePriorityHigh = 4,
	NSOperationQueuePriorityVeryHigh = 8
};
```


```Objective-C
- (void)setQueuePriority
{
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    queue.maxConcurrentOperationCount = 2;
    
    for (int i=0; i<10; i++)
    {
        NSBlockOperation *op = [NSBlockOperation blockOperationWithBlock:^{
            sleep(2);
            NSLog(@"operationTask%d---%@", i, [NSThread currentThread]);
        }];
        op.queuePriority = i < 5 ? NSOperationQueuePriorityVeryLow : NSOperationQueuePriorityVeryHigh;
        [queue addOperation:op];
    }
}
```

打印结果如下：

```Objective-C
2022-05-04 10:19:24.525595+0800 ZZFoundation[17996:4142112] operationTask0---<NSThread: 0x6000027a6c40>{number = 4, name = (null)}
2022-05-04 10:19:24.525595+0800 ZZFoundation[17996:4142111] operationTask1---<NSThread: 0x6000027d04c0>{number = 6, name = (null)}
2022-05-04 10:19:26.530875+0800 ZZFoundation[17996:4142110] operationTask5---<NSThread: 0x6000027a2380>{number = 3, name = (null)}
2022-05-04 10:19:26.530874+0800 ZZFoundation[17996:4142112] operationTask6---<NSThread: 0x6000027a6c40>{number = 4, name = (null)}
2022-05-04 10:19:28.532884+0800 ZZFoundation[17996:4142112] operationTask8---<NSThread: 0x6000027a6c40>{number = 4, name = (null)}
2022-05-04 10:19:28.532912+0800 ZZFoundation[17996:4142113] operationTask7---<NSThread: 0x6000027d94c0>{number = 7, name = (null)}
2022-05-04 10:19:30.533368+0800 ZZFoundation[17996:4142112] operationTask2---<NSThread: 0x6000027a6c40>{number = 4, name = (null)}
2022-05-04 10:19:30.533368+0800 ZZFoundation[17996:4142110] operationTask9---<NSThread: 0x6000027a2380>{number = 3, name = (null)}
2022-05-04 10:19:32.538526+0800 ZZFoundation[17996:4142112] operationTask3---<NSThread: 0x6000027a6c40>{number = 4, name = (null)}
2022-05-04 10:19:32.538557+0800 ZZFoundation[17996:4142113] operationTask4---<NSThread: 0x6000027d94c0>{number = 7, name = (null)}
```

### 0x05 依赖和优先级

当一个操作的所有依赖都已经完成时，操作对象通常会进入准备就绪状态，等待执行。

`queuePriority` 属性决定了进入准备就绪状态下的操作之间的开始执行顺序。并且，优先级不能取代依赖关系。

如果一个队列中既包含高优先级操作，又包含低优先级操作，并且两个操作都已经准备就绪，那么队列先执行高优先级操作。

如果一个队列中既包含了准备就绪状态的操作，又包含了未准备就绪的操作，未准备就绪的操作优先级比准备就绪的操作优先级高。那么，虽然准备就绪的操作优先级低，也会优先执行。优先级不能取代依赖关系。如果要控制操作间的启动顺序，则必须使用依赖关系。


```Objective-C
- (void)operationAddDependency
{
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    queue.maxConcurrentOperationCount = 1;
    
    NSBlockOperation *ope1 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope1---%@", [NSThread currentThread]);
        }
    }];
    NSBlockOperation *ope2 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope2---%@", [NSThread currentThread]);
        }
        
    }];
    NSBlockOperation *ope3 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope3---%@", [NSThread currentThread]);
        }
    }];
    NSBlockOperation *ope4 = [NSBlockOperation blockOperationWithBlock:^{
        for (int i=0; i<2; i++) {
            sleep(2);
            NSLog(@"ope4---%@", [NSThread currentThread]);
        }
    }];
    
    // 先添加依赖关系
    [ope2 addDependency:ope1];
    [ope3 addDependency:ope1];
    
    ope2.queuePriority = NSOperationQueuePriorityLow;
    ope3.queuePriority = NSOperationQueuePriorityHigh;
    
    [ope4 addDependency:ope2];
    [ope4 addDependency:ope3];
    
    [queue addOperation:ope4];
    [queue addOperation:ope3];
    [queue addOperation:ope2];
    [queue addOperation:ope1];
}
```

打印结果如下：

```Objective-C
2022-05-04 10:33:43.747564+0800 ZZFoundation[18316:4152159] ope1---<NSThread: 0x600003989f00>{number = 4, name = (null)}
2022-05-04 10:33:45.750100+0800 ZZFoundation[18316:4152159] ope1---<NSThread: 0x600003989f00>{number = 4, name = (null)}
2022-05-04 10:33:47.754802+0800 ZZFoundation[18316:4152161] ope3---<NSThread: 0x6000039c8940>{number = 5, name = (null)}
2022-05-04 10:33:49.760038+0800 ZZFoundation[18316:4152161] ope3---<NSThread: 0x6000039c8940>{number = 5, name = (null)}
2022-05-04 10:33:51.764869+0800 ZZFoundation[18316:4152159] ope2---<NSThread: 0x600003989f00>{number = 4, name = (null)}
2022-05-04 10:33:53.767601+0800 ZZFoundation[18316:4152159] ope2---<NSThread: 0x600003989f00>{number = 4, name = (null)}
2022-05-04 10:33:55.773155+0800 ZZFoundation[18316:4152159] ope4---<NSThread: 0x600003989f00>{number = 4, name = (null)}
2022-05-04 10:33:57.776049+0800 ZZFoundation[18316:4152159] ope4---<NSThread: 0x600003989f00>{number = 4, name = (null)}
```

- 没有依赖关系的是 `op1`，先执行。

- `op2`、`op3` 都依赖 `op1`，当 `op1` 执行完成后，`op2`、`op3`进入准备就绪状态。因为 `op3` 的优先级比 `op2` 高，所以 `op3` 先执行。

- `op4` 依赖于 `op2`、`op3`。当 `op2`、`op3` 都执行完成，`op4`才会执行。

<br />

### 0x06 线程之前的通信

如子线程中下载图片 完成后回到主线程刷新界面。

```
- (void)threadCommunication
{
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    
    NSBlockOperation *ope1 = [NSBlockOperation blockOperationWithBlock:^{
        [NSThread sleepForTimeInterval:2];
        NSLog(@"子线程下载图片1---%@", [NSThread currentThread]);
    }];
    
    NSBlockOperation *ope2 = [NSBlockOperation blockOperationWithBlock:^{
        [NSThread sleepForTimeInterval:2];
        NSLog(@"子线程下载图片2---%@", [NSThread currentThread]);
    }];
    NSBlockOperation *ope3 = [NSBlockOperation blockOperationWithBlock:^{
        [NSThread sleepForTimeInterval:2];
        NSLog(@"回到主线程 合并图片---%@", [NSThread currentThread]);
    }];
    // 不同队列的operation可以相互依赖
    [ope3 addDependency:ope2];
    [ope3 addDependency:ope1];
    
    [queue addOperation:ope1];
    [queue addOperation:ope2];
    
    // 模拟两个子线程做完事 回到主线程刷新UI
    [[NSOperationQueue mainQueue] addOperation:ope3];
}
```

打印结果如下：

```
2022-05-04 10:39:27.272761+0800 ZZFoundation[18440:4156173] 子线程下载图片2---<NSThread: 0x6000005cdf40>{number = 5, name = (null)}
2022-05-04 10:39:27.272761+0800 ZZFoundation[18440:4156170] 子线程下载图片1---<NSThread: 0x600000593b40>{number = 6, name = (null)}
2022-05-04 10:39:29.274200+0800 ZZFoundation[18440:4156076] 回到主线程 合并图片---<NSThread: 0x6000005d0240>{number = 1, name = main}
```

### 0x07 线程安全

涉及到多线程，肯定会想到线程安全，以保证数据的正确性。苹果提供了NSLock、@synchronized（这个东西耗性能，不提倡使用。苹果都不给代码自动补全）等线程锁来保证线程安全。下面引用常用的卖票的例子说明

```
// 模拟出售火车票 解释线程安全
// 初始化票数 卖票窗口 并开始卖票
- (void)initTicketStatueSave
{
    self.ticketCount = 50;
    
    // 1号售票窗口
    NSOperationQueue *shQueue = [[NSOperationQueue alloc] init];
    shQueue.maxConcurrentOperationCount = 1;
    // 2号售票窗口
    NSOperationQueue *bjQueue = [[NSOperationQueue alloc] init];
    bjQueue.maxConcurrentOperationCount = 1;
    
    NSBlockOperation *shOpe = [NSBlockOperation blockOperationWithBlock:^{
        [self saleTicketSafely];
    }];
    
    NSBlockOperation *bjOpe = [NSBlockOperation blockOperationWithBlock:^{
        [self saleTicketSafely];
    }];
    
    // 开始售票
    [shQueue addOperation:shOpe];
    [bjQueue addOperation:bjOpe];
}

- (void)saleTicketSafely
{
    while (1) {
        @synchronized(self) {
            if (self.ticketCount > 0) {
                self.ticketCount--;
                NSLog(@"%@", [NSString stringWithFormat:@"剩余票数:%zd 窗口:%@", self.ticketCount, [NSThread currentThread]]);
                [NSThread sleepForTimeInterval:0.5];
            }
        }
        
        if (self.ticketCount <= 0) {
            NSLog(@"所有车票都已出售");
            break;
        }
    }
}
```

打印结果如下：

```
2022-05-04 10:45:05.811493+0800 ZZFoundation[18592:4161534] 剩余票数:49 窗口:<NSThread: 0x600003df8c40>{number = 6, name = (null)}
2022-05-04 10:45:06.313315+0800 ZZFoundation[18592:4161535] 剩余票数:48 窗口:<NSThread: 0x600003d83100>{number = 4, name = (null)}
...
2022-05-04 10:45:30.509002+0800 ZZFoundation[18592:4161534] 剩余票数:0 窗口:<NSThread: 0x600003df8c40>{number = 6, name = (null)}
2022-05-04 10:45:30.509000+0800 ZZFoundation[18592:4161535] 所有车票都已出售
2022-05-04 10:45:31.013914+0800 ZZFoundation[18592:4161534] 所有车票都已出售
```


<br>

**Reference**：

- [Threading Programming Guide](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Multithreading/Introduction/Introduction.html)

- [Concurrency Programming Guide](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/Introduction/Introduction.html)

- [NSOperation](https://developer.apple.com/documentation/foundation/nsoperation)

- [NSOperationQueue](https://developer.apple.com/documentation/foundation/nsoperationqueue)

- [iOS多线程：『NSOperation、NSOperationQueue』详尽总结](https://bujige.net/blog/iOS-Complete-learning-NSOperation.html)

- [并发编程：API 及挑战](https://objccn.io/issue-2-1/)

<br>