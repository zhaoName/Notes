# AFNetworking - AFURLRequestSerialization

本文主要介绍 `AFURLRequestSerialization`，所用版本为 AFNetworking 4.0.1。


<br>

## 一、AFQueryStringPair

`AFQueryStringPair` 的实现很简单

```Objective-C
@implementation AFQueryStringPair

- (instancetype)initWithField:(id)field value:(id)value {
    self = [super init];
    if (!self) {
        return nil;
    }

    self.field = field;
    self.value = value;

    return self;
}

- (NSString *)URLEncodedStringValue {
    if (!self.value || [self.value isEqual:[NSNull null]]) {
        return AFPercentEscapedStringFromString([self.field description]);
    } else {
        return [NSString stringWithFormat:@"%@=%@", AFPercentEscapedStringFromString([self.field description]), AFPercentEscapedStringFromString([self.value description])];
    }
}

@end
```

在 `- URLEncodedStringValue` 方法中会调用 `AFPercentEscapedStringFromString` 函数，实现如下：

```Objective-C
/**
 Returns a percent-escaped string following RFC 3986 for a query string key or value.
 RFC 3986 states that the following characters are "reserved" characters.
    - General Delimiters: ":", "#", "[", "]", "@", "?", "/"
    - Sub-Delimiters: "!", "$", "&", "'", "(", ")", "*", "+", ",", ";", "="

 In RFC 3986 - Section 3.4, it states that the "?" and "/" characters should not be escaped to allow
 query strings to include a URL. Therefore, all "reserved" characters with the exception of "?" and "/"
 should be percent-escaped in the query string.
    - parameter string: The string to be percent-escaped.
    - returns: The percent-escaped string.
 */
NSString * AFPercentEscapedStringFromString(NSString *string) {
    static NSString * const kAFCharactersGeneralDelimitersToEncode = @":#[]@"; // does not include "?" or "/" due to RFC 3986 - Section 3.4
    static NSString * const kAFCharactersSubDelimitersToEncode = @"!$&'()*+,;=";

    NSMutableCharacterSet * allowedCharacterSet = [[NSCharacterSet URLQueryAllowedCharacterSet] mutableCopy];
    [allowedCharacterSet removeCharactersInString:[kAFCharactersGeneralDelimitersToEncode stringByAppendingString:kAFCharactersSubDelimitersToEncode]];

	// FIXME: https://github.com/AFNetworking/AFNetworking/pull/3028
    // return [string stringByAddingPercentEncodingWithAllowedCharacters:allowedCharacterSet];

    static NSUInteger const batchSize = 50;

    NSUInteger index = 0;
    NSMutableString *escaped = @"".mutableCopy;

    while (index < string.length) {
        NSUInteger length = MIN(string.length - index, batchSize);
        NSRange range = NSMakeRange(index, length);

        // To avoid breaking up character sequences such as 👴🏻👮🏽
        range = [string rangeOfComposedCharacterSequencesForRange:range];

        NSString *substring = [string substringWithRange:range];
        NSString *encoded = [substring stringByAddingPercentEncodingWithAllowedCharacters:allowedCharacterSet];
        [escaped appendString:encoded];

        index += range.length;
    }
    NSLog(@"AFPercentEscapedStringFromString---string:%@ -- escaped:%@", string, escaped);
	return escaped;
}
```

这个函数的作用就是将参数进行百分号转义(包括 emoji 表情)，如下：

```Objective-C
emoji string:!👴🏿👷🏻👮🏽👴🏿👷🏻👮🏽
escaped:%21%F0%9F%91%B4%F0%9F%8F%BF%F0%9F%91%B7%F0%9F%8F%BB%F0%9F%91%AE%F0%9F%8F%BD%F0%9F%91%B4%F0%9F%8F%BF%F0%9F%91%B7%F0%9F%8F%BB%F0%9F%91%AE%F0%9F%8F%BD
```

<br>

## 二、AFQueryStringFromParameters()

`AFQueryStringFromParameters()` 函数的主要作用是将传入的参数(`NSDictionary`) 序列化成 URL 中所用的参数(xx=xx&xx=xx)。

