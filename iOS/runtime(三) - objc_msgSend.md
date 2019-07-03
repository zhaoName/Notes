
# runtime(三) - objc_msgSend

<br>


我们都知道`OC`中的方法调用在底层都会转成`objc_msgSend `，本篇文章主要从三大块解释`objc_msgSend `调用流程。这样也和前面所将内容串起来。


## 一、消息发送

消息发送 你可以认为是根据消息接收者查找要调用方法的过程。被调用的实例(类)方法可能在方法缓存里([runtime(二) - 方法缓存](https://github.com/zhaoName/Notes/blob/master/iOS/runtime(%E4%BA%8C)%20-%20%E6%96%B9%E6%B3%95%E7%BC%93%E5%AD%98.md))，也能在`class`(`meta-class`)对象或其父类中([OC对象本质(二)](https://github.com/zhaoName/Notes/blob/master/iOS/OC%E5%AF%B9%E8%B1%A1%E6%9C%AC%E8%B4%A8(%E4%BA%8C).md))。具体查找流程需要查看`runtime`源码。


### 0x01 方法缓存中查找

在`objc4-750`中搜索`objc_msgSend`，你会发现最终在`objc-msg-arm64.s`中找到`_objc_msgSend `的汇编实现。


```
ENTRY _objc_msgSend
	UNWIND _objc_msgSend, NoFrame

    // nil check and tagged pointer check
    // p0 中存放的是方法的第一个参数也就是消息接收者，也就是实例对象 或类对象
    // 当其为空时 直接 return, 这也就解释了 [nil xxx] 为什么不会崩溃
	cmp	p0, #0
	
	// (MSB tagged pointer looks negative)
	b.le	LNilOrTagged		
	
    // p13 = isa
    // 将消息接收者的 isa 放在寄存器 p13
	ldr	p13, [x0]
	
    // p16 = class
    // 由 isa 找到对一个的类对象
	GetClassFromIsa_p16 p13
	
LGetIsaDone:
    // calls imp or objc_msgSend_uncached
    // 查找缓存，找到就 calls imp，没找到就 objc_msgSend_uncached
    // 记住 NORMAL
	CacheLookup NORMAL
	......
END_ENTRY _objc_msgSend
```

上面是对消息接收者防空判断，并将`isa`和对应的`calss`对象放到相应的寄存器中。然后去`CacheLookup NORMAL`中查找缓存。

```
.macro CacheLookup
	// p1 = SEL, p16 = isa
    // p10 = buckets, p11 = occupied|mask
	ldp	p10, p11, [x16, #CACHE]
#if !__LP64__
	and	w11, w11, 0xffff	// p11 = mask
#endif
    // x12 = _cmd & mask
	and	w12, w1, w11
    // p12 = buckets + ((_cmd & mask) << (1+PTRSHIFT))
	add	p12, p10, p12, LSL #(1+PTRSHIFT)
	......
	
    // {imp, sel} = *bucket
	ldp	p17, p9, [x12]
    // if (bucket->sel != _cmd)
1:	cmp	p9, p1
    // scan more
    // 方法名不同 则遍历下一个方法缓存中的元素
	b.ne  2f
    // call or return imp
    // 在方法缓存中找到 imp
	CacheHit $0
	
2:	// not hit: p12 = not-hit bucket
    // miss if bucket->sel == 0
	CheckMiss $0
    // wrap if bucket == buckets
	cmp	p12, p10
	b.eq	3f
    // {imp, sel} = *--bucket
    // 循环遍历方法缓存
    // objc-cache.mm 中的 bucket_t * cache_t::find(cache_key_t k, id receiver)
	ldp	p17, p9, [x12, #-BUCKET_SIZE]!
    // loop
	b	1b

3:	// double wrap
	JumpMiss $0
	
.endmacro
```

上面就是从方法缓存中查找被调用方法的汇编实现过程。最后分两种情况

- 找到就调用`CacheHit $0`，直接返回去掉用方法

- 循环遍历后没找到调用`CheckMiss $0`，去方法列表中查找。


```
.macro CheckMiss
	// miss if bucket->sel == 0
.if $0 == GETIMP
	cbz	p9, LGetImpMiss
.elseif $0 == NORMAL
    // 记得上面 CacheLookup NORMAL， 所以走这个判断
	cbz	p9, __objc_msgSend_uncached
......
.endmacro


STATIC_ENTRY __objc_msgSend_uncached
UNWIND __objc_msgSend_uncached, FrameWithNoSaves

// THIS IS NOT A CALLABLE C FUNCTION
// Out-of-band p16 is the class to search
// 名字起的很贴切了  方法列表中查找
MethodTableLookup
TailCallFunctionPointer x17

END_ENTRY __objc_msgSend_uncached


.macro MethodTableLookup
	......
	
	// receiver and selector already in x0 and x1
	mov	x2, x16
	bl	__class_lookupMethodAndLoadCache3
	
	......
.endmacro
```

到`__class_lookupMethodAndLoadCache3 `汇编代码结束。也就是说从方法列表中查找对应方法时`C/C++`实现的。


### 0x02 方法列表中查找

在`objc4-750`中搜索`__class_lookupMethodAndLoadCache3 `发现只有`***.s`文件中能找到，在`C/C++`代码中没有这个函数。

这是因为`C/C++`中的函数在转成汇编调用时，函数名会多一个下划线如`main -> _main`。所以我们要搜索`_class_lookupMethodAndLoadCache3 `，就会在`objc-runtime-new.mm`中找到其具体实现。


```
// objc-runtime-new.mm

IMP _class_lookupMethodAndLoadCache3(id obj, SEL sel, Class cls)
{
    return lookUpImpOrForward(cls, sel, obj,  YES/*initialize*/, NO/*cache*/, YES/*resolver*/);
}


IMP lookUpImpOrForward(Class cls, SEL sel, id inst,  bool initialize, bool cache, bool resolver)
{
    IMP imp = nil;
    bool triedResolver = NO;
    ......

 retry:    
    runtimeLock.assertLocked();
    // Try this class's cache.
    imp = cache_getImp(cls, sel);
    if (imp) goto done;
    
    // Try this class's method lists.
    {
        // 在本类的方法列表中查找
        Method meth = getMethodNoSuper_nolock(cls, sel);
        if (meth) {
            // 找到就添加到当前类的方法缓存中
            log_and_fill_cache(cls, meth->imp, sel, inst, cls);
            imp = meth->imp;
            goto done;
        }
    }

    // Try superclass caches and method lists.
    {
        // 在父类的缓存和方法列表中查找
        unsigned attempts = unreasonableClassCount();
        for (Class curClass = cls->superclass; curClass != nil; curClass = curClass->superclass)
        {
            // Halt if there is a cycle in the superclass chain.
            if (--attempts == 0) {
                _objc_fatal("Memory corruption in class list.");
            }
            
            // Superclass cache.
            imp = cache_getImp(curClass, sel);
            if (imp) {
                if (imp != (IMP)_objc_msgForward_impcache) {
                    // Found the method in a superclass. Cache it in this class.
                    // 在父类缓存中找到的方法 也缓存到本类的缓存中
                    log_and_fill_cache(cls, imp, sel, inst, curClass);
                    goto done;
                }
                else { break;}
            }
            
            // Superclass method list.
            // 在父类的方法列表中查找
            Method meth = getMethodNoSuper_nolock(curClass, sel);
            if (meth) {
                // 找到也缓存到本类中
                log_and_fill_cache(cls, meth->imp, sel, inst, curClass);
                imp = meth->imp;
                goto done;
            }
        }
    }

    // No implementation found. Try method resolver once.
    // 缓存和方法列表中都没找到且未曾动态解析 resolveClassMethod 和 resolveInstanceMethod
    // 这也叫动态方法解析 下面章节会讲到
    if (resolver  &&  !triedResolver) {
        runtimeLock.unlock();
        _class_resolveMethod(cls, sel, inst);
        runtimeLock.lock();
        // Don't cache the result; we don't hold the lock so it may have 
        // changed already. Re-do the search from scratch instead.
        triedResolver = YES;
        goto retry;
    }

    // No implementation found, and method resolver didn't help. 
    // Use forwarding.
    // 是否实现 forwardingTargetForSelector 和 forwardInvocation
    // 这也叫消息转发 下面章节会讲到
    imp = (IMP)_objc_msgForward_impcache;
    cache_fill(cls, sel, imp, inst);

 done:
    runtimeLock.unlock();

    return imp;
}
```

我们看出跳出汇编代码查找方法的过程大致如下

- 先去本类的方法缓存中查找(因为在此期间可能在别的函数将要找的方法添加方法缓存中)

- 在本类的方法列表中查找，找到就添加到当前类的方法缓存中

- 按照先父类缓存再父类方法列表的先后顺序遍历父类，找到还是添加到当前类的方法缓存中

- 动态方法解析 `resolveClassMethod` 和 `resolveInstanceMethod`

- 消息转发 `forwardingTargetForSelector` 和 `forwardInvocation`


## 二、动态方法解析

当从方法缓存和方法列表(当前类、父类)中都没找到对应的方法，系统还会给两次机会去继续调用方法。我们先看第一次机会。

```
IMP lookUpImpOrForward(Class cls, SEL sel, id inst, bool initialize, bool cache, bool resolver)
{
    ......
    // No implementation found. Try method resolver once.
    // 缓存和方法列表中都没找到且未曾动态解析 resolveClassMethod 和 resolveInstanceMethod
    if (resolver  &&  !triedResolver) {
        runtimeLock.unlock();
        _class_resolveMethod(cls, sel, inst);
        runtimeLock.lock();
        // Don't cache the result; we don't hold the lock so it may have 
        // changed already. Re-do the search from scratch instead.
        triedResolver = YES;
        goto retry;
    }
    ......
}

// 
void _class_resolveMethod(Class cls, SEL sel, id inst)
{
    if (! cls->isMetaClass()) {
        // 类对象 实例方法
        // try [cls resolveInstanceMethod:sel]
        _class_resolveInstanceMethod(cls, sel, inst);
    } 
    else {
        // meta-class 对象 类方法
        // try [nonMetaClass resolveClassMethod:sel] and [cls resolveInstanceMethod:sel]
        _class_resolveClassMethod(cls, sel, inst);
        if (!lookUpImpOrNil(cls, sel, inst, NO/*initialize*/, YES/*cache*/, NO/*resolver*/)) 
        {
            _class_resolveInstanceMethod(cls, sel, inst);
        }
    }
}

// 只写_class_resolveInstanceMethod，_class_resolveClassMethod和其类似
static void _class_resolveInstanceMethod(Class cls, SEL sel, id inst)
{
    if (! lookUpImpOrNil(cls->ISA(), SEL_resolveInstanceMethod, cls, NO/*initialize*/, YES/*cache*/, NO/*resolver*/)) {
        // Resolver not implemented.
        // 没实现 resolveInstanceMethod 方法就直接返回
        return;
    }

    BOOL (*msg)(Class, SEL, SEL) = (typeof(msg))objc_msgSend;
    bool resolved = msg(cls, SEL_resolveInstanceMethod, sel);

    // Cache the result (good or bad) so the resolver doesn't fire next time.
    // +resolveInstanceMethod adds to self a.k.a. cls
    IMP imp = lookUpImpOrNil(cls, sel, inst,  NO/*initialize*/, YES/*cache*/, NO/*resolver*/);
	......
}
```

上述就是动态方法解析的底层实现，流程大致如下：

- 判断是否已经动态解析过，若已经解析过则跳过

- 为解析则调用`+resolveInstanceMethod:`或者`+resolveClassMethod:`方法来动态解析方法

- 标记已经动态解析过

- 重新执行消息发送(除了汇编部分)


在`OC`中动态添加实例方法或类方法

```
// ZNPerson.m
void c_testObject(id self, SEL sel)
{
    NSLog(@"c_testObject:%@ %s", self, sel);
}

- (void)testObject
{
    NSLog(@"%s", __func__);
}

+ (BOOL)resolveInstanceMethod:(SEL)sel
{
    if (sel == @selector(testPerson)) {
        //Method mm = class_getInstanceMethod(self, @selector(testObject));
        //class_addMethod(self, sel, method_getImplementation(mm), method_getTypeEncoding(mm));
        // 转成 c 语言函数
        class_addMethod(self, sel, c_testObject, "v16@0:8");
        return YES;
    }
    return [super resolveInstanceMethod:sel];
}


+ (BOOL)resolveClassMethod:(SEL)sel
{
    if (sel == @selector(testClassMethod)) {
        Method mm = class_getInstanceMethod(self, @selector(testObject));
        class_addMethod(object_getClass(self), sel, method_getImplementation(mm), method_getTypeEncoding(mm));
        return YES;
    }
    return [super resolveClassMethod:sel];
}

// ViewController.m
- (void)viewDidLoad {
    [super viewDidLoad];
    
    ZNPerson *per = [[ZNPerson alloc] init];
    [per testPerson];
    [ZNPerson testClassMethod];
}

// 打印结果
2019-07-02 21:55:16.714777 runtime-isa[1182:466653] c_testObject:<ZNPerson: 0x170010750> testPerson
2019-07-02 21:55:16.714885 runtime-isa[1182:466653] -[ZNPerson testObject]
```


## 三、消息转发

当消息发送和动态方法解析都未成功，会进入消息转发阶段。

```
IMP lookUpImpOrForward(Class cls, SEL sel, id inst, bool initialize, bool cache, bool resolver)
{
    ......
    // No implementation found, and method resolver didn't help. 
    // Use forwarding.
    // 是否实现 forwardingTargetForSelector 和 forwardInvocation
    imp = (IMP)_objc_msgForward_impcache;
    cache_fill(cls, sel, imp, inst);
    ......
}

// objc-msg-arm64.s
STATIC_ENTRY __objc_msgForward_impcache

// No stret specialization.
b	__objc_msgForward
END_ENTRY __objc_msgForward_impcache


ENTRY __objc_msgForward
adrp	x17, __objc_forward_handler@PAGE
ldr	p17, [x17, __objc_forward_handler@PAGEOFF]
TailCallFunctionPointer x17
	
END_ENTRY __objc_msgForward
```

接下来的代码苹果就不开源了，外国有个大佬一步步调试汇编，将接下来的代码反汇编出来

```
// 伪代码
int __forwarding__(void *frameStackPointer, int isStret) {
    id receiver = *(id *)frameStackPointer;
    SEL sel = *(SEL *)(frameStackPointer + 8);
    const char *selName = sel_getName(sel);
    Class receiverClass = object_getClass(receiver);

    // 调用 forwardingTargetForSelector:
    if (class_respondsToSelector(receiverClass, @selector(forwardingTargetForSelector:))) {
        id forwardingTarget = [receiver forwardingTargetForSelector:sel];
        if (forwardingTarget && forwardingTarget != receiver) {
            if (isStret == 1) {
                int ret;
                objc_msgSend_stret(&ret,forwardingTarget, sel, ...);
                return ret;
            }
            return objc_msgSend(forwardingTarget, sel, ...);
        }
    }

    // 僵尸对象
    const char *className = class_getName(receiverClass);
    const char *zombiePrefix = "_NSZombie_";
    size_t prefixLen = strlen(zombiePrefix); // 0xa
    if (strncmp(className, zombiePrefix, prefixLen) == 0) {
        CFLog(kCFLogLevelError,
              @"*** -[%s %s]: message sent to deallocated instance %p",
              className + prefixLen,
              selName,
              receiver);
        <breakpoint-interrupt>
    }

    // 调用 methodSignatureForSelector 获取方法签名后再调用 forwardInvocation
    if (class_respondsToSelector(receiverClass, @selector(methodSignatureForSelector:))) {
        NSMethodSignature *methodSignature = [receiver methodSignatureForSelector:sel];
        if (methodSignature) {
            BOOL signatureIsStret = [methodSignature _frameDescriptor]->returnArgInfo.flags.isStruct;
            if (signatureIsStret != isStret) {
                CFLog(kCFLogLevelWarning ,
                      @"*** NSForwarding: warning: method signature and compiler disagree on struct-return-edness of '%s'.  Signature thinks it does%s return a struct, and compiler thinks it does%s.",
                      selName,
                      signatureIsStret ? "" : not,
                      isStret ? "" : not);
            }
            if (class_respondsToSelector(receiverClass, @selector(forwardInvocation:))) {
                NSInvocation *invocation = [NSInvocation _invocationWithMethodSignature:methodSignature frame:frameStackPointer];

                [receiver forwardInvocation:invocation];

                void *returnValue = NULL;
                [invocation getReturnValue:&value];
                return returnValue;
            } else {
                CFLog(kCFLogLevelWarning ,
                      @"*** NSForwarding: warning: object %p of class '%s' does not implement forwardInvocation: -- dropping message",
                      receiver,
                      className);
                return 0;
            }
        }
    }

    SEL *registeredSel = sel_getUid(selName);

    // selector 是否已经在 Runtime 注册过
    if (sel != registeredSel) {
        CFLog(kCFLogLevelWarning ,
              @"*** NSForwarding: warning: selector (%p) for message '%s' does not match selector known to Objective C runtime (%p)-- abort",
              sel,
              selName,
              registeredSel);
    } // doesNotRecognizeSelector
    else if (class_respondsToSelector(receiverClass,@selector(doesNotRecognizeSelector:))) {
        [receiver doesNotRecognizeSelector:sel];
    }
    else {
        CFLog(kCFLogLevelWarning ,
              @"*** NSForwarding: warning: object %p of class '%s' does not implement doesNotRecognizeSelector: -- abort",
              receiver,
              className);
    }

    // The point of no return.
    kill(getpid(), 9);
}
```

消息转发大致流程如下：

- 调用`forwardingTargetForSelector:`方法，若返回值不为`nil`，则`objc_msgSend(返回值, SEL)`

- 若返回值为`nil`，调用`methodSignatureForSelector:`方法，若返回值不为`nil`，则调用`forwardInvocation:`方法，在`forwardInvocation:`里你可以做任何操作

- 若返回值为`nil`，则调用`doesNotRecognizeSelector:`方法，程序崩溃。

**注意：上面三个方法前面可以使`+`也可以是`-`，由方法类型决定。**

在`OC`中分别实现上述三个方法

- `forwardingTargetForSelector:`

```
// ZNPerson.m
- (id)forwardingTargetForSelector:(SEL)aSelector
{
    if (aSelector == @selector(testPerson)) {
        // objc_msgSeng([[ZNGirl alloc] init], aSelector)
        return [[ZNGirl alloc] init];
    }
    return [super forwardingTargetForSelector:aSelector];
}

// ZNGirl.m
- (void)testPerson
{
    NSLog(@"%s", __func__);
}

// ViewController.m
- (void)viewDidLoad {
    [super viewDidLoad];
    
    ZNPerson *per = [[ZNPerson alloc] init];
    [per testPerson];
}

// 打印结果
2019-07-02 23:29:03.442608 runtime-isa[1254:479161] -[ZNGirl testPerson]
```

这样就将属于`ZNPerson`的消息，转发给了`ZNGirl`。


- `methodSignatureForSelector:`和`forwardInvocation:`

```
// ZNPerson.m
- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector
{
    if (aSelector == @selector(testPerson:)) {
        // 注意 "v16@0:8" 一定要和 testPerson 的 typeEncoding 一样
        return [NSMethodSignature signatureWithObjCTypes:"v16@0:8@16"];
    }
    return [super methodSignatureForSelector:aSelector];
}

// anInvocation 包装了 methodSignatureForSelector: 中返回的 TypeEncodings
- (void)forwardInvocation:(NSInvocation *)anInvocation
{
    NSString *str = nil;
    [anInvocation getArgument:&str atIndex:2];
    NSLog(@"%@", str);
}

// ViewController.m
- (void)viewDidLoad {
    [super viewDidLoad];
    
   	 ZNPerson *per = [[ZNPerson alloc] init];
    [per testPerson:@"----testPerson-----"];
}

// 打印结果
2019-07-02 23:43:38.813649 runtime-isa[1271:481872] ----testPerson-----
```

![](https://images.gitee.com/uploads/images/2019/0703/094811_98866de2_1355277.png "msgSend.png")


<br>

写于2019-07-02

<br>