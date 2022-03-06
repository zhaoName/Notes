# Access Control


访问控制 (Access Control) 可以限定其它源文件或模块对你的代码的访问。这个特性可以让你隐藏代码的实现细节，并且能提供一个接口来让别人访问和使用你的代码。

你可以明确地给单个类型（类、结构体、枚举）设置访问级别，也可以给这些类型的属性、方法、构造器、下标等设置访问级别。协议也可以被限定在一定访问级别的范围内使用，包括协议里的全局常量、变量和函数。

<br>

## 一、简介


### 0x01 模块和源文件

Swift 中的访问控制模型基于模块和源文件这两个概念。

模块指的是独立的代码单元，框架或应用程序会作为一个独立的模块来构建和发布。在 Swift 中，一个模块可以使用 `import` 关键字导入另外一个模块。

在 Swift 中，Xcode 的每个 target（例如框架或应用程序）都被当作独立的模块处理。如果你是为了实现某个通用的功能，或者是为了封装一些常用方法而将代码打包成独立的框架，这个框架就是 Swift 中的一个模块。当它被导入到某个应用程序或者其他框架时，框架的内容都将属于这个独立的模块。

源文件就是 Swift 模块中的源代码文件（实际上，源文件属于一个应用程序或框架）。尽管我们一般会将不同的类型分别定义在不同的源文件中，但是同一个源文件也可以包含多个类型、函数等的定义。

### 0x02 访问级别

Swift 为代码中的实体提供了五种不同的访问级别 (以下从高到低排列)。这些访问级别不仅与源文件中定义的实体相关，同时也与源文件所属的模块相关。

- `open` 允许在定义实体的模块、其他模块中访问，允许其他模块进行继承、重写(`open` 只能作用在类、类成员上)

- `public` 允许在定义实体的模块、其他模块中访问，不允许其他模块进行继承、重写

- `internal` 允许让实体被同一模块源文件中的任何实体访问，但是不能被模块外的实体访问

- `fileprivate` 限制实体只能在其定义的源文件内部访问

- `private` 限制实体只能在其定义的作用域，以及同一文件内的 `extension` 访问

若不指定访问级别，则绝大多数实体默认都是 `internal` 级别。

<br>

## 二、访问级别使用准则

**Swift 中的访问级别遵循一个基本原则：实体不能定义在具有更低访问级别（更严格）的实体中**

### 0x01 常用举例

变量、常量类型的访问级别 ≥ 变量常量的访问级别。

假如在 `A` 源文件的全局域定义 `private ` 级别的类型 `Person` 和 `internal` 级别的常量 `p`，那在同模块下的源文件 `B` 中是可以访问到常量 `p`，但常量 `p` 的类型又是 `private`，这就造成冲突。所以变量、常量类型的访问级别  ≥ 变量、常量的访问级别。

```swift
private class Person {}

// 常量 p 是 internal 级别 > 类型 Person 是 private 级别，就会报错。
// ❎ Constant cannot be declared internal because its type 'Person' uses a private type
internal let p = Person()
```

参数类型、返回值类型 ≥ 函数

```swift
private class Person {}

// 参数类型 open > 函数 private，返回值类型 private = 函数 private
private func test(a: Int) -> Person {
    return Person()
}

// 参数类型 open > 函数 internal，返回值类型 private < 函数 internal 所以报错
// ❎ Function must be declared private or fileprivate because its result uses a private type
func test1(a: Int) -> Person {
    return Person()
}
```

父类 ≥ 子类

```swift
private class Person {}

// ❎ Class must be declared private or fileprivate because its superclass is private
class Student: Person {}
```

父协议 ≥ 子协议

```swift
private protocol SuperProtocol {}

// ❎ Protocol must be declared private or fileprivate because it refines a private protocol
protocol ChildProtocol: SuperProtocol {}
```

原类型 ≥ `typealias`

```swift
private class Person {}

// ❎ Type alias must be declared private or fileprivate because its underlying type uses a private type
typealias MyPerson = Person
```

原始值类型、关联值类型 ≥ 枚举类型

```swift
private typealias MyInt = Int

private class Person {}

// ❎ 'MyEnum' declares raw type 'MyInt' (aka 'Int'), but does not conform to RawRepresentable and conformance could not be synthesized
enum MyEnum: MyInt {
    // ❎ Enum with raw type cannot have cases with arguments
    case sample(p: Person)
}
```

### 0x02 元组类型