```Objective-C
NSString * AFQueryStringFromParameters(NSDictionary *parameters) {
    NSMutableArray *mutablePairs = [NSMutableArray array];
    for (AFQueryStringPair *pair in AFQueryStringPairsFromDictionary(parameters)) {
        [mutablePairs addObject:[pair URLEncodedStringValue]];
    }

    NSString *result = [mutablePairs componentsJoinedByString:@"&"];
    NSLog(@"AFQueryStringFromParameters---%@", result);
    return result;
}

NSArray * AFQueryStringPairsFromDictionary(NSDictionary *dictionary) {
    return AFQueryStringPairsFromKeyAndValue(nil, dictionary);
}
```

```Objective-C
NSArray * AFQueryStringPairsFromKeyAndValue(NSString *key, id value) {
    NSLog(@"AFQueryStringPairsFromKeyAndValue --- %@ -- %@", key, value);
    NSMutableArray *mutableQueryStringComponents = [NSMutableArray array];

    NSSortDescriptor *sortDescriptor = [NSSortDescriptor sortDescriptorWithKey:@"description" ascending:YES selector:@selector(compare:)];

    if ([value isKindOfClass:[NSDictionary class]]) {
        NSDictionary *dictionary = value;
        // Sort dictionary keys to ensure consistent ordering in query string, which is important when deserializing potentially ambiguous sequences, such as an array of dictionaries
        for (id nestedKey in [dictionary.allKeys sortedArrayUsingDescriptors:@[ sortDescriptor ]]) {
            id nestedValue = dictionary[nestedKey];
            if (nestedValue) {
                [mutableQueryStringComponents addObjectsFromArray:AFQueryStringPairsFromKeyAndValue((key ? [NSString stringWithFormat:@"%@[%@]", key, nestedKey] : nestedKey), nestedValue)];
            }
        }
    } else if ([value isKindOfClass:[NSArray class]]) {
        NSArray *array = value;
        for (id nestedValue in array) {
            [mutableQueryStringComponents addObjectsFromArray:AFQueryStringPairsFromKeyAndValue([NSString stringWithFormat:@"%@[]", key], nestedValue)];
        }
    } else if ([value isKindOfClass:[NSSet class]]) {
        NSSet *set = value;
        for (id obj in [set sortedArrayUsingDescriptors:@[ sortDescriptor ]]) {
            [mutableQueryStringComponents addObjectsFromArray:AFQueryStringPairsFromKeyAndValue(key, obj)];
        }
    } else {
        [mutableQueryStringComponents addObject:[[AFQueryStringPair alloc] initWithField:key value:value]];
    }

    return mutableQueryStringComponents;
}
```

运行以下测试用例：

```Objective-C
- (void)testNestedDictionaryQueryStringFromParameters {
    // @"key[key1]=value&key2=value&"
    XCTAssertTrue([AFQueryStringFromParameters(@{@"key":@{@"key1": @"value"}, @"key2":@"value&"}) isEqualToString:@"key%5Bkey1%5D=value&key2=value%26"]);
}
```

打印过程：

```Objective-C
2022-06-05 00:11:45.721743+0800 xctest[73398:3514479] AFQueryStringPairsFromKeyAndValue --- (null) -- {
    key =     {
        key1 = value;
    };
    key2 = "value&";
}
2022-06-05 00:11:45.723358+0800 xctest[73398:3514479] AFQueryStringPairsFromKeyAndValue --- key -- {
    key1 = value;
}
2022-06-05 00:11:45.724044+0800 xctest[73398:3514479] AFQueryStringPairsFromKeyAndValue --- key[key1] -- value
2022-06-05 00:11:45.724801+0800 xctest[73398:3514479] AFQueryStringPairsFromKeyAndValue --- key2 -- value&
2022-06-05 00:11:45.725463+0800 xctest[73398:3514479] AFPercentEscapedStringFromString---string:key[key1] -- escaped:key%5Bkey1%5D
2022-06-05 00:11:45.725932+0800 xctest[73398:3514479] AFPercentEscapedStringFromString---string:value -- escaped:value
2022-06-05 00:11:45.726433+0800 xctest[73398:3514479] AFPercentEscapedStringFromString---string:key2 -- escaped:key2
2022-06-05 00:11:45.727347+0800 xctest[73398:3514479] AFPercentEscapedStringFromString---string:value& -- escaped:value%26
2022-06-05 00:11:45.727959+0800 xctest[73398:3514479] AFQueryStringFromParameters---key%5Bkey1%5D=value&key2=value%26
```

