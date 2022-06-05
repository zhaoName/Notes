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

- 使用 `AFURLRequestSerialization ` 中的 `HTTPRequestHeaders` 属性设置 HTTP Header
- **最重要的作用：序列化请求参数**

具体实现看后面代码分析。

<br>

## 四、AFURLRequestSerialization


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