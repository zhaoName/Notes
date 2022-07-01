# AFNetworking - AFURLSessionManager

本文主要介绍 AFURLSessionManager 的实现，所用版本为 AFNetworking 4.0.1。

<br>

## 一、AFURLSessionManagerTaskDelegate

### 0x01 init

`AFURLSessionManagerTaskDelegate `的初始化代码如下：

```Objective-C
- (instancetype)initWithTask:(NSURLSessionTask *)task {
    self = [super init];
    if (!self) {
        return nil;
    }
    
    _mutableData = [NSMutableData data];
    _uploadProgress = [[NSProgress alloc] initWithParent:nil userInfo:nil];
    _downloadProgress = [[NSProgress alloc] initWithParent:nil userInfo:nil];
    
    __weak __typeof__(task) weakTask = task;
    for (NSProgress *progress in @[ _uploadProgress, _downloadProgress ])
    {
        progress.totalUnitCount = NSURLSessionTransferSizeUnknown;
        progress.cancellable = YES;
        progress.cancellationHandler = ^{
            [weakTask cancel];
        };
        progress.pausable = YES;
        progress.pausingHandler = ^{
            [weakTask suspend];
        };
#if AF_CAN_USE_AT_AVAILABLE
        if (@available(macOS 10.11, *))
#else
        if ([progress respondsToSelector:@selector(setResumingHandler:)])
#endif
        {
            progress.resumingHandler = ^{
                [weakTask resume];
            };
        }
        
        [progress addObserver:self
                   forKeyPath:NSStringFromSelector(@selector(fractionCompleted))
                      options:NSKeyValueObservingOptionNew
                      context:NULL];
    }
    return self;
}

```

- 声明接收网络请求数据的 `mutableData` 和上传、下载所用到的进度 `uploadProgress`、`_downloadProgress`

- 通过 `NSProgress` 来操作 `task` 的`cancel`、`suspend`、`resume`
- KVO 监听 `NSProgress ` 的 `fractionCompleted`。监听这个属性就相对于监听 `totalUnitCount` 和 `completedUnitCount` 这两个属性

### 0x02 进度回调

KVO 监听到上传或下载进度发生变化时，将进度通过 block 回调出去

```Objective-C
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context {
   if ([object isEqual:self.downloadProgress]) {
        if (self.downloadProgressBlock) {
            self.downloadProgressBlock(object);
        }
    }
    else if ([object isEqual:self.uploadProgress]) {
        if (self.uploadProgressBlock) {
            self.uploadProgressBlock(object);
        }
    }
}
```

### 0x03 代理

`AFURLSessionManagerTaskDelegate ` 实现 3 个代理

- `NSURLSessionDataDelegate`

```Objective-C
- (void)URLSession:(__unused NSURLSession *)session
          dataTask:(__unused NSURLSessionDataTask *)dataTask
    didReceiveData:(NSData *)data
{
    NSLog(@"%s -- %lld", __func__, dataTask.countOfBytesExpectedToReceive);
    self.downloadProgress.totalUnitCount = dataTask.countOfBytesExpectedToReceive;
    self.downloadProgress.completedUnitCount = dataTask.countOfBytesReceived;

    [self.mutableData appendData:data];
}

- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
   didSendBodyData:(int64_t)bytesSent
    totalBytesSent:(int64_t)totalBytesSent
totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend{
    
    self.uploadProgress.totalUnitCount = task.countOfBytesExpectedToSend;
    self.uploadProgress.completedUnitCount = task.countOfBytesSent;
}
```

- `NSURLSessionDownloadDelegate`