<br>

## 三、AFURLRequestSerialization

`AFURLRequestSerialization` 是一个协议，用于构建一个规范的 `NSURLRequest `。

```Objective-C
/**
 The `AFURLRequestSerialization` protocol is adopted by an object that encodes parameters for a specified HTTP requests. Request serializers may encode parameters as query strings, HTTP bodies, setting the appropriate HTTP header fields as necessary.

 For example, a JSON request serializer may set the HTTP body of the request to a JSON representation, and set the `Content-Type` HTTP header field value to `application/json`.
 */
@protocol AFURLRequestSerialization <NSObject, NSSecureCoding, NSCopying>

/**
 Returns a request with the specified parameters encoded into a copy of the original request.

 @param request The original request.
 @param parameters The parameters to be encoded.
 @param error The error that occurred while attempting to encode the request parameters.

 @return A serialized request.
 */
- (nullable NSURLRequest *)requestBySerializingRequest:(NSURLRequest *)request
                               withParameters:(nullable id)parameters
                                        error:(NSError * _Nullable __autoreleasing *)error NS_SWIFT_NOTHROW;

@end
```

在这个方法中主要实现一下功能：

- 创建新的 `NSURLRequest `，并将 original request  的 `mutableCopy` 一份

- 使用 `AFHTTPRequestSerializer ` 中的 `HTTPRequestHeaders` 属性设置 HTTP Header
- **最重要的作用：序列化请求参数**

具体实现看后面代码分析。

<br>

## 四、AFHTTPRequestSerializer

### 0x01 `init`

