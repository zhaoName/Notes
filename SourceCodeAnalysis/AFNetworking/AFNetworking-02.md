# AFNetworking - AFURLResponseSerialization

本文主要介绍 AFURLResponseSerialization，所用版本为 AFNetworking 4.0.1。


<br>

## 一、AFURLResponseSerialization

`AFURLResponseSerialization ` 是个协议，协议中只有一直方法，其作用就是判断 `response ` 是否合法。

如一个 JSON 响应会检查 content-type 是否为 `application/json`，HTTP status code 是否为 2xx，接收到的有效 JSON 数据是否能序列化成对象。

```Objective-C
/**
 The `AFURLResponseSerialization` protocol is adopted by an object that decodes data into a more useful object representation, according to details in the server response. Response serializers may additionally perform validation on the incoming response and data.

 For example, a JSON response serializer may check for an acceptable status code (`2XX` range) and content type (`application/json`), decoding a valid JSON response into an object.
 */
@protocol AFURLResponseSerialization <NSObject, NSSecureCoding, NSCopying>

/**
 The response object decoded from the data associated with a specified response.

 @param response The response to be processed.
 @param data The response data to be decoded.
 @param error The error that occurred while attempting to decode the response data.

 @return The object decoded from the specified response data.
 */
- (nullable id)responseObjectForResponse:(nullable NSURLResponse *)response
                           data:(nullable NSData *)data
                          error:(NSError * _Nullable __autoreleasing *)error NS_SWIFT_NOTHROW;

@end
```

`AFHTTPResponseSerializer` 及其子类都会遵循此协议中的方法。

<br>

## 一、AFHTTPResponseSerializer

### 0x01 `init`

`AFHTTPResponseSerializer` 的初始化方法如下:

```Objective-C
+ (instancetype)serializer {
    return [[self alloc] init];
}

- (instancetype)init {
    self = [super init];
    if (!self) {
        return nil;
    }
    
    self.acceptableStatusCodes = [NSIndexSet indexSetWithIndexesInRange:NSMakeRange(200, 100)];
    self.acceptableContentTypes = nil;
    
    return self;
}
```

会在 `init` 方法中设置 `self.acceptableStatusCodes` 的范围为 [200, 299]。

### 0x02 `responseObjectForResponse:data:error:`

`AFHTTPResponseSerializer ` 遵守的协议方法 `responseObjectForResponse:data:error:` 实现如下：

```Objective-C
- (BOOL)validateResponse:(NSHTTPURLResponse *)response data:(NSData *)data error:(NSError * __autoreleasing *)error
{
    BOOL responseIsValid = YES;
    NSError *validationError = nil;
    
    if ([response isKindOfClass:[NSHTTPURLResponse class]]) {
        // 若 MIMEType 不对，且 (MIMEType 不为空或 data 不为空), 说明请求头中设置的 content-type 有问题
        if (self.acceptableContentTypes
            && ![self.acceptableContentTypes containsObject:[response MIMEType]]
            && !([response MIMEType] == nil && [data length] == 0) ) {
            
            if ([data length] > 0 && [response URL]) {
                NSMutableDictionary *mutableUserInfo = [@{
                    NSLocalizedDescriptionKey: [NSString stringWithFormat:NSLocalizedStringFromTable(@"Request failed: unacceptable content-type: %@", @"AFNetworking", nil), [response MIMEType]],
                    NSURLErrorFailingURLErrorKey:[response URL],
                    AFNetworkingOperationFailingURLResponseErrorKey: response,
                } mutableCopy];
                if (data) {
                    mutableUserInfo[AFNetworkingOperationFailingURLResponseDataErrorKey] = data;
                }
                
                validationError = AFErrorWithUnderlyingError([NSError errorWithDomain:AFURLResponseSerializationErrorDomain code:NSURLErrorCannotDecodeContentData userInfo:mutableUserInfo], validationError);
            }
            
            responseIsValid = NO;
        }
        
        // 若 response.statusCode 不是 2xx 且  response.URL 有值，说明请求出错
        if (self.acceptableStatusCodes && ![self.acceptableStatusCodes containsIndex:(NSUInteger)response.statusCode] && [response URL]) {
            NSMutableDictionary *mutableUserInfo = [@{
                NSLocalizedDescriptionKey: [NSString stringWithFormat:NSLocalizedStringFromTable(@"Request failed: %@ (%ld)", @"AFNetworking", nil), [NSHTTPURLResponse localizedStringForStatusCode:response.statusCode], (long)response.statusCode],
                NSURLErrorFailingURLErrorKey:[response URL],
                AFNetworkingOperationFailingURLResponseErrorKey: response,
            } mutableCopy];
            
            if (data) {
                mutableUserInfo[AFNetworkingOperationFailingURLResponseDataErrorKey] = data;
            }
            
            validationError = AFErrorWithUnderlyingError([NSError errorWithDomain:AFURLResponseSerializationErrorDomain code:NSURLErrorBadServerResponse userInfo:mutableUserInfo], validationError);
            
            responseIsValid = NO;
        }
    }
    
    if (error && !responseIsValid) {
        *error = validationError;
    }
    
    return responseIsValid;
}

// 协议方法
- (id)responseObjectForResponse:(NSURLResponse *)response data:(NSData *)data error:(NSError *__autoreleasing *)error
{
    [self validateResponse:(NSHTTPURLResponse *)response data:data error:error];
    return data;
}
```

