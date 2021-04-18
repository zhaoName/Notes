
# Singleton Pattern (单例模式)


<br>

## 一、简介


```
static SinglePatton *_single = nil;

+ (instancetype)shareInstance
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _single = [[SinglePatton alloc] init];
    });
    return _single;
}
```

<br>


<br>


<br>

参考：

- [避免滥用单例](https://objccn.io/issue-13-2/)

- [单例的正确写法](https://zhuanlan.zhihu.com/p/37660939)

<br>