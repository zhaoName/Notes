# Category - 关联对象

<br>

在`OC`中一个"正常的类"即可以添加成员变量，也可以使用`@property`声明属性。并且声明属性编译器会帮我们干三件事。

- 生成一个带下划线的同名成员变量`_key`

- 生成`setter getter`方法的声明

- 生成`setter getter`方法的实现

分类的底层结构决定分类不能添加成员变量，可以添加属性。但在分类中添加属性，编译器只会干一件事 -- 生成`setter getter`方法的声明。那我们怎么做才能让在分类中声明的属性像"正常类"中声明的属性一样正常使用呢？ 这就需要用到`runtime`中的关联对象

## 一、使用

### 0x01 常用API

```Objective-C
// 添加关联对象
objc_setAssociatedObject(id object, const void *key, id value, objc_AssociationPolicy policy)

// 获取关联对象
objc_getAssociatedObject(id object, const void *key)

// 移除所有的关联对象
objc_removeAssociatedObjects(id object)
```

### 0x02 用法

给`NSObject`添加的`name`属性

```Objective-C
// NSObject+Add.h
@property (nonatomic, strong) NSString *name;

// NSObject+Add.m
#import <objc/runtime.h>

static const char *NameKey = "NameKey";

- (void)setName:(NSString *)name
{
    objc_setAssociatedObject(self, NameKey, name, OBJC_ASSOCIATION_COPY_NONATOMIC);
}

- (NSString *)name
{
    return objc_getAssociatedObject(self, NameKey);
}
```

但这样每次声明属性都要维护写一个类似`static const char *NameKey = "NameKey";`这样的`key `很麻烦！我们改进下

```Objective-C
// 使用属性的 getter 方法的 @selector 作为 key，既能保证唯一又不用维护
- (void)setName:(NSString *)name
{
    objc_setAssociatedObject(self, @selector(name), name, OBJC_ASSOCIATION_COPY_NONATOMIC);
}

- (NSString *)name
{
    return objc_getAssociatedObject(self, @selector(name));
}
```

### 0x03 objc_AssociationPolicy

`objc_AssociationPolicy`指的是使用什么策略关联对象。相当于声明属性时使用`strong`之类的修饰符。

![](../Images/iOS/Category/AssociationObject_image0301.png)

## 二、原理

### 0x01 原理解释

在`objc4-750`的`objc-runtime.mm`找到`objc_setAssociatedObject()`的实现.

```Objective-C
// objc-runtime.mm
void objc_setAssociatedObject(id object, const void *key, id value, objc_AssociationPolicy policy) {
    _object_set_associative_reference(object, (void *)key, value, policy);
}

// objc-references.mm
void _object_set_associative_reference(id object, void *key, id value, uintptr_t policy) 
{
    // retain the new value (if any) outside the lock.
    ObjcAssociation old_association(0, nil);
    id new_value = value ? acquireValue(value, policy) : nil;
    {
        AssociationsManager manager;
        // 取出 AssociationsManager 中的 AssociationsHashMap
        AssociationsHashMap &associations(manager.associations());
        // 将 object 包装成 disguised_ptr_t 类型的值
        disguised_ptr_t disguised_object = DISGUISE(object);
        
        // 若传进来的 value 不为空 且经过 acquireValuev() 转换的值不为空
        if (new_value) {
            // break any existing association.
            AssociationsHashMap::iterator i = associations.find(disguised_object);
            
            if (i != associations.end()) {
                // object对应的 ObjectAssociationMap 已存在，则遍历 ObjectAssociationMap 内部
                ObjectAssociationMap *refs = i->second;
                ObjectAssociationMap::iterator j = refs->find(key);
                if (j != refs->end()) 
                {
                    old_association = j->second;
                    // key 对应的 ObjcAssociation 已存在 则覆盖原有的值
                    j->second = ObjcAssociation(policy, new_value);
                } 
                else 
                {
                	// key 对应的 ObjcAssociation 不存在，生成新的 ObjcAssociation 将 alue 和 policy 存起来
                	// 新生成的 ObjcAssociation 和 key 对应起来，存进 ObjectAssociationMap中
                    (*refs)[key] = ObjcAssociation(policy, new_value);
                }
            } 
            else {
                // create the new association (first time).
                // object 对应的 ObjectAssociationMap 不存在，则创建新的 ObjectAssociationMap 并绑定上对应的 object
                ObjectAssociationMap *refs = new ObjectAssociationMap;
                associations[disguised_object] = refs;
                
                // 新的 ObjectAssociationMap 中存放 key 和对应的 ObjcAssociation(policy, new_value)
                (*refs)[key] = ObjcAssociation(policy, new_value);
                object->setHasAssociatedObjects();
            }
        }
        else {
            // setting the association to nil breaks the association.
            // 若 value 为空，且存储 value 的 ObjcAssociation 存在，则删除 AssociationsHashMap中的这对值
            AssociationsHashMap::iterator i = associations.find(disguised_object);
            if (i !=  associations.end()) {
                ObjectAssociationMap *refs = i->second;
                ObjectAssociationMap::iterator j = refs->find(key);
                if (j != refs->end()) {
                    old_association = j->second;
                    refs->erase(j);
                }
            }
        }
    }
    // release the old value (outside of the lock).
    if (old_association.hasValue()) ReleaseValue()(old_association);
}
```

用一幅图来解释

![](../Images/iOS/Category/AssociationObject_image0302.png)


- `AssociationsHashMap`和`ObjectAssociationMap`可以看做`OC`中的字典。

- `AssociationsHashMap`中的键是传进来的`object`, 值是`ObjectAssociationMap`

- `ObjectAssociationMap`中的键是传进来的`key`，值是`ObjcAssociation`。而ObjcAssociation中放的是传进来的`policy`和`value`。


### 0x02 关联对象没有`weak`效果

关联对象没有`weak`效果，也就是说当关联对象的`value`所指向的内存被释放，不能将其置为`nil`，再去获取`value`的值就会造成坏内存访问(野指针)。

![](../Images/iOS/Category/AssociationObject_image0303.png)

## 总结

- 关联对象并不是存储在被关联对象本身的内存中，而是存在一个全局统一的`AssociationsManager`中

- 若设置关联对象某个`value`为`nil`，则相当于移除`AssociationsHashMap`中一对`key`和`ObjectAssociationMap`

- 关联对象的底层设计决定关联对象没有弱引用`weak`的效果。


**相关内容**

- [Categary - 底层实现](https://github.com/zhaoName/Notes/blob/master/iOS/Category-imp.md)

- [Categoty - load,initialize](https://github.com/zhaoName/Notes/blob/master/iOS/Categoty-load-initialize.md)

<br>

写于2019-04-25

<br>