其内部会调用 `validateResponse:data:error:` 此方法作用有两个：

- 检验 content-type 是否正确

- 检验 status code 是否是 2xx

测试若 status code 为 300 会发生什么

```Objective-C
- (void)testThatAFHTTPResponseSerializationHandlesAll2XXCodes {
    // statusCode 取值范围 [200, 300]
    NSIndexSet *indexSet = [NSIndexSet indexSetWithIndexesInRange:NSMakeRange(200, 101)];
    [indexSet enumerateIndexesUsingBlock:^(NSUInteger statusCode, BOOL *stop) {
        NSHTTPURLResponse *response = [[NSHTTPURLResponse alloc] initWithURL:self.baseURL statusCode:(NSInteger)statusCode HTTPVersion:@"1.1" headerFields:@{@"Content-Type": @"text/html"}];

        XCTAssert([self.responseSerializer.acceptableStatusCodes containsIndex:statusCode], @"Status code %@ should be acceptable", @(statusCode));

        NSError *error = nil;
        [self.responseSerializer validateResponse:response data:[@"text" dataUsingEncoding:NSUTF8StringEncoding] error:&error];

        XCTAssertNil(error, @"Error handling status code %@", @(statusCode));
    }];
}
```

当 `indexSet` 遍历到值为 300 时，会有如下打印

```Objective-C
error: -[AFHTTPResponseSerializationTests testThatAFHTTPResponseSerializationHandlesAll2XXCodes] : (([self.responseSerializer.acceptableStatusCodes containsIndex:statusCode]) is true) failed - Status code 300 should be acceptable

error: -[AFHTTPResponseSerializationTests testThatAFHTTPResponseSerializationHandlesAll2XXCodes] : ((error) == nil) failed: "Error Domain=com.alamofire.error.serialization.response Code=-1011 "Request failed: multiple choices (300)" UserInfo={NSLocalizedDescription=Request failed: multiple choices (300), NSErrorFailingURLKey=https://httpbin.org, com.alamofire.serialization.response.error.data={length = 4, bytes = 0x74657874}, com.alamofire.serialization.response.error.response=<NSHTTPURLResponse: 0x7b080003e2e0> { URL: https://httpbin.org } { Status Code: 300, Headers {
    "Content-Type" =     (
        "text/html"
    );
} }}" - Error handling status code 300
```

<br>

## 二、AFJSONResponseSerializer

### 0x01 `init`

```Objective-C
+ (instancetype)serializer {
    return [self serializerWithReadingOptions:(NSJSONReadingOptions)0];
}

+ (instancetype)serializerWithReadingOptions:(NSJSONReadingOptions)readingOptions {
    AFJSONResponseSerializer *serializer = [[self alloc] init];
    serializer.readingOptions = readingOptions;
    
    return serializer;
}

- (instancetype)init {
    self = [super init];
    if (!self) {
        return nil;
    }
    
    self.acceptableContentTypes = [NSSet setWithObjects:@"application/json", @"text/json", @"text/javascript", nil];
    return self;
}
```
 `AFJSONResponseSerializer `的初始化方法有

- 指定 JSON 序列化默认方式

- 重写父类属性 `acceptableContentTypes` 值
- `acceptableStatusCodes ` 保持和父类一致


### 0x02 `responseObjectForResponse:data:error:`

 `AFJSONResponseSerializer ` 遵守的协议方法 `responseObjectForResponse:data:error:` 实现如下：

```Objective-C
- (id)responseObjectForResponse:(NSURLResponse *)response data:(NSData *)data error:(NSError *__autoreleasing *)error
{
    if (![self validateResponse:(NSHTTPURLResponse *)response data:data error:error]) {
        if (!error || AFErrorOrUnderlyingErrorHasCodeInDomain(*error, NSURLErrorCannotDecodeContentData, AFURLResponseSerializationErrorDomain)) {
            return nil;
        }
    }
    
    // Workaround for behavior of Rails to return a single space for `head :ok` (a workaround for a bug in Safari), which is not interpreted as valid input by NSJSONSerialization.
    // See https://github.com/rails/rails/issues/1742
    BOOL isSpace = [data isEqualToData:[NSData dataWithBytes:" " length:1]];
    
    if (data.length == 0 || isSpace) {
        return nil;
    }
    
    NSError *serializationError = nil;
    id responseObject = [NSJSONSerialization JSONObjectWithData:data options:self.readingOptions error:&serializationError];
    
    if (!responseObject)
    {
        if (error) {
            *error = AFErrorWithUnderlyingError(serializationError, *error);
        }
        return nil;
    }
    
    if (self.removesKeysWithNullValues) {
        // 过滤掉值为 NSNull 的字段
        return AFJSONObjectByRemovingKeysWithNullValues(responseObject, self.readingOptions);
    }
    
    return responseObject;
}
```
- 首先调用父类的 `validateResponse:data:error:` 判断 `statusCode` 和 `acceptableContentTypes` 是否合法

