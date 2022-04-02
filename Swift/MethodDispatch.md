# Method Dispatch in Swift



<br>

## 一、方法派发

编译的编程语言有三种主要的调度方法可供使用：直接调度、表调度和消息调度，我将在下面解释。 大多数语言都支持其中的一种或两种。 Java 默认使用表分派，但您可以使用 final 关键字选择直接分派。 C++ 默认使用直接分派，但您可以通过添加 virtual 关键字来选择表分派。 Objective-C 总是使用消息派发，但允许开发人员回退到 C 以获得直接派发的性能增益。 Swift 承担了支持所有三种调度类型的崇高目标。 这非常有效，但对许多开发人员来说是一个困惑的根源，并且是大多数 Swift 开发人员遇到的许多陷阱的背后。


### 0x01 Direct Dispatch

Direct dispatch (直接派发) 也称 static dispatch (静态派发)。直接派发是最快和最有效的方法分派类型。 直接分派的方法可以在运行时立即执行，因为编译器在编译时就知道确切的内存地址。 这也意味着编译器可以执行各种优化，例如内联，从而在运行时实现更快的执行时间。

直接派发的限制是不允许子类化，动态性不好。

### 0x02 


```Swift

```

```Swift

```


```Swift

```

```Swift

```

<br>



<br>

```Swift

```

```Swift

```


```Swift

```


<br>

**Reference**

- [A Deep Dive Into Method Dispatches in Swift](https://betterprogramming.pub/a-deep-dive-into-method-dispatches-in-swift-65a8e408a7d0)

- [Method Dispatch in Swift](https://www.rightpoint.com/rplabs/switch-method-dispatch-table)
- 
<br>