```Objective-C
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
      didWriteData:(int64_t)bytesWritten
 totalBytesWritten:(int64_t)totalBytesWritten
totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite{
    NSLog(@"%s -- %lld", __func__, totalBytesExpectedToWrite);
    self.downloadProgress.totalUnitCount = totalBytesExpectedToWrite;
    self.downloadProgress.completedUnitCount = totalBytesWritten;
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
 didResumeAtOffset:(int64_t)fileOffset
expectedTotalBytes:(int64_t)expectedTotalBytes{
    NSLog(@"%s -- %lld", __func__, expectedTotalBytes);
    self.downloadProgress.totalUnitCount = expectedTotalBytes;
    self.downloadProgress.completedUnitCount = fileOffset;
}

- (void)URLSession:(NSURLSession *)session
      downloadTask:(NSURLSessionDownloadTask *)downloadTask
didFinishDownloadingToURL:(NSURL *)location
{
    self.downloadFileURL = nil;

    if (self.downloadTaskDidFinishDownloading) {
        self.downloadFileURL = self.downloadTaskDidFinishDownloading(session, downloadTask, location);
        if (self.downloadFileURL) {
            NSError *fileManagerError = nil;

            if (![[NSFileManager defaultManager] moveItemAtURL:location toURL:self.downloadFileURL error:&fileManagerError]) {
                [[NSNotificationCenter defaultCenter] postNotificationName:AFURLSessionDownloadTaskDidFailToMoveFileNotification object:downloadTask userInfo:fileManagerError.userInfo];
            } else {
                [[NSNotificationCenter defaultCenter] postNotificationName:AFURLSessionDownloadTaskDidMoveFileSuccessfullyNotification object:downloadTask userInfo:nil];
            }
        }
    }
}
```

- `NSURLSessionTaskDelegate`

```Objective-C
- (void)URLSession:(__unused NSURLSession *)session
              task:(NSURLSessionTask *)task
didCompleteWithError:(NSError *)error
{
    NSLog(@"%s -- %@", __func__, error.localizedDescription);
    error = objc_getAssociatedObject(task, AuthenticationChallengeErrorKey) ?: error;
    __strong AFURLSessionManager *manager = self.manager;

    __block id responseObject = nil;

    NSMutableDictionary *userInfo = [NSMutableDictionary dictionary];
    userInfo[AFNetworkingTaskDidCompleteResponseSerializerKey] = manager.responseSerializer;

    //Performance Improvement from #2672
    NSData *data = nil;
    if (self.mutableData) {
        data = [self.mutableData copy];
        //We no longer need the reference, so nil it out to gain back some memory.
        self.mutableData = nil;
    }

#if AF_CAN_USE_AT_AVAILABLE && AF_CAN_INCLUDE_SESSION_TASK_METRICS
    if (@available(iOS 10, macOS 10.12, watchOS 3, tvOS 10, *)) {
        if (self.sessionTaskMetrics) {
            userInfo[AFNetworkingTaskDidCompleteSessionTaskMetrics] = self.sessionTaskMetrics;
        }
    }
#endif

    if (self.downloadFileURL) {
        userInfo[AFNetworkingTaskDidCompleteAssetPathKey] = self.downloadFileURL;
    } else if (data) {
        userInfo[AFNetworkingTaskDidCompleteResponseDataKey] = data;
    }

    if (error) {
        userInfo[AFNetworkingTaskDidCompleteErrorKey] = error;

        dispatch_group_async(manager.completionGroup ?: url_session_manager_completion_group(), manager.completionQueue ?: dispatch_get_main_queue(), ^{
            if (self.completionHandler) {
                // 请求出错 回调出错信息
                self.completionHandler(task.response, responseObject, error);
            }

            dispatch_async(dispatch_get_main_queue(), ^{
                // 请求完成通知，带着请求失败的错误信息
                [[NSNotificationCenter defaultCenter] postNotificationName:AFNetworkingTaskDidCompleteNotification object:task userInfo:userInfo];
            });
        });
    } else {
        dispatch_async(url_session_manager_processing_queue(), ^{
            NSError *serializationError = nil;
            // 请求到的数据序列化成对应的格式，如JSON、Plist
            responseObject = [manager.responseSerializer responseObjectForResponse:task.response data:data error:&serializationError];

            if (self.downloadFileURL) {
                responseObject = self.downloadFileURL;
            }

            if (responseObject) {
                userInfo[AFNetworkingTaskDidCompleteSerializedResponseKey] = responseObject;
            }

            if (serializationError) {
                userInfo[AFNetworkingTaskDidCompleteErrorKey] = serializationError;
            }

            dispatch_group_async(manager.completionGroup ?: url_session_manager_completion_group(), manager.completionQueue ?: dispatch_get_main_queue(), ^{
                if (self.completionHandler) {
                    // 请求成功回调
                    self.completionHandler(task.response, responseObject, serializationError);
                }

                dispatch_async(dispatch_get_main_queue(), ^{
                    // 请求完成通知，带着请求成功后的数据
                    [[NSNotificationCenter defaultCenter] postNotificationName:AFNetworkingTaskDidCompleteNotification object:task userInfo:userInfo];
                });
            });
        });
    }
}

#if AF_CAN_INCLUDE_SESSION_TASK_METRICS
- (void)URLSession:(NSURLSession *)session
              task:(NSURLSessionTask *)task
didFinishCollectingMetrics:(NSURLSessionTaskMetrics *)metrics AF_API_AVAILABLE(ios(10), macosx(10.12), watchos(3), tvos(10)) {
    self.sessionTaskMetrics = metrics;
}
#endif
```

