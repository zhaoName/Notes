# Aspects - 01




<br>

## 一、AspectInfo

### 0x01 `@protocol AspectInfo`

`@protocol AspectInfo` 的定义如下：

```Objective-C
/// The AspectInfo protocol is the first parameter of our block syntax.
@protocol AspectInfo <NSObject>

/// The instance that is currently hooked.
- (id)instance;

/// The original invocation of the hooked method.
- (NSInvocation *)originalInvocation;

/// All method arguments, boxed. This is lazily evaluated.
- (NSArray *)arguments;

@end
```

从注释可知：

- `protocol AspectInfo` 是 block 语法的第一个参数

- `instance` 当前被 hook 的实例对象
- `originalInvocation` 被 hooked 方法的原始的 invocation
- `arguments` 方法的所有参数，懒加载实现

<br>

### 0x02 `class AspectInfo`

`AspectInfo.h` 定义如下

```Objective-C
@interface AspectInfo : NSObject <AspectInfo>
- (id)initWithInstance:(__unsafe_unretained id)instance invocation:(NSInvocation *)invocation;
@property (nonatomic, unsafe_unretained, readonly) id instance;
@property (nonatomic, strong, readonly) NSArray *arguments;
@property (nonatomic, strong, readonly) NSInvocation *originalInvocation;
@end
```

`AspectInfo.m` 定义如下

```Objective-C
@implementation AspectInfo

@synthesize arguments = _arguments;

- (id)initWithInstance:(__unsafe_unretained id)instance invocation:(NSInvocation *)invocation {
    NSCParameterAssert(instance);
    NSCParameterAssert(invocation);
    if (self = [super init]) {
        _instance = instance;
        _originalInvocation = invocation;
    }
    return self;
}

- (NSArray *)arguments {
    // Lazily evaluate arguments, boxing is expensive.
    if (!_arguments) {
        _arguments = self.originalInvocation.aspects_arguments;
    }
    return _arguments;
}
```

`AspectInfo` 是继承于 `NSObject`，并且遵循了 `AspectInfo` 协议。

`- (id)initWithInstance:invocation:`方法，把外面传进来的实例`instance` 和原始的 `invocation` 保存到 `AspectInfo` 实例对应的成员变量中。

`- (NSArray *)arguments` 是一个懒加载方法，返回的是原始的 `invocation` 里面的`aspects` 参数数组。

`aspects_arguments` 这个 `getter` 方法是怎么实现的呢？作者是通过一个为`NSInvocation` 添加一个分类来实现的。

<br>

### 0x03 `NSInvocation (Aspects)`

`NSInvocation+Aspects.h` 定义如下：

```Objective-C
@interface NSInvocation (Aspects)
- (NSArray *)aspects_arguments;
@end
```

`NSInvocation+Aspects.m` 定义如下：

```Objective-C
@implementation NSInvocation (Aspects)
...
- (NSArray *)aspects_arguments {
	NSMutableArray *argumentsArray = [NSMutableArray array];
	for (NSUInteger idx = 2; idx < self.methodSignature.numberOfArguments; idx++) {
		[argumentsArray addObject:[self aspect_argumentAtIndex:idx] ?: NSNull.null];
	}
	return [argumentsArray copy];
}

@end
```

作者为原始的 `NSInvocation` 类添加一个 `Aspects` 分类，这个分类中只向外暴露了一个方法 `aspects_arguments`，返回值是一个数组，数组里面包含了当前 `invocation` 的所有参数。

`aspects_arguments`方法内部实现很简单，就是一层for循环，把 `methodSignature` 方法签名里面的参数，都加入到数组里，最后把数组返回。

有两个地方需要详细说明。一是为什么循环从 2 开始，二是 `[self aspect_argumentAtIndex:idx]` 内部是怎么实现的。

对于 OC 来说所有方法的调用都是走消息发送机制 `objc_msgSend(void /* id self, SEL op, ... */ )`。`objc_msgSend `固定包含两个参数，第一个参数是消息发送者 `self` ，第二个参数是方法名 `SEL` 。所以获取方法的入参要从下标为 2 的地方开始获取。

`[self aspect_argumentAtIndex:idx]` 内部实现如下：
 
