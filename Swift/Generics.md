# Generics


泛型 (Generics) 代码让你能根据自定义的需求，编写出适用于任意类型的、灵活可复用的函数及类型。你可避免编写重复的代码，而是用一种清晰抽象的方式来表达代码的意图。

<br>

## 一、泛型解决的问题

如下，定义一个交换两个 `Int` 值的函数。

```swift
func swapTwoInts(_ a: inout Int, _ b: inout Int) {
    (a, b) = (b, a)
}

var a = 10
var b = 20

swapTwoInts(&a, &b)

print("a=\(a), b=\(b)")
// 打印结果
a=20, b=10
```

虽然 `swapTwoInts(_:_:)` 函数很实用，但它只能作用于 `Int` 类型。若需要交换 `String` 、`Double`等类型的数据，需要定义新的函数。

```swift
func swapTwoStrings(_ a: inout String, _ b: inout String) {
    (a, b) = (b, a)
}

func swapTwoDoubles(_ a: inout Double, _ b: inout Double) {
    (a, b) = (b, a)
}
```

在实际应用中，通常需要一个更实用更灵活的函数来交换两个任意类型的值，而泛型代码能帮你解决这种问题。


<br>

## 二、泛型函数、泛型类型

### 0x01 泛型函数

泛型函数通常使用占位符类型名 `T`，占位符类型名并不关心` T` 具体的类型，但它要求 `a` 和 `b` 必须是相同的类型，`T` 的实际类型由每次调用 `swapTwoValues(_:_:)` 来决定。


```swift
func swapTwoValues<T>(_ a: inout T, _ b: inout T) {
    (a, b) = (b, a)
}
```

如下 `T` 分别代表 `Int` 和 `String`

```swift
var someInt = 3
var anotherInt = 107
swapTwoValues(&someInt, &anotherInt)
// someInt 现在是 107，anotherInt 现在是 3

var someString = "hello"
var anotherString = "world"
swapTwoValues(&someString, &anotherString)
// someString 现在是“world”，anotherString 现在是“hello”
```

### 0x02 泛型类型

`Element` 为待提供的类型定义了一个占位名。

```swift
struct Stack<Element> {
    var items: [Element] = []
    mutating func push(_ item: Element) {
        items.append(item)
    }
    mutating func pop() -> Element {
        return items.removeLast()
    }
}

// 和泛型函数不同，泛型类型声明时必须指定具体类型(Int or String ...)
let intSatck = Stack<Int>()
let stringSatck = Stack<String>()
```

`Element` 在如下三个地方被用作占位符：

- 创建 `items` 属性，使用 `Element` 类型的空数组对其进行初始化。

- 指定 `push(_:)` 方法的唯一参数 `item` 的类型必须是 `Element` 类型。
- 指定 `pop()` 方法的返回值类型必须是 `Element` 类型

<br>

## 三、关联类型

协议中若想使用泛型，需使用 `associatedtype` 关键字，来声明一个或多个关联类型作为协议定义的一部分。

如下定义了一个 `Container` 协议，协议中定义了一个关联类型 `Item`

```swift
protocol Container {
    associatedtype Item
    mutating func append(_ item: Item)
    var count: Int { get }
    subscript(i: Int) -> Item { get }
}
```

该协议没有指定容器中元素该如何存储以及元素类型。该协议只指定了任何遵从 `Container` 协议的类型必须提供的三个功能。

上述泛型 `Stack` 遵守 `Container ` 协议

```swift
struct Stack<Element>: Container {
    var items: [Element] = []
    mutating func push(_ item: Element) {
        items.append(item)
    }
    mutating func pop() -> Element {
        return items.removeLast()
    }
    
    // Container 协议实现的部分
    // 指定协议中关联类型的具体类型
    // 这句代码也可注释掉，由 Swift 的类型推断类确定 Item 是 Element 类型
    typealias Item = Element
    mutating func append(_ item: Element) {
        push(item)
    }
    var count: Int {
        return items.count
    }
    subscript(i: Int) -> Element {
        return items[i]
    }
}


var intStack = Stack<Int>()
var stringStack = Stack<String>()
```

### 0x01 给关联类型添加约束

要遵守 `Container` 协议，`Item` 类型也必须遵守 `Equatable` 协议

```swift
protocol Container {
    associatedtype Item: Equatable
    mutating func append(_ item: Item)
    var count: Int { get }
    subscript(i: Int) -> Item { get }
}
```

### 0x02 在关联类型约束里使用协议


```swift
protocol SuffixableContainer: Container {
    associatedtype Suffix: SuffixableContainer where Suffix.Item == Item
    func suffix(_ size: Int) -> Suffix
}
```

`Suffix` 拥有两个约束：它必须遵循 `SuffixableContainer` 协议（就是当前定义的协议），以及它的 `Item` 类型必须是和容器里的 `Item` 类型相同。


```swift
extension Stack: SuffixableContainer {
    func suffix(_ size: Int) -> Stack {
        var result = Stack()
        for index in (count-size)..<count {
            result.append(self[index])
        }
        return result
    }
    // 推断 suffix 结果是Stack。
}

var stackOfInts = Stack<Int>()
stackOfInts.append(10)
stackOfInts.append(20)
stackOfInts.append(30)
let suffix = stackOfInts.suffix(2)
// suffix 包含 20 和 30
```


<br>


## 四、泛型约束

### 0x01 `where`

通过泛型 `where` 子句让关联类型遵从某个特定的协议，以及某个特定的类型参数和关联类型必须类型相同。

如下定义了一个名为 `allItemsMatch` 的泛型函数，用来检查两个 `Container` 实例是否包含相同顺序的相同元素。

```swift
func allItemsMatch<C1: Container, C2: Container>
(_ someContainer: C1, _ anotherContainer: C2) -> Bool
    where C1.Item == C2.Item, C1.Item: Equatable {

        // 检查两个容器含有相同数量的元素
        if someContainer.count != anotherContainer.count {
            return false
        }

        // 检查每一对元素是否相等
        for i in 0..<someContainer.count {
            if someContainer[i] != anotherContainer[i] {
                return false
            }
        }

        // 所有元素都匹配，返回 true
        return true
}
```

- `C1` 必须符合 `Container` 协议（写作 `C1: Container`）。

- `C2` 必须符合 `Container` 协议（写作 `C2: Container`）。
- `C1` 的 `Item` 必须和 `C2` 的 `Item` 类型相同（写作 `C1.Item == C2.Item`）。
- `C1` 的 `Item` 必须符合 `Equatable` 协议（写作 `C1.Item: Equatable`）


```swift
var stackOfStrings = Stack<String>()
stackOfStrings.push("uno")
stackOfStrings.push("dos")
stackOfStrings.push("tres")

var arrayOfStrings = ["uno", "dos", "tres"]

if allItemsMatch(stackOfStrings, arrayOfStrings) {
    print("All items match.")
} else {
    print("Not all items match.")
}
// 打印“All items match.”
```

### 0x02 具有泛型 `Where` 子句的扩展


```swift
// 只有当栈中的元素符合 Equatable 协议时，扩展才会添加 isTop(_:) 方法
extension Stack where Element: Equatable {
    func isTop(_ item: Element) -> Bool {
        guard let topItem = items.last else {
            return false
        }
        return topItem == item
    }
}

```

<br>


参考： 

- [Generics](https://docs.swift.org/swift-book/LanguageGuide/Generics.html)

- [泛型](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/22_generics)

<br>

