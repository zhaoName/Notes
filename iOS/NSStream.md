# NSStream


<br>

## 一、简介

流提供了一种简单的方式在不同和介质中交换数据，这种交换方式是与设备无关的。流是在通信路径中串行传输的连续的比特位序列。从编码的角度来看，流是单向的，因此流可以是输入流或输出流。除了基于文件的流外，其它形式的流都是不可查找的，这些流的数据一旦消耗完后，就无法从流对象中再次获取。

在 Cocoa 中包含三个与流相关的类：`NSStream`、`NSInputStream` 和 `NSOutputStream`。`NSStream` 是一个抽象基类，定义了所有流对象的基础接口和属性。`NSInputStream` 和 `NSOutputStream` 继承自 `NSStream`，实现了输入流和输出流的默认行为。

`NSInputStream` 继承自 `NSStream `，是一个只读的流

`NSOutputStream` 继承自 `NSStream `，是一个只写的流

下图描述了流的应用场景：

![](../Images/iOS/NSStream/NSStream_01.png)

从图中看，`NSInputStream` 可以从文件、`socket` 和 `NSData` 对象中获取数据；`NSOutputStream` 可以将数据写入文件、`socket`、内存缓存和 `NSData` 对象中。这三处类主要处理一些比较底层的任务。

流对象有一些相关的属性。大部分属性是用于处理网络安全和配置的，这些属性统称为SSL和SOCKS代理信息。两个比较重要的属性是：

- `NSStreamDataWrittenToMemoryStreamKey`：允许输出流查询写入到内存的数据

- `NSStreamFileCurrentOffsetKey`：允许操作基于文件的流的读写位置

可以给流对象指定一个代理对象。如果没有指定，则流对象作为自己的代理。流对象调用唯一的代理方法 `stream:handleEvent:` 来处理流相关的事件：

- 对于 `NSInputStream ` 来说，是有可用的数据可读取事件。我们可以使用 `read:maxLength:` 方法从流中获取数据。

- 对于 `NSOutputStream ` 来说，是准备好写入的数据事件。我们可以使用 `write:maxLength:` 方法将数据写入流。

Cocoa 中的流对象与 Core Foundation 中的流对象是对应的。我们可以通过 `toll-free` 桥接方法来进行相互转换。`NSStream`、`NSInputStream` 和 `NSOutputStream` 分别对应`CFStream`、`CFReadStream` 和 `CFWriteStream`。

但这两者间的实现并不是完全一样的。Cocoa 流使用代理方式异步处理回调(`scheduleInRunLoop:forMode:`) 而 Core Foundation 一般使用回调函数来处理数据。另外我们可以子类化 `NSStream`、`NSInputStream` 和 `NSOutputStream`，来自定义一些属性和行为，而 Core Foundation 中的流对象则无法进行扩展。

<br>


## 二、从 NSInputStream 中读取数据

从一个 `NSInputStream` 流中读取数据主要包括以下几个步骤：

- 从数据源中创建和初始化一个 `NSInputStream` 实例

- 将流对象放入一个 run loop 中并打开流
- 处理流对象发送到其代理的事件
- 当没有更多数据可读取时，关闭并销毁流对象。

### 0x01 准备 `NSInputStream` 实例

要创建一个 `NSInputStream` 实例，必须要有数据源。数据源可以是文件、`NSData` 对象和网络socket。创建好后，我们设置其代理对象，并将其放入到 run loop 中，然后打开流。

```Objective-C
- (void)setUpStreamForFile:(NSString *)path
{
    NSInputStream *inputStream = [[NSInputStream alloc] initWithFileAtPath:path];
    inputStream.delegate = self;
    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    [inputStream open];
}
```

在流对象放入 run loop 且有流事件(有可读数据)发生时，流对象会向代理对象发送`stream:handleEvent:` 消息。在打开流之前，我们需要调用流对象的 `scheduleInRunLoop:forMode:` 方法，这样做可以避免在没有数据可读时阻塞代理对象的操作。我们需要确保的是流对象被放入正确的 run loop 中，即放入流事件发生的那个线程的 run loop 中。


