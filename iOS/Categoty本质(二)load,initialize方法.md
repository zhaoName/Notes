# Categoty本质(二)load、initialize方法

<br>


## 一、load方法

### 0x01 准备工作

新建`ZZPerson`并创建两个分类`ZZPerson+TestA.m`和`ZZPerson+TestB.m`，分别实现其`load`方法

```
// ZZPerson.m
+ (void)load
{
    NSLog(@"+[ZZPerson load]");
}

// ZZPerson+TestA.m
+ (void)load
{
    NSLog(@"+[ZZPerson(TestA) load]");
}

// ZZPerson+TestB.m
+ (void)load
{
    NSLog(@"+[ZZPerson(TestB) load]");
}
```

### 0x02 load方法什么时候调用？

这时候啥也不做，`ZZPerson`的头文件也不用导入，直接运行代码，看看会出现什么情况

```
2019-04-27 23:53:36.527709+0800 ZZCategory[50064:5855381] +[ZZPerson load]
2019-04-27 23:53:36.527937+0800 ZZCategory[50064:5855381] +[ZZPerson(TestA) load]
2019-04-27 23:53:36.527958+0800 ZZCategory[50064:5855381] +[ZZPerson(TestB) load]
```

控制台会有打印信息！！这说明 **`+load`方法是在程序将类或分类载进内存时就会自动调用。**