- 检验将 `data` 序列化成 JSON 对象是否出错
- 若设置 `removesKeysWithNullValues` 为 `YES` (默认是 `NO`)，则过滤掉值为 `NSNull` 的字段

下面的测试用例，会检测过滤掉值为 `NSNull` 的字段

```Objective-C
- (void)testThatJSONRemovesKeysWithNullValues {
    self.responseSerializer.removesKeysWithNullValues = YES;
    NSHTTPURLResponse *response = [[NSHTTPURLResponse alloc] initWithURL:self.baseURL statusCode:200 HTTPVersion:@"1.1" headerFields:@{@"Content-Type":@"text/json"}];
    NSData *data = [NSJSONSerialization dataWithJSONObject:@{@"key":@"value",@"nullkey":[NSNull null],@"array":@[@{@"subnullkey":[NSNull null]}], @"arrayWithNulls": @[[NSNull null]]}
                                                   options:(NSJSONWritingOptions)0
                                                     error:nil];

    NSError *error = nil;
    NSDictionary *responseObject = [self.responseSerializer responseObjectForResponse:response data:data error:&error];
    
    XCTAssertNil(error);
    XCTAssertNotNil(responseObject[@"key"]);
    XCTAssertNil(responseObject[@"nullkey"]);
    XCTAssertNil(responseObject[@"array"][0][@"subnullkey"]);
    XCTAssertEqualObjects(responseObject[@"arrayWithNulls"], @[]);
}
```

<br>


## 三、XML Response Serializer

AFNetworking 提供两种 XML 解析方式：`AFXMLParserResponseSerializer ` 和 `AFXMLDocumentResponseSerializer `。在这两个类的初始化方法中都会设置 `acceptableContentTypes`，如下：

```Objective-C
self.acceptableContentTypes = [[NSSet alloc] initWithObjects:@"application/xml", @"text/xml", nil];
```

`AFXMLParserResponseSerializer` 的解析如下：

```Objective-C
- (id)responseObjectForResponse:(NSHTTPURLResponse *)response
                           data:(NSData *)data
                          error:(NSError *__autoreleasing *)error
{
    if (![self validateResponse:(NSHTTPURLResponse *)response data:data error:error]) {
        if (!error || AFErrorOrUnderlyingErrorHasCodeInDomain(*error, NSURLErrorCannotDecodeContentData, AFURLResponseSerializationErrorDomain)) {
            return nil;
        }
    }
    
    return [[NSXMLParser alloc] initWithData:data];
}
```

`AFXMLDocumentResponseSerializer` 的解析如下：

```Objective-C
- (id)responseObjectForResponse:(NSURLResponse *)response data:(NSData *)data error:(NSError *__autoreleasing *)error
{
    if (![self validateResponse:(NSHTTPURLResponse *)response data:data error:error]) {
        if (!error || AFErrorOrUnderlyingErrorHasCodeInDomain(*error, NSURLErrorCannotDecodeContentData, AFURLResponseSerializationErrorDomain)) {
            return nil;
        }
    }
    
    NSError *serializationError = nil;
    NSXMLDocument *document = [[NSXMLDocument alloc] initWithData:data options:self.options error:&serializationError];
    
    if (!document)
    {
        if (error) {
            *error = AFErrorWithUnderlyingError(serializationError, *error);
        }
        return nil;
    }
    
    return document;
}
```

测试用例：


```Objective-C
static NSData * AFXMLTestData() {
    return [@"<?xml version=\"1.0\" encoding=\"UTF-8\"?><foo attr1=\"1\" attr2=\"2\"><bar>someValue</bar></foo>" dataUsingEncoding:NSUTF8StringEncoding];
}
```

```Objective-C
- (void)testThatXMLParserResponseSerializerReturnsNSXMLParserObjectForValidXML {
    NSHTTPURLResponse *response = [[NSHTTPURLResponse alloc] initWithURL:self.baseURL statusCode:200 HTTPVersion:@"1.1" headerFields:@{@"Content-Type": @"application/xml"}];
    NSError *error = nil;
    id responseObject = [self.responseSerializer responseObjectForResponse:response data:AFXMLTestData() error:&error];

    XCTAssertNil(error, @"Serialization error should be nil");
    XCTAssert([responseObject isKindOfClass:[NSXMLParser class]], @"Expected response to be a NSXMLParser");
}
```

<br>