前两个代理是处理和上传、下载进度相关的回调。最终在 `NSURLSessionTaskDelegate` 的 `URLSession: task:didCompleteWithError:` 方法中将请求到的数据 `responseSerializer ` 的反序列化方法将 `NSData` 转换成对应的数据类型。

调用如下测试用例方法：

```Objective-C
- (void)testDownloadTaskDoesReportProgress {
    __weak XCTestExpectation *expectation = [self expectationWithDescription:@"Progress should equal 1.0"];
    NSURLSessionTask *task;
    task = [self.localManager
            downloadTaskWithRequest:[self bigImageURLRequest]
            progress:^(NSProgress * _Nonnull downloadProgress) {
                if (downloadProgress.fractionCompleted == 1.0) {
                    [expectation fulfill];
                }
            }
            destination:nil
            completionHandler:nil];
    [task resume];
    [self waitForExpectationsWithCommonTimeout];
}
```

由于测试用例中的 `NSURLRequest` 没有设置 `Content-Lenght`，所以需要将 `NSURLSessionDownloadDelegate` 的代理方法作如下改动，否则 `NSProgress` 的 `totalUnitCount` 值为 -1。这样 `AFURLSessionTaskCompletionHandler` 回调会一直不能被触发，测试用例报错。

```Objective-C
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
      didWriteData:(int64_t)bytesWritten
 totalBytesWritten:(int64_t)totalBytesWritten
totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite{
    NSLog(@"%s -- %lld", __func__, totalBytesExpectedToWrite);
    self.downloadProgress.totalUnitCount = 901049; //totalBytesExpectedToWrite;
    self.downloadProgress.completedUnitCount = totalBytesWritten;
}
```

最终打印结果

```Objective-C
2022-07-01 23:02:23.640737+0800 xctest[31817:13764391] NSURLSessionTaskDelegate--didReceiveChallenge
2022-07-01 23:02:25.733972+0800 xctest[31817:13764394] NSURLSessionDownloadDelegate-didWriteData: 912-- 912 -- -1
2022-07-01 23:02:25.735327+0800 xctest[31817:13764394] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:25.769951+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 42786-- 43698 -- -1
2022-07-01 23:02:25.771309+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:25.884779+0800 xctest[31817:13764394] NSURLSessionDownloadDelegate-didWriteData: 8187-- 51885 -- -1
2022-07-01 23:02:25.885605+0800 xctest[31817:13764394] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:25.887157+0800 xctest[31817:13764334] NSURLSessionDownloadDelegate-didWriteData: 45154-- 97039 -- -1
2022-07-01 23:02:25.887567+0800 xctest[31817:13764334] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.005990+0800 xctest[31817:13764392] NSURLSessionDownloadDelegate-didWriteData: 12159-- 109198 -- -1
2022-07-01 23:02:26.007609+0800 xctest[31817:13764392] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.120715+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 32744-- 141942 -- -1
2022-07-01 23:02:26.121787+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.122969+0800 xctest[31817:13764394] NSURLSessionDownloadDelegate-didWriteData: 110526-- 252468 -- -1
2022-07-01 23:02:26.123437+0800 xctest[31817:13764394] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.346063+0800 xctest[31817:13764392] NSURLSessionDownloadDelegate-didWriteData: 8184-- 260652 -- -1
2022-07-01 23:02:26.346791+0800 xctest[31817:13764392] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.355251+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 163771-- 424423 -- -1
2022-07-01 23:02:26.356271+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.624842+0800 xctest[31817:13764392] NSURLSessionDownloadDelegate-didWriteData: 8187-- 432610 -- -1
2022-07-01 23:02:26.625679+0800 xctest[31817:13764392] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.701929+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 212860-- 645470 -- -1
2022-07-01 23:02:26.705741+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.728162+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 24588-- 670058 -- -1
2022-07-01 23:02:26.730118+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.734053+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 131016-- 801074 -- -1
2022-07-01 23:02:26.734776+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:26.846871+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 73687-- 874761 -- -1
2022-07-01 23:02:26.848076+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:27.045217+0800 xctest[31817:13764394] NSURLSessionDownloadDelegate-didWriteData: 8192-- 882953 -- -1
2022-07-01 23:02:27.046486+0800 xctest[31817:13764394] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
2022-07-01 23:02:27.057434+0800 xctest[31817:13764334] NSURLSessionTaskDelegate-didFinishCollectingMetrics++++++
2022-07-01 23:02:27.058081+0800 xctest[31817:13764390] NSURLSessionDownloadDelegate-didWriteData: 18096-- 901049 -- -1
2022-07-01 23:02:27.060104+0800 xctest[31817:13764390] -[AFURLSessionManagerTaskDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:] -- -1
```

<br>

## 二、_AFURLSessionTaskSwizzling



```Objective-C
- (instancetype)init {
    return [self initWithSessionConfiguration:nil];
}

- (instancetype)initWithSessionConfiguration:(NSURLSessionConfiguration *)configuration {
    self = [super init];
    if (!self) {
        return nil;
    }

    if (!configuration) {
        configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    }

    self.sessionConfiguration = configuration;

    self.operationQueue = [[NSOperationQueue alloc] init];
    self.operationQueue.maxConcurrentOperationCount = 1;

    self.responseSerializer = [AFJSONResponseSerializer serializer];

    self.securityPolicy = [AFSecurityPolicy defaultPolicy];

#if !TARGET_OS_WATCH
    self.reachabilityManager = [AFNetworkReachabilityManager sharedManager];
#endif
    // {TaskIdentifier: TaskDelegates}
    self.mutableTaskDelegatesKeyedByTaskIdentifier = [[NSMutableDictionary alloc] init];
    // 初始化锁
    self.lock = [[NSLock alloc] init];
    self.lock.name = AFURLSessionManagerLockName;

    [self.session getTasksWithCompletionHandler:^(NSArray *dataTasks, NSArray *uploadTasks, NSArray *downloadTasks) {
        for (NSURLSessionDataTask *task in dataTasks) {
            [self addDelegateForDataTask:task uploadProgress:nil downloadProgress:nil completionHandler:nil];
        }

        for (NSURLSessionUploadTask *uploadTask in uploadTasks) {
            [self addDelegateForUploadTask:uploadTask progress:nil completionHandler:nil];
        }

        for (NSURLSessionDownloadTask *downloadTask in downloadTasks) {
            [self addDelegateForDownloadTask:downloadTask progress:nil destination:nil completionHandler:nil];
        }
    }];

    return self;
}
```

- 初始化 `NSURLSessionConfiguration`

- 设置 `NSURLSession` 代理回调方法所处线程，串行异步队列
- 设置响应体的序列化参数
- 设置网络安全设置相关
- 初始化网络状态监听
- 初始化锁
- 初始化 `NSURLSession`, 指定代理、代理回调队列。

```Objective-C
```

```Objective-C
```

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


<br>