```Objective-C
// Thanks to the ReactiveCocoa team for providing a generic solution for this.
- (id)aspect_argumentAtIndex:(NSUInteger)index {
    const char *argType = [self.methodSignature getArgumentTypeAtIndex:index];
    // Skip const type qualifier.
    if (argType[0] == _C_CONST) argType++;
    
    #define WRAP_AND_RETURN(type) do { type val = 0; [self getArgument:&val atIndex:(NSInteger)index]; return @(val); } while (0)
    if (strcmp(argType, @encode(id)) == 0 || strcmp(argType, @encode(Class)) == 0) {
        __autoreleasing id returnObj;
        [self getArgument:&returnObj atIndex:(NSInteger)index];
        return returnObj;
    } else if (strcmp(argType, @encode(SEL)) == 0) {
        SEL selector = 0;
        [self getArgument:&selector atIndex:(NSInteger)index];
        return NSStringFromSelector(selector);
    } else if (strcmp(argType, @encode(Class)) == 0) {
        __autoreleasing Class theClass = Nil;
        [self getArgument:&theClass atIndex:(NSInteger)index];
        return theClass;
        // Using this list will box the number with the appropriate constructor, instead of the generic NSValue.
    } else if (strcmp(argType, @encode(char)) == 0) {
        WRAP_AND_RETURN(char);
    } else if (strcmp(argType, @encode(int)) == 0) {
        WRAP_AND_RETURN(int);
    } else if (strcmp(argType, @encode(short)) == 0) {
        WRAP_AND_RETURN(short);
    } else if (strcmp(argType, @encode(long)) == 0) {
        WRAP_AND_RETURN(long);
    } else if (strcmp(argType, @encode(long long)) == 0) {
        WRAP_AND_RETURN(long long);
    } else if (strcmp(argType, @encode(unsigned char)) == 0) {
        WRAP_AND_RETURN(unsigned char);
    } else if (strcmp(argType, @encode(unsigned int)) == 0) {
        WRAP_AND_RETURN(unsigned int);
    } else if (strcmp(argType, @encode(unsigned short)) == 0) {
        WRAP_AND_RETURN(unsigned short);
    } else if (strcmp(argType, @encode(unsigned long)) == 0) {
        WRAP_AND_RETURN(unsigned long);
    } else if (strcmp(argType, @encode(unsigned long long)) == 0) {
        WRAP_AND_RETURN(unsigned long long);
    } else if (strcmp(argType, @encode(float)) == 0) {
        WRAP_AND_RETURN(float);
    } else if (strcmp(argType, @encode(double)) == 0) {
        WRAP_AND_RETURN(double);
    } else if (strcmp(argType, @encode(BOOL)) == 0) {
        WRAP_AND_RETURN(BOOL);
    } else if (strcmp(argType, @encode(bool)) == 0) {
        WRAP_AND_RETURN(BOOL);
    } else if (strcmp(argType, @encode(char *)) == 0) {
        WRAP_AND_RETURN(const char *);
    } else if (strcmp(argType, @encode(void (^)(void))) == 0) {
        __unsafe_unretained id block = nil;
        [self getArgument:&block atIndex:(NSInteger)index];
        return [block copy];
    } else {
        NSUInteger valueSize = 0;
        NSGetSizeAndAlignment(argType, &valueSize, NULL);
        
        unsigned char valueBytes[valueSize];
        [self getArgument:valueBytes atIndex:(NSInteger)index];
        
        return [NSValue valueWithBytes:valueBytes objCType:argType];
    }
    return nil;
    #undef WRAP_AND_RETURN
}
```

`_C_CONST` 是一个常量，用来判断 encoding 的字符串是不是 CONST 常量。

```Objective-C

#define _C_ID       '@'
#define _C_CLASS    '#'
#define _C_SEL      ':'
#define _C_CHR      'c'
#define _C_UCHR     'C'
#define _C_SHT      's'
#define _C_USHT     'S'
#define _C_INT      'i'
#define _C_UINT     'I'
#define _C_LNG      'l'
#define _C_ULNG     'L'
#define _C_LNG_LNG  'q'
#define _C_ULNG_LNG 'Q'
#define _C_FLT      'f'
#define _C_DBL      'd'
#define _C_BFLD     'b'
#define _C_BOOL     'B'
#define _C_VOID     'v'
#define _C_UNDEF    '?'
#define _C_PTR      '^'
#define _C_CHARPTR  '*'
#define _C_ATOM     '%'
#define _C_ARY_B    '['
#define _C_ARY_E    ']'
#define _C_UNION_B  '('
#define _C_UNION_E  ')'
#define _C_STRUCT_B '{'
#define _C_STRUCT_E '}'
#define _C_VECTOR   '!'
#define _C_CONST    'r'
```

`WRAP_AND_RETURN` 是一个宏定义。这个宏定义里面调用的 `getArgument:atIndex:` 方法是用来在 `NSInvocation` 中根据 `index` 得到对应的`Argument`，最后 `return` 的时候把 `val` 包装成对象，返回出去。

再下面的 `if-else` 中很多字符传的比较，如 `strcmp(argType, @encode(id)) == 0` 牵扯到 一个叫 [Type Encoding](https://github.com/zhaoName/Notes/blob/master/iOS/TypeEncodings.md)的东西。判断参数类型，返回相应的结果。

如上代码依次判断了 `id，class，SEL`，接着是一大推基本类型，`char`、`int`、`short`、`long`、`long long`、`unsigned char`、`unsigned int`、`unsigned short`、`unsigned long`、`unsigned long long`、`float`、`double`、`BOOL`、`bool`、`char *`这些基本类型都会利用 `WRAP_AND_RETURN` 打包成对象返回。最后判断 block 和 struct 结构体，也会返回对应的对象。

假如 hook 了如下方法:

```Objective-C
- (NSArray *)testTypes:(NSObject *)obj isShow:(BOOL)isShow
{
    NSLog(@"%s", __func__);
    return  @[@1];
}
```

执行完 `aspects_arguments`，数组里面装的的是：
 
 ```Objective-C
(
    "<NSObject: 0x6000024b84c0>",
    1
)
 ```



**总结，AspectInfo里面主要是 NSInvocation 信息。将NSInvocation包装一层，比如参数信息等。**

<br>

## 二、


```Objective-C

```

```Objective-C

```

```Objective-C

```

<br>



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

参考：

