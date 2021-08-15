# Closure

闭包 (closure) 是自包含的函数代码块，可以在代码中被传递和使用。Swift 中的闭包与 C 和 Objective-C 中的代码块（`blocks`）以及其他一些编程语言中的匿名函数（`Lambdas`）比较相似。

Swift 中闭包有如下三种表达方式：

- 全局函数是一个有名字但不会捕获任何值的闭包

- 嵌套函数是一个有名字并可以捕获其封闭函数域内值的闭包
- 闭包表达式是一个利用轻量级语法所写的可以捕获其上下文中变量或常量值的匿名闭包


<br>

## 一、闭包表达式


### 0x01 闭包表达式

Swift 标准库提供了名为 `sorted(by:)` 的方法，它会基于你提供的排序闭包表达式的判断结果对数组中的值（类型确定）进行排序。

```swift
var names = ["Chris", "Alex", "Ewa", "Barry", "Daniella"]
```

可以写一个闭包函数 `backward(_:_:)` 当 `sorted(by:)` 的参数传进去


```swift
func backward(_ s1: String, _ s2: String) -> Bool {
    return s1 > s2
}
var reversedNames = names.sorted(by: backward)

// 打印结果
// reversedNames 为 ["Ewa", "Daniella", "Chris", "Barry", "Alex"]
```

为了 `s1 > s2` 这个表达式而写了个函数，有点浪费。我们还可以用闭包表达式来更好的构造一个内联闭包函数。

闭包表达式的语法

```swift
{ (parameters) -> return type in
    statements
}
```

所以排序函数还可以这样写：

```swift
names.sorted { (s1: String, s2: String) -> Bool in
    return s1 > s2
}
```

在内联闭包表达式中，函数和返回值类型都写在大括号内，而不是大括号外。

### 0x02 闭包表达式的语法优化

Swift 的闭包表达式拥有简洁的风格，并鼓励在常见场景中进行语法优化，主要优化如下：

- 利用上下文推断参数和返回值类型

```swift
names.sorted(by: { s1, s2 in return s1 > s2 } )
```

- 隐式返回单表达式闭包，即单表达式闭包可以省略 `return` 关键字

```swift
names.sorted(by: { s1, s2 in s1 > s2 } )
```

- 参数名称缩写

```swift
reversedNames = names.sorted(by: { $0 > $1 } )
```

- 运算符

Swift 的 `String` 类型定义了关于大于号（`>`）的字符串实现，其作为一个函数接受两个 `String` 类型的参数并返回 `Bool` 类型的值。Swift 可以自动推断找到系统自带的那个字符串函数的实现。

```swift
names.sorted(by: >)
```

- 尾随闭包语法

若将一个很长的闭包表达式作为函数的最后一个实参，使用尾随闭包可以增强函数的可读性。尾随闭包是一个书写在函数圆括号之后的闭包表达式，函数支持将其作为最后一个参数调用。

```swift
func someFunctionThatTakesAClosure(closure: () -> Void) {
    // 函数体部分
}

// 以下是不使用尾随闭包进行函数调用
someFunctionThatTakesAClosure(closure: {
    // 闭包主体部分
})

// 以下是使用尾随闭包进行函数调用
someFunctionThatTakesAClosure() {
    // 闭包主体部分
}
```

如果闭包表达式是函数或方法的唯一参数，则当你使用尾随闭包时，你甚至可以把 `()` 省略掉

```swift
someFunctionThatTakesAClosure {
    // 闭包主体部分
}
```

<br>


## 二、闭包中值捕获

闭包可以在其被定义的上下文中捕获常量或变量。即使定义这些常量和变量的原作用域已经不存在，闭包仍然可以在闭包函数体内引用和修改这些值。

Swift 中，可以捕获值的闭包的最简单形式是嵌套函数，嵌套函数可以捕获其外部函数所有的参数以及定义的常量和变量。


### 0x01 

```swift
```


```swift
```

```swift
```


```swift
```


```swift
```


```swift
```

<br>


## 三、自动闭包

```swift
```


<br>

## 四、逃逸闭包


```swift
```

<br>

```swift
```

```swift
```

```swift
```

```swift
```

```swift
```


<br>



<br>

参考：

- [Closures](https://docs.swift.org/swift-book/LanguageGuide/Closures.html)

- [闭包](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/07_closures)

<br>