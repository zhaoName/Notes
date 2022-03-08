
# Literal Protocol (字面量协议)


<br>

### 0x01 字面量的默认类型

所谓字面量，就是指像特定的数字，字符串或者是布尔值这样，能够直接了当地指出自己的类型并为变量进行赋值的值。

Swift 自带的绝大多数类型 (`Bool、Int、Double、String、Array ...`) 都支持直接通过字面量初始化，如下：


```swift
let age = 10.0
let isShow = false
let literalString = "literalString"
```

那为什么我们用字面量 10.0 初始化的常量 `age` ，它就是 `Double` 类型，而不是 `Float` 类型的呢？

这是因为 Swift 为常见的字面定义了默认类型

```swift
public typealias IntegerLiteralType = Int

public typealias FloatLiteralType = Double

public typealias BooleanLiteralType = Bool

public typealias StringLiteralType = String
```

我们也可以通过 `typealias ` 修改字面量的默认类型

```swift
typealias FloatLiteralType = Float
let age = 10.0  // Float

typealias IntegerLiteralType = UInt32
let int32 = 32 // UInt32
```

<br>

### 0x02 字面量协议

在 Swift 中我们之所以能够用字面量来初始化，是因为我们遵守了对应的字面量协议，如 `Bool` 的定义

```swift
extension Bool : ExpressibleByBooleanLiteral {
    /// Creates an instance initialized to the specified Boolean literal.
    ///
    /// Do not call this initializer directly. It is used by the compiler when
    /// you use a Boolean literal. Instead, create a new `Bool` instance by
    /// using one of the Boolean literals `true` or `false`.
    ///
    ///     var printedMessage = false
    ///
    ///     if !printedMessage {
    ///         print("You look nice today!")
    ///         printedMessage = true
    ///     }
    ///     // Prints "You look nice today!"
    ///
    /// In this example, both assignments to the `printedMessage` variable call
    /// this Boolean literal initializer behind the scenes.
    ///
    /// - Parameter value: The value of the new instance.
    public init(booleanLiteral value: Bool)

    /// A type that represents a Boolean literal, such as `Bool`.
    public typealias BooleanLiteralType = Bool
}
```

其它还有 (不展开具体定义)

```swift
Int: ExpressibleByIntegerLiteral 

Float、Double: ExpressibleByIntegerLiteral、ExpressibleByFloatLiteral

String: ExpressibleByStringLiteral

Array、Set: ExpressibleByArrayLiteral

Dictionary: ExpressibleByDictionaryLiteral

Optional: ExpressibleByNilLiteral
```

<br>

### 0x03 字面量协议应用

用布尔值来初始化整形变量

```swift
extension Int: ExpressibleByBooleanLiteral {
    public init(booleanLiteral value: BooleanLiteralType) {
        // value 就是传进来的布尔值
        self = value ? 1 : -1
    }
}

let positive: Int = true
let nagetive: Int = false
print("positive: \(positive), nagetive: \(nagetive)")

// positive: 1, nagetive: -1
```

用字面量来声明对象

```swift
class Person: ExpressibleByIntegerLiteral, ExpressibleByFloatLiteral, ExpressibleByStringLiteral, CustomStringConvertible {
    var name: String = ""
    var age: Int = 0
    
    required init(integerLiteral value: IntegerLiteralType) {
        self.age = value
    }
    
    required init(floatLiteral value: FloatLiteralType) {
        self.age = Int(value)
    }
    
    required init(stringLiteral value: StringLiteralType) {
        self.name = value
    }
    
    var description: String {
        "name=\(name), age=\(age)"
    }
}

var person: Person = 18
print(person) // name=, age=18
person = 20.0
print(person) // name=, age=20
person = "zhao"
print(person) // name=zhao, age=0
```

<br>





