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

### 0x02 `validateResponse:data:error:`

`validateResponse:data:error:` 的实现如下:

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

此方法作用有两个：

- 检验 content-type 是否正确

- 检验 status code 是否是 2xx


```Objective-C
```

```Objective-C
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

<br>
