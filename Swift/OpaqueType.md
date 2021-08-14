# Opaque Type

具有不透明返回类型的函数或方法会隐藏返回值的类型信息。函数不再提供具体的类型作为返回类型，而是根据它支持的协议来描述返回值。在处理模块和调用代码之间的关系时，隐藏类型信息非常有用，因为返回的底层数据类型仍然可以保持私有。而且不同于返回协议类型，不透明类型能保证类型一致性 —— 编译器能获取到类型信息，同时模块使用者却不能获取到。

<br>

### 0x01 引入问题

如下声明一个 `Runnable ` 协议，两个类 `Person `、`Dog`，它们都遵守 `Runnable ` 协议。

```swift
protocol Runnable {
    associatedtype Speed
    var speed: Speed { get }
}

class Person: Runnable {
    var speed: Double { 3.5 }
}

class Dog: Runnable {
    var speed: Int { 5 }
}
```

定义一个函数，根据传进来的值，返回的不同类型(遵守`Runnable `协议)

```swift
func get(_ type: Int) -> Runnable {
    return type == 0 ? Person() : Dog()
}
```

可以看到编译报错，说没有指定关联类型的具体类型。这是为什么呢？

![](../Images/Swift/OpaqueType/OpaqueType_images01.png)

虽然 `Person ` 和 `Dog` 中都指明了关联类型，但需要程序运行起来才能知道`Runnable ` 中的关联类型 `Speed` 到底是 `Double` 类型还是 `Int` 类型。编译时期是不能确定的，所以报错。

这时我们需要不透明类型来帮我们解决问题

<br>


### 0x02 返回不透明类型



使用`some` 关键字能声明一个不透明类型，且 `some` 限制只能返回一种类型。

```swift
func get(_ type: Int) -> some Runnable {
    return Dog()
}
```

这时有人该说了，既然只能返回一种类型，那我为什么不直接指定`get(_:)` 的返回值为 `Dog`

```swift
func get(_ type: Int) -> Dog {
    return Dog()
}
```

原因很简单，若你直接将 `Dog` 作为返回对象，将会保留 `Dog` 类中的实现细节。但将不透明类型 `some Runnable` 作为返回类型，你得到的结果只能实现 `Runnable ` 协议中的细节。


<br>

### 0x03 `some` 用在属性类型

`some` 还可以用在属性类型上。

如下 `Person` 类中有个宠物 `pet`，若只想让外部知道某个人有个宠物，但不像让人知道是什么品种的宠物，可以用不透明类型声明属性的类型。

```swift
protocol Runnable {
    associatedtype Speed
    var speed: Speed { get }
}

class Dog: Runnable {
    var speed: Int { 5 }
}

class Person {
    var pet: some Runnable {
        return Dog()
    }
}
```

<br>

参考：

- [Opaque Types](https://docs.swift.org/swift-book/LanguageGuide/OpaqueTypes.html)

- [不透明类型](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/23_opaque_types)

<br>