但是看到控制台的打印信息又有点疑惑！根据[Categary本质(一)底层实现](https://gitee.com/zhaoName0x01/Notes/blob/master/iOS/Categary本质(一)底层实现.md)所分析的结果，当分类中存在同样的方法，最终会调用哪个分类的方法，由分类的编译顺序决定，后编译的优先调用。那应该只会调用`ZZPerson+TestB`中的`+load`方法！但结果原类和分类中的`+load`都调用了，这是为啥呢？

想解释清楚这个问题，需要查看`runtime`中关于`+load`方法的底层实现！


### 0x03 load的底层实现

在`objc-os.mm`中找到`_objc_init()`

```
// runtime的初始化方法，也是runtime的入口函数
void _objc_init(void)
{
	......
    
	// map_images 中有合并分类的过程
	// load_images 中有加载+load方法的过程
    _dyld_objc_notify_register(&map_images, load_images, unmap_image);
}
```

在`objc-runtime-new.mm`中找到`load_images()`

```
void load_images(const char *path __unused, const struct mach_header *mh)
{
    ......
    // Discover load methods
    {
        mutex_locker_t lock2(runtimeLock);
        // 从源文件中读取所有类和分类的+load方法 组成结构体loadable_class 或 loadable_category
        // 为+load方法调用做准备
        prepare_load_methods((const headerType *)mh);
    }

    // Call +load methods (without runtimeLock - re-entrant)
    // 调用类和分类中的+load方法
    call_load_methods();
}
```

- `prepare_load_methods()`

```
// 从源文件中读取所有类和分类的+load方法 组成结构体loadable_class 或 loadable_category, 再将结构体放在数组中
void prepare_load_methods(const headerType *mhdr)
{
    size_t count, i;
    
    runtimeLock.assertLocked();
    // 从源文件中获取所有类，并遍历
    classref_t *classlist =
    _getObjc2NonlazyClassList(mhdr, &count);
    for (i = 0; i < count; i++) {
        // 按 superclass -> class 的顺序将cls和+load对应起来 存在数组里
        schedule_class_load(remapClass(classlist[i]));
    }
    
    // 从源文件中获取所有分类， 并遍历
    category_t **categorylist = _getObjc2NonlazyCategoryList(mhdr, &count);
    for (i = 0; i < count; i++) {
        category_t *cat = categorylist[i];
        Class cls = remapClass(cat->cls);
        if (!cls) continue;  // category for ignored weak-linked class
        realizeClass(cls);
        assert(cls->ISA()->isRealized());
        // 将分类和分类的+load方法对应起来(struct loadable_category) 放到数组中
        add_category_to_loadable_list(cat);
    }
}

// 按 superclass -> class 的顺序将cls和+load对应起来 存在数组里
static void schedule_class_load(Class cls)
{
	......
    // Ensure superclass-first ordering
    // 递归调用 如果父类存在，优先添加父类的+load方法
    schedule_class_load(cls->superclass);
    
    // class和+load方法对应起来(struct loadable_class) 放在数组里
    add_class_to_loadable_list(cls);
    cls->setInfo(RW_LOADED); 
}

// class和+load方法对应起来(struct loadable_class) 放在数组里
void add_class_to_loadable_list(Class cls)
{
    ......
    // loadable_classes扩容
	if (loadable_classes_used == loadable_classes_allocated) {
        loadable_classes_allocated = loadable_classes_allocated*2 + 16;
        loadable_classes = (struct loadable_class *)
            realloc(loadable_classes, loadable_classes_allocated * sizeof(struct loadable_class));
    }
	    
    // 将cls 和+load方法对应起来 存在loadable_classes数组中
    // loadable_classes 中是 struct loadable_class 结构体
    loadable_classes[loadable_classes_used].cls = cls;
    loadable_classes[loadable_classes_used].method = method;
    // 子类和父类的总个数
    loadable_classes_used++;
}

// 将分类和分类的+load方法对应起来(struct loadable_category) 放到数组中
void add_category_to_loadable_list(Category cat)
{
    ...
    // 得到分类的 +load 方法
    method = _category_getLoadMethod(cat);
    
    // loadable_categories扩容
    if (loadable_categories_used == loadable_categories_allocated) {
        loadable_categories_allocated = loadable_categories_allocated*2 + 16;
        loadable_categories = (struct loadable_category *)
            realloc(loadable_categories, loadable_categories_allocated * sizeof(struct loadable_category));
    }
    
	// 将 cat 和 +load 方法对应起来放在 loadable_category 结构体中，然后放在loadable_categories数组中
    loadable_categories[loadable_categories_used].cat = cat;
    loadable_categories[loadable_categories_used].method = method;
    // category的个数
    loadable_categories_used++;
}
```

- `call_load_methods()`

```
// objc-loadmethod.mm

// 调用prepare_load_methods()中准备好的 类和分类中的+load方法
// 优先调用父类的+load方法, 在调用子类的+load方法
// 直到类中的+load方法调用完成，才会调用分类的+load方法
void call_load_methods(void)
{
	......
    do {
        // 1. Repeatedly call class +loads until there aren't any more
        while (loadable_classes_used > 0) {
        	// 调用类中的 +load 方法
            call_class_loads();
        }
        // 2. Call category +loads ONCE
        // 调用分类中的 +load 方法
        more_categories = call_category_loads();

        // 3. Run more +loads if there are classes OR more untried categories
    } while (loadable_classes_used > 0  ||  more_categories);
    ......
}

// 调用prepare_load_methods()中准备好的类的 +load方法
// 若有新的类 loadable，不会再为他们调动+load方法
static void call_class_loads(void)
{
    // Detach current loadable list.
    // 取出prepare_load_methods()中的数组loadable_classes
    struct loadable_class *classes = loadable_classes;
    int used = loadable_classes_used;
    loadable_classes = nil;
    loadable_classes_allocated = 0;
    loadable_classes_used = 0;
    
    // Call all +loads for the detached list.
    for (i = 0; i < used; i++) {
    	// 从结构体中取出cls 和 +load
        Class cls = classes[i].cls;
        load_method_t load_method = (load_method_t)classes[i].method;
        if (!cls) continue; 

        if (PrintLoading) {
            _objc_inform("LOAD: +[%s load]\n", cls->nameForLogging());
        }
        // 拿到类的 +load 方法地址 直接调用
        (*load_method)(cls, SEL_load);
    }
    
    // Destroy the detached list.
    if (classes) free(classes);
}


// 调用prepare_load_methods()中准备好的分类的 +load方法
// 若有新的分类变成 loadable，则其 +load方法不会别调用，会将其放在loadable_categories最后 并返回true
static bool call_category_loads(void)
{
    int i, shift;
    bool new_categories_added = NO;
    
    // Detach current loadable list.
    // 取出prepare_load_methods()中的数组loadable_classes
    struct loadable_category *cats = loadable_categories;
    int used = loadable_categories_used;
    int allocated = loadable_categories_allocated;
    loadable_categories = nil;
    loadable_categories_allocated = 0;
    loadable_categories_used = 0;

    // Call all +loads for the detached list.
    // 遍历loadable_classes
    for (i = 0; i < used; i++) {
        Category cat = cats[i].cat;
        load_method_t load_method = (load_method_t)cats[i].method;
        Class cls;
        if (!cat) continue;
		
		// 获取分类对应的类
        cls = _category_getClass(cat);
        if (cls  &&  cls->isLoadable()) {
            if (PrintLoading) {
                _objc_inform("LOAD: +[%s(%s) load]\n", cls->nameForLogging(), _category_getName(cat));
            }
            // 拿到分类的 +load 方法地址 直接调用
            (*load_method)(cls, SEL_load);
            cats[i].cat = nil;
        }
    }
    ......
}
```

这样我们就清楚为什么类和分类中的`+load`方法都调用了！

因为`+ load`方法的调用并不是走`objc_msgSend`机制，它是直接找到类或分类的`+load`方法的地址，进行调用。

### 0x04 验证有继承关系时`load`方法调用顺序

新键`ZZStudent`继承自`ZZPerson`,也给其添加两个分类

```
// ZZStudent.m
+ (void)load
{
    NSLog(@"+[ZZStudent load]");
}

// ZZStudent+TestA.m
+ (void)load
{
    NSLog(@"+[ZZStudent(Add1) load]");
}

// ZZStudent+TestB.m
+ (void)load
{
    NSLog(@"+[ZZStudent(Add2) load]");
}
```

打印信息

```
2019-04-28 22:30:08.601849+0800 ZZCategory[96937:6529221] +[ZZPerson load]
2019-04-28 22:30:08.602060+0800 ZZCategory[96937:6529221] +[ZZStudent load]
2019-04-28 22:30:08.602072+0800 ZZCategory[96937:6529221] +[ZZStudent(Add1) load]
2019-04-28 22:30:08.602080+0800 ZZCategory[96937:6529221] +[ZZStudent(Add2) load]
2019-04-28 22:30:08.602087+0800 ZZCategory[96937:6529221] +[ZZPerson(TestA) load]
2019-04-28 22:30:08.602094+0800 ZZCategory[96937:6529221] +[ZZPerson(TestB) load]
```

**+load 总结**

- `+load`方法会在`runtime`加载类、分类的时候自动调用

- 每个类、分类的`+load`方法只会调用一次，且是直接找到`+load`方法内存地址进行调用

- `+load`方法调用顺序

	- 优先调用类的`+load`方法，再调用分类的`+load`方法
	
	- 对于所有类来说按照编译顺序调用，对于某个类来说优先调用父类的`+load`方法
	
	- 对于分类来说，`+load`方法的调用顺序和分类的编译顺序一致

## 二、initalize方法


### 0x01 准备

新建`ZZPerson`和其子类`ZZStudent`,并分别为其创建两个分类。然后实现`+initialize`方法

```
// ZZPerson.m
+ (void)initialize
{
    NSLog(@"+[ZZPerson initialize]");
}

// ZZPerson+TestA.m
+ (void)initialize
{
    NSLog(@"+[ZZPerson(TestA) initialize]");
}

// ZZPerson+TestB.m
+ (void)initialize
{
    NSLog(@"+[ZZPerson(TestB) initialize]");
}

// ZZStudent.m
+ (void)initialize
{
    NSLog(@"+[ZZStudent initialize]");
}

// ZZStudent+TestA.m
+ (void)initialize
{
    NSLog(@"+[ZZStudent(Add1) initialize]");
}

// ZZStudent+TestB.m
+ (void)initialize
{
    NSLog(@"+[ZZStudent(Add2) initialize]");
}
```

### 0x02 `+initialize`什么时候调用？

```
[ZZPerson alloc];
[ZZPerson alloc];
// 打印信息
2019-04-28 23:16:50.813674+0800 ZZCategory[97528:6796881] +[ZZPerson(TestB) initialize]

[ZZStudent alloc];
[ZZStudent alloc];
// 打印信息
2019-04-28 23:17:29.714468+0800 ZZCategory[97544:6800754] +[ZZPerson(TestB) initialize]
2019-04-28 23:17:29.714832+0800 ZZCategory[97544:6800754] +[ZZStudent(Add2) initialize]
```

当给父类`ZZPerson`发消息时，只有分类的`+initialize`调用的且只调用一次。

当给子类`ZZStudent`发消息时，先调用父类分类中的`+initialize`方法，再调用子类分类中的`+initialize`方法，只调用一次。

这是为啥呢？


### 0x03 `+initialize`底层实现


`+initialize`底层实现查找顺序：`class_getClassMethod()` -> `class_getInstanceMethod()` -> `lookUpImpOrNil()` -> `lookUpImpOrForward()` -> `_class_initialize()` -> `callInitialize()`。

-  `lookUpImpOrForward()`

```
IMP lookUpImpOrForward(Class cls, SEL sel, id inst, bool initialize, bool cache, bool resolver)
{
    ......
    // 若需要initialize 且类没有被initialize过
    if (initialize  &&  !cls->isInitialized()) {
        runtimeLock.unlock();
        // 初始化类
        _class_initialize (_class_getNonMetaClass(cls, inst));
        runtimeLock.lock();
    }
    ......
}
```

- `_class_initialize()`


```
void _class_initialize(Class cls)
{
    assert(!cls->isMetaClass());

    Class supercls;
    bool reallyInitialize = NO;

    // Make sure super is done initializing BEFORE beginning to initialize cls.
    // See note about deadlock above.
    supercls = cls->superclass;
    
    // 若父类存在 且父类没有被initialize过
    if (supercls  &&  !supercls->isInitialized()) {
    	// 递归调用 优先初始化父类
        _class_initialize(supercls);
    }
    ......
    
    // 调用+initialize方法
    callInitialize(cls);
}
```

- `callInitialize()`

```
void callInitialize(Class cls)
{
    ((void(*)(Class, SEL))objc_msgSend)(cls, SEL_initialize);
    asm("");
}
```

通过分析可知调用`+initialize`方法时，是通过`objc_msgSend()`函数调用的。这也就解释了上面为什么会调用分类的`+initialize`方法。

**`+initialize` 总结**

- `+initialize`方法会在类第一次接收到消息时调用

- 优先调用父类的`+initialize`方法，再调用子类的`+initialize`方法

- 如果子类没有实现`+initialize`，会调用父类的`+initialize`(所以父类的`+initialize`可能会被调用多次）

- 若分类实现了`+initialize`，就覆盖类本身的`+initialize`调用


**相关内容**

- [Categary本质(一)底层实现](https://gitee.com/zhaoName0x01/Notes/blob/master/iOS/Categary本质(一)底层实现.md)

- [Category本质(三)关联对象](https://gitee.com/zhaoName0x01/Notes/blob/master/iOS/Category本质(三)关联对象.md)

<br>

写于2019-04-27

<br>