```Objective-C
+ (instancetype)serializer {
    return [[self alloc] init];
}

- (instancetype)init {
    self = [super init];
    if (!self) {
        return nil;
    }

    self.stringEncoding = NSUTF8StringEncoding;

    self.mutableHTTPRequestHeaders = [NSMutableDictionary dictionary];
    self.requestHeaderModificationQueue = dispatch_queue_create("requestHeaderModificationQueue", DISPATCH_QUEUE_CONCURRENT);

    // Accept-Language HTTP Header; see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.4
    NSMutableArray *acceptLanguagesComponents = [NSMutableArray array];
    [[NSLocale preferredLanguages] enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        float q = 1.0f - (idx * 0.1f);
        [acceptLanguagesComponents addObject:[NSString stringWithFormat:@"%@;q=%0.1g", obj, q]];
        *stop = q <= 0.5f;
    }];
    [self setValue:[acceptLanguagesComponents componentsJoinedByString:@", "] forHTTPHeaderField:@"Accept-Language"];

    // User-Agent Header; see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.43
    NSString *userAgent = nil;
#if TARGET_OS_IOS
    userAgent = [NSString stringWithFormat:@"%@/%@ (%@; iOS %@; Scale/%0.2f)", [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleExecutableKey] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleIdentifierKey], [[NSBundle mainBundle] infoDictionary][@"CFBundleShortVersionString"] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleVersionKey], [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion], [[UIScreen mainScreen] scale]];
#elif TARGET_OS_TV
    userAgent = [NSString stringWithFormat:@"%@/%@ (%@; tvOS %@; Scale/%0.2f)", [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleExecutableKey] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleIdentifierKey], [[NSBundle mainBundle] infoDictionary][@"CFBundleShortVersionString"] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleVersionKey], [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion], [[UIScreen mainScreen] scale]];
#elif TARGET_OS_WATCH
    userAgent = [NSString stringWithFormat:@"%@/%@ (%@; watchOS %@; Scale/%0.2f)", [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleExecutableKey] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleIdentifierKey], [[NSBundle mainBundle] infoDictionary][@"CFBundleShortVersionString"] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleVersionKey], [[WKInterfaceDevice currentDevice] model], [[WKInterfaceDevice currentDevice] systemVersion], [[WKInterfaceDevice currentDevice] screenScale]];
#elif defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
    userAgent = [NSString stringWithFormat:@"%@/%@ (Mac OS X %@)", [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleExecutableKey] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleIdentifierKey], [[NSBundle mainBundle] infoDictionary][@"CFBundleShortVersionString"] ?: [[NSBundle mainBundle] infoDictionary][(__bridge NSString *)kCFBundleVersionKey], [[NSProcessInfo processInfo] operatingSystemVersionString]];
#endif
    if (userAgent) {
        if (![userAgent canBeConvertedToEncoding:NSASCIIStringEncoding]) {
            NSMutableString *mutableUserAgent = [userAgent mutableCopy];
            if (CFStringTransform((__bridge CFMutableStringRef)(mutableUserAgent), NULL, (__bridge CFStringRef)@"Any-Latin; Latin-ASCII; [:^ASCII:] Remove", false)) {
                userAgent = mutableUserAgent;
            }
        }
        [self setValue:userAgent forHTTPHeaderField:@"User-Agent"];
    }

    // HTTP Method Definitions; see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html
    self.HTTPMethodsEncodingParametersInURI = [NSSet setWithObjects:@"GET", @"HEAD", @"DELETE", nil];

    self.mutableObservedChangedKeyPaths = [NSMutableSet set];
    for (NSString *keyPath in AFHTTPRequestSerializerObservedKeyPaths()) {
        if ([self respondsToSelector:NSSelectorFromString(keyPath)]) {
            [self addObserver:self forKeyPath:keyPath options:NSKeyValueObservingOptionNew context:AFHTTPRequestSerializerObserverContext];
        }
    }

    return self;
}
```

- 设置默认编码格式 `NSUTF8StringEncoding`

- 声明 `HTTPRequestHeaders` 和 修改 headers 所用的队列
- 设置请求头 Accept-Language
- 设置请求头 User-Agent
- `HTTPMethodsEncodingParametersInURI` 中包含 GET、HEAD、DELETE 三种请求，这三种请求的参数都是拼接在 URL 后面，而 POST、PUT 请求在放在 body 中。
- 声明 `mutableObservedChangedKeyPaths`，以便设置 `NSURLRequest` 属性。

<br>

### 0x02 `mutableObservedChangedKeyPaths `

这里需要将 `mutableObservedChangedKeyPaths` 单独说一下。它保存的是设置后的属性(没修改的不保存)，然后再遍历赋值给 `NSURLRequest` 的属性。为什么要这么做呢？

`AFHTTPRequestSerializer` 的目的是构造一个正确的 `NSURLRequest`，那`AFHTTPRequestSerializer` 就会暴露一些 `NSURLRequest` 所用到的属性，以便给使用者设置值。但是这些属性是基本数据类型，如 `timeoutInterval`，都会带有默认值，若直接赋值给 `NSURLRequest` 就分不清到底是外部设置的，还是默认值。所以我们采用 KVO 监听的方法，监听修改的属性和设置之后的值，然后保存起来再赋值给 `NSURLRequest`。

下面是`AFHTTPRequestSerializer` 暴露的属性：

```Objective-C
static NSArray * AFHTTPRequestSerializerObservedKeyPaths() {
    static NSArray *_AFHTTPRequestSerializerObservedKeyPaths = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _AFHTTPRequestSerializerObservedKeyPaths = @[NSStringFromSelector(@selector(allowsCellularAccess)),
                                                     NSStringFromSelector(@selector(cachePolicy)),
                                                     NSStringFromSelector(@selector(HTTPShouldHandleCookies)),
                                                     NSStringFromSelector(@selector(HTTPShouldUsePipelining)),
                                                     NSStringFromSelector(@selector(networkServiceType)),
                                                     NSStringFromSelector(@selector(timeoutInterval))];
    });

    return _AFHTTPRequestSerializerObservedKeyPaths;
}
```