元组的访问级别将由元组中访问级别最低的类型来决定，不能被显示指定。

```swift
typealias MyInt = Int

private class Person {}
// 元组的访问级别是 private
private var data: (MyInt, Person)
```

### 0x03 泛型类型

泛型类型的访问级别是类型的访问级别以及所有泛型类型参数的访问级别中最低的那个决定的

```swift
fileprivate class Dog {}
class Cat {}
public class Person<T1, T2> {}

// Person<Dog, Cat> 的访问级别是 fileprivate
fileprivate var p = Person<Dog, Cat>()
```

### 0x04 成员、嵌套类型

类型的访问级别会影响成员(属性、方法、初始化器、下标)、嵌套类型的默认访问级别

- 一般情况下，类型是 `private` 或 `fileprivate`，那么成员、方法、嵌套类型等默认也是 `private` 或 `fileprivate`

```swift
fileprivate class SomeFilePrivateClass {        // 显式 fileprivate 类
    func someFilePrivateMethod() {}             // 隐式 fileprivate 类成员
    private func somePrivateMethod() {}         // 显式 private 类成员
}

private class SomePrivateClass {                // 显式 private 类
    func somePrivateMethod() {}                 // 隐式 private 类成员
}
```

- 一般情况下，类型为 `internal` 或 `public`, 那么成员、方法、嵌套类型等默认是 `internal` 

```swift
public class SomePublicClass {                   // 显式 public 类
    public var somePublicProperty = 0            // 显式 public 类成员
    var someInternalProperty = 0                 // 隐式 internal 类成员
    fileprivate func someFilePrivateMethod() {}  // 显式 fileprivate 类成员
    private func somePrivateMethod() {}          // 显式 private 类成员
}

class SomeInternalClass {                        // 隐式 internal 类
    var someInternalProperty = 0                 // 隐式 internal 类成员
    fileprivate func someFilePrivateMethod() {}  // 显式 fileprivate 类成员
    private func somePrivateMethod() {}          // 显式 private 类成员
}
```

### 0x05 `getter setter`

`getter` `setter`默认自动接收它们所属环境的访问级别。也可以单独给 `setter` 设置一个比 `getter` 更低的访问级别，用于限制写的权限。


```swift
public struct TrackedString {
    // getter 的访问级别设置为 internal，而 setter 的访问级别设置为 private
    private(set) var numberOfEdits = 0
    
    // getter 的访问级别设置为 public，而 setter 的访问级别设置为 fileprivate
    public fileprivate(set) var numberOfTracked = 0
    
    var value: String = "" {
        didSet {
            numberOfEdits += 1
        }
    }
}
```

### 0x06 初始化器

若结构体有 `private` 或 `fileprivate` 的存储实例属性，那么它的成员初始化器也是 `private` 或 `fileprivate`

```swift
struct TrackedString {
    private var numberOfEdits = 0
    var numberOfTracked = 0
}

// 只能访问到 TrackedString() 初始化器
// 访问不到 TrackedString(numberOfEdits:, numberOfTracked:) 初始化器
let t = TrackedString()
```

### 0x07 枚举类型的 `case`

不能给 `enum` 的每个 `case` 单独设置访问级别，每个 `case` 自动接收所属环境的访问级别。`public enum` 定义的 `case` 也是 `public`

```swift
public enum CompassPoint {
    // north 也是 public 级别
    case north
    case south
    case east
    case west
}
```

### 0x08 `extension` 的私有成员

扩展同一文件内的类，结构体或者枚举，`extension` 里的代码会表现得跟声明在原类型里的一模一样

- 在类型的声明里声明一个私有成员，在同一文件的 `extension` 里访问。

- 在 `extension` 里声明一个私有成员，在同一文件的另一个 `extension` 里访问。
- 在 `extension` 里声明一个私有成员，在同一文件的类型声明里访问。

```swift
protocol SomeProtocol {
    func doSomething()
}

struct SomeStruct {
    private var privateVariable = 12
}

extension SomeStruct: SomeProtocol {
    func doSomething() {
        print(privateVariable)
    }
}
```

### 0x09 注意点

下面代码是能编译过的，因为在全局作用域下定义的 `private` 等价于 `fileprivate`

```swift
private class Person {}

fileprivate class Student: Person {}
```

<br>

参考：

- [Access Control](https://docs.swift.org/swift-book/LanguageGuide/AccessControl.html)

- [访问控制](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/26_access_control)