### 0x02 处理流事件

在流事件被打开后，我们可以使用 `streamStatus` 属性查看流的状态，用 `hasBytesAvailable` 属性检测是否有可读的数据，用 `streamError` 来查看流处理过程中产生的错误。

`NSStreamStatus` 的定义如下：

```Objective-C
typedef NS_ENUM(NSUInteger, NSStreamStatus) {
    NSStreamStatusNotOpen = 0,
    NSStreamStatusOpening = 1,
    NSStreamStatusOpen = 2,
    NSStreamStatusReading = 3,
    NSStreamStatusWriting = 4,
    NSStreamStatusAtEnd = 5,
    NSStreamStatusClosed = 6,
    NSStreamStatusError = 7
};
```

流一旦打开后，将会持续发送 `stream:handleEvent:` 消息给代理对象，直到流结束为止。这个消息接收一个 `NSStreamEvent` 常量作为参数，以标识事件的类型。对于 `NSInputStream` 对象，事件类型如下。通常我们会对 `NSStreamEventHasBytesAvailable` 更感兴趣。

```Objective-C
typedef NS_OPTIONS(NSUInteger, NSStreamEvent) {
    NSStreamEventNone = 0,
    NSStreamEventOpenCompleted = 1UL << 0,
    NSStreamEventHasBytesAvailable = 1UL << 1,
    NSStreamEventHasSpaceAvailable = 1UL << 2,
    NSStreamEventErrorOccurred = 1UL << 3,
    NSStreamEventEndEncountered = 1UL << 4
};
```

使用 `NSInputStream ` 读取文件，然后将读取到内容转化成字符串打印出来，代码如下：

```Objective-C
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    NSLog(@"%s -- %lu", __func__, eventCode);
    
    switch (eventCode) {
        case NSStreamEventHasBytesAvailable:
            [self printInputStreamContent:aStream];
            break;
        ...
    }
}

- (void)printInputStreamContent:(NSStream *)aStream
{
    uint8_t buf[1024];
    NSInteger len = 0;
    len = [(NSInputStream *)aStream read:buf maxLength:1024];
    if(len) {
        [self.data appendBytes:(const void *)buf length:len];
        NSLog(@"%@", [[NSString alloc] initWithData:self.data encoding:NSUTF8StringEncoding]);
    } else {
        NSLog(@"no buffer!");
    }
}
```

打印结果：

```Objective-C
2022-06-18 23:54:03.021859+0800 ZZFoundation[70482:7752276] -[ZZStreamViewController stream:handleEvent:] -- 1
2022-06-18 23:54:03.022058+0800 ZZFoundation[70482:7752276] -[ZZStreamViewController stream:handleEvent:] -- 2
2022-06-18 23:54:03.022356+0800 ZZFoundation[70482:7752276] 流读取到的内容: 
- [AFHTTPRequestSerializer multipartFormRequestWithMethod:URLString:parameters:constructingBodyWithBlock:error:]
    - [AFHTTPRequestSerializer requestWithMethod:URLString:parameters:error:]
    - [AFStreamingMultipartFormData initWithURLRequest:stringEncoding:]
    - AFQueryStringPairsFromDictionary()
    - [AFStreamingMultipartFormData appendPartWithFormData:name:]
        - [AFStreamingMultipartFormData appendPartWithHeaders:body:]
            - [AFMultipartBodyStream appendHTTPBodyPart]
    - [AFStreamingMultipartFormData requestByFinalizingMultipartFormData]
        - [AFMultipartBodyStream setInitialAndFinalBoundaries]
        - [AFMultipartBodyStream contentLength]
            - [AFHTTPBodyPart contentLength]

2022-06-18 23:54:03.022627+0800 ZZFoundation[70482:7752276] -[ZZStreamViewController stream:handleEvent:] -- 2
2022-06-18 23:54:03.022962+0800 ZZFoundation[70482:7752276] no buffer!
2022-06-18 23:54:03.023128+0800 ZZFoundation[70482:7752276] -[ZZStreamViewController stream:handleEvent:] -- 16
```

### 0x03 释放流对象

当 `NSInputStream` 读取到流的结尾时，会发送一个 `NSStreamEventEndEncountered` 事件给代理，代理对象此时应该销毁流对象。

```Objective-C
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    switch (eventCode) {
        ...
        case NSStreamEventEndEncountered:
            [aStream close];
            [aStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            aStream = nil;
            break;
        ...
    }
}
```


<br>

## 三、向 NSOutputStream 中写入数据


向 `NSOutputStream` 实例中写入数据需要经过一下几个步骤：

- 使用要写入的数据创建和初始化一个 `NSOutputStream` 实例，并设置代理对象

- 将流对象放到 run loop 中并打开流
- 处理流对象发送到代理对象中的事件
- 如果流对象写入数据到内存，则通过请求 `NSStreamDataWrittenToMemoryStreamKey` 属性来获取数据
- 当没有更多数据可供写入时，释放流对象

### 0x01 创建 `NSOutputStream` 实例对象

和创建 `NSInputStream` 类似，需要设置代理、`scheduleInRunLoop`、并打开输出流。数据可写入的位置包括文件、C缓存、程序内存和网络 socket。

```Objective-C
- (void)createOutputStream
{
    NSOutputStream *oStream = [[NSOutputStream alloc] initToMemory];
    [oStream setDelegate:self];
    [oStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [oStream open];
}
```


### 0x02 处理流事件

处理 `NSOutputStream ` 流事件时，需要注意一下两点

- `hasSpaceAvailable` 属性表示是否有空间来写入数据

- 在 `stream:handleEvent:` 中主要处理 `NSStreamEventHasSpaceAvailable` 事件，并调用流的`write:maxLength` 方法写数据。

```Objective-C
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    NSLog(@"%s -- %lu", __func__, eventCode);
    switch (eventCode) {
        case NSStreamEventHasSpaceAvailable:
        {
            uint8_t *readBytes = (uint8_t *)[self.data mutableBytes];
            readBytes += self.byteIndex;
            int data_len = (int)[self.data length];
            unsigned int len = (data_len - self.byteIndex >= 1024) ? 1024 : (data_len - self.byteIndex);
            uint8_t buf[len];
            
            (void)memcpy(buf, readBytes, len);
            
            len = [(NSOutputStream *)aStream write:buf maxLength:len];
            self.byteIndex += len;
            break;
        }
        ...
    }
}
```

注意 `write:maxLength` 的返回值，有以下三层意思: 

- 如果写入操作成功，则返回放入流的实际字节数。

- 如果流是固定长度并且已达到其容量，则返回 0。
- 如果写入流时出错，则返回 -1。

### 0x03 释放流对象

如果 `NSOutputStream` 对象的目标是应用的内存时，在 `NSStreamEventEndEncountered` 事件中可能需要从内存中获取流中的数据。我们将调用 `NSOutputStream` 对象的 `propertyForKey:` 的属性，并指定 key 为 `NSStreamDataWrittenToMemoryStreamKey`来 获取这些数据。

```Objective-C
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    NSLog(@"%s -- %lu", __func__, eventCode);
    switch (eventCode) {
        case NSStreamEventEndEncountered:
        {
            NSData *data = [(NSOutputStream *)aStream propertyForKey:NSStreamDataWrittenToMemoryStreamKey];
            if (data) {
                NSLog(@"写入到流读内容: %@", [[NSString alloc] initWithData:self.data encoding:NSUTF8StringEncoding]);
            } else {
                NSLog(@"No data written to memory!");
            }
            
            [aStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            [aStream close];
            aStream = nil;
            break;
        } 
        ...
    }
}
```

```Objective-C

```

<br>

```Objective-C

```

```Objective-C

```
```Objective-C

```
```Objective-C

```

<br>


<br>

**Reference**

- [Introduction to Stream Programming Guide for Cocoa](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Streams/Streams.html)

- [iOS中流(Stream)的使用](http://southpeak.github.io/page/8/)

<br>