然后通过 KVO 监听这些属性值的变化

```Objective-C
for (NSString *keyPath in AFHTTPRequestSerializerObservedKeyPaths()) {
    if ([self respondsToSelector:NSSelectorFromString(keyPath)]) {
        [self addObserver:self forKeyPath:keyPath options:NSKeyValueObservingOptionNew context:AFHTTPRequestSerializerObserverContext];
    }
}
```
若监听到这些属性值发生变化，则会将属性和属性值添加到 `mutableObservedChangedKeyPaths` 中

```Objective-C
- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(__unused id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{
    if (context == AFHTTPRequestSerializerObserverContext) {
        if ([change[NSKeyValueChangeNewKey] isEqual:[NSNull null]]) {
            [self.mutableObservedChangedKeyPaths removeObject:keyPath];
        } else {
            [self.mutableObservedChangedKeyPaths addObject:keyPath];
        }
    }
}
```

再将保存后的属性赋值给 `NSURLRequest`

```Objective-C
- (NSMutableURLRequest *)requestWithMethod:(NSString *)method
                                 URLString:(NSString *)URLString
                                parameters:(id)parameters
                                     error:(NSError *__autoreleasing *)error
{
    ...
    for (NSString *keyPath in self.mutableObservedChangedKeyPaths) {
        [mutableRequest setValue:[self valueForKeyPath:keyPath] forKey:keyPath];
    }
	...
}
```

<br>

### 0x03 `requestBySerializingRequest:withParameters:error:`

`requestBySerializingRequest:withParameters:error:` 就是协议 `AFURLRequestSerialization` 中声明的方法。在 `AFHTTPRequestSerializer` 中实现

```Objective-C
- (NSURLRequest *)requestBySerializingRequest:(NSURLRequest *)request
                               withParameters:(id)parameters
                                        error:(NSError *__autoreleasing *)error
{
    NSParameterAssert(request);

    NSMutableURLRequest *mutableRequest = [request mutableCopy];

    [self.HTTPRequestHeaders enumerateKeysAndObjectsUsingBlock:^(id field, id value, BOOL * __unused stop) {
        if (![request valueForHTTPHeaderField:field]) {
            [mutableRequest setValue:value forHTTPHeaderField:field];
        }
    }];

    NSString *query = nil;
    // 序列化参数
    if (parameters) {
        if (self.queryStringSerialization) {
            // 自定义序列化方式
            NSError *serializationError;
            query = self.queryStringSerialization(request, parameters, &serializationError);

            if (serializationError) {
                if (error) {
                    *error = serializationError;
                }

                return nil;
            }
        } else {
            // 默认序列化
            switch (self.queryStringSerializationStyle) {
                case AFHTTPRequestQueryStringDefaultStyle:
                    query = AFQueryStringFromParameters(parameters);
                    break;
            }
        }
    }

    if ([self.HTTPMethodsEncodingParametersInURI containsObject:[[request HTTPMethod] uppercaseString]]) {
        if (query && query.length > 0) {
            // `GET`, `HEAD`, 和 `DELETE`请求，直接将序列化后的参数拼接到 URL 后面
            mutableRequest.URL = [NSURL URLWithString:[[mutableRequest.URL absoluteString] stringByAppendingFormat:mutableRequest.URL.query ? @"&%@" : @"?%@", query]];
        }
    } else {
        // #2864: an empty string is a valid x-www-form-urlencoded payload
        if (!query) {
            query = @"";
        }
        if (![mutableRequest valueForHTTPHeaderField:@"Content-Type"]) {
            [mutableRequest setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
        }
        // 其他请求(如 POST ...) 序列化后的参数放在 body 中
        [mutableRequest setHTTPBody:[query dataUsingEncoding:self.stringEncoding]];
    }

    return mutableRequest;
}
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

<br>

```Objective-C

```

```Objective-C

```

```Objective-C

```

<br>