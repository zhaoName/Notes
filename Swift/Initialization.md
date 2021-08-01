# Initialization


初始化 (`Initialization`) 是准备一个类、结构或枚举的实例以供使用的过程。此过程会为该实例上的每个存储属性设置初始值，并在新实例准备好使用之前执行所需的任何其他设置或初始化。



<br>

## 一、初始化

### 0x01 指定初始化和便捷初始化

指定初始化器 (Designated Initializer)是类中最主要的初始化器 。一个指定初始化器将初始化类中提供的所有属性，并调用合适的父类初始化器让初始化过程沿着父类链继续往上进行。

类倾向于拥有极少的指定构造器，普遍的是一个类只拥有一个指定构造器(最少有一个)。

```
init(parameters) {
    statements
}
```

便捷初始化器 (Convenience Initializer) 是类中比较次要的、辅助型的初始化器。你可以定义便利初始化器来调用同一个类中的指定初始化器，并为部分形参提供默认值。

```
convenience init(parameters) {
    statements
}
```


### 0x02 初始化器相互调用的规则

- 指定初始化器必须调用其直接父类的的指定初始化器

- 便捷初始化器必须调用同类中定义的其它初始化器
- 便捷初始化器最终必须调用一个指定初始化器

![](../Images/Swift/Initialization/Initialization_image01.png)

![](../Images/Swift/Initialization/Initialization_image02.png)

可以总结为：

- 指定初始化器必须总是向上代理

- 便捷初始化器必须总是横向代理

<br>

## 二、两段式初始化


<br>

参考：

- [Initialization](https://docs.swift.org/swift-book/LanguageGuide/Initialization.html)

- [构造过程](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/14_initialization)

<br>
