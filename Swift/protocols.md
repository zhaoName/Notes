# Protocols

协议规定了用来实现某一特定任务或者功能的方法、属性，以及其他需要的东西。类、结构体或枚举都可以遵循协议，并为协议定义的这些要求提供具体实现。某个类型能够满足某个协议的要求，就可以说该类型遵循这个协议


<br>

## 一、语法

协议的定义方式与类、结构体和枚举的定义非常相似

```swift
protocol SomeProtocol {

}
```

某个类遵守某个协议时，需要在类型名称后加上协议名称，中间以冒号（`:`）分隔。遵循多个协议时，各协议之间用逗号（`,`）分隔


```swift
struct SomeStructure: FirstProtocol, AnotherProtocol {

}
```

若是一个类拥有父类，应该将父类名放在遵循的协议名之前，以逗号分隔

```swift
class SomeClass: SomeSuperClass, FirstProtocol, AnotherProtocol {
    
}
```

<br>

## 二、协议中的属性

协议总是用 `var` 关键字来声明变量属性，在类型声明后加上 `{ set get }` 来表示属性是可读可写的，可读属性则用 `{ get } `来表示

```swift
protocol SomeProtocol {
    var mustBeSettable: Int { get set }
    var doesNotNeedToBeSettable: Int { get }
}
```

实现协议的属性权限要不小于协议中定义的属性权限

- 若协议中要求属性是可读可写的，那实现该属性不能是常量属性或只读的计算型属性，而需要用 `var ` 修饰的属性

```swift
class Person: SomeProtocol {
    // 协议中要求属性是可读可写的，实现的时候必须用 var 修饰
    var mustBeSettable: Int {
        set { }
        get {
            return 1
        }
    }
   ...
}
```

- 若协议中的属性权限只是可读的，可以用任何属性实现

```swift
class Person: SomeProtocol {
    // 协议中要求属性是可读可写的，实现的时候必须用 var 修饰
    var mustBeSettable: Int = 1
    
    // 下面三个任选其一即可
    // 计算属性实现协议中的「只读属性」
    var doesNotNeedToBeSettable: Int {
        return 0
    }
    
    // 存储属性实现协议中的「只读属性」
    let doesNotNeedToBeSettable: Int = 0
    
    // 可读可写的存储属性实现协议中的 「只读属性」
    var doesNotNeedToBeSettable: Int = 0
}
```

在协议中定义类型属性时，总是使用 `static` 关键字作为前缀。当类类型遵循协议时，除了 `static` 关键字，还可以使用 `class` 关键字来声明类型属性。

```swift
protocol SomeProtocol {
    static var commonClassProperty: Int { get set }
    
    static var classPropety: Int { get set }
}

struct TestStruct: SomeProtocol {
    static var commonClassProperty: Int = 0
    
    static var classPropety: Int = 0
}

class Person: SomeProtocol {
    static var commonClassProperty: Int = 1
    
    // 类中可以用 class 关键字声明类型属性
    class var classPropety: Int {
        set {}
        get { 1 }
    }
}
```


<br>

## 三、协议中的方法

### 0x01 协议中的类方法

在协议中定义类方法的时候，必须 `static` 关键字作为前缀。在类实现协议时，类方法要求使用 `class` 或 `static` 作为关键字前缀。

```swift
protocol SomeProtocol {
    static func testStaticMethod()
}

struct TestStruct: SomeProtocol {
    static func testStaticMethod() {
        
    }
}

class Person: SomeProtocol {
    static func testStaticMethod() {
        
    }
}
```


### 0x02 mutating

在值类型（即结构体和枚举）的实例方法中，将 `mutating` 关键字作为方法的前缀，写在 `func` 关键字之前，表示可以在该方法中修改它所属的实例以及实例的任意属性的值。

实现协议中的 `mutating` 方法时，若是类类型，则不用写 `mutating` 关键字。而对于结构体和枚举，则必须写 `mutating` 关键字。

```swift
protocol SomeProtocol {
    mutating func testMethod()
}

struct TestStruct: SomeProtocol {
    var st: Int = 0
    
    // 结构体、枚举要修改遵守协议的示例 方法前必须要加 mutating
    mutating func testMethod() {
        st = 1
    }   
}

class Person: SomeProtocol {
    var age: Int = 0
    
    func testMethod() {
        self.age = 10
    }
}
```

### 0x03 协议中的初始化器

协议可以要求遵循协议的类型实现指定的构造器。

```swift
protocol SomeProtocol {
    init(someParameter: Int)
}
```

遵循协议的类中实现构造器，无论是作为指定构造器，还是作为便利构造器。都必须为构造器实现标上 `required` 修饰符。`final` 修饰的类除外，因为`final` 类没有子类。

```swift
class Person: SomeProtocol {
    required init(someParameter: Int) {
        
    }
}

struct TestStruct: SomeProtocol {
    // 结构体中没有 required 关键字
    init(someParameter: Int) {
        
    }
}
```

如果一个子类重写了父类的指定构造器，并且该构造器满足了某个协议的要求，那么该构造器的实现需要同时标注 `required` 和 `override` 修饰符

```swift
protocol SomeProtocol {
    init()
}

class SomeSuperClass {
    init() {
        // 这里是构造器的实现部分
    }
}

class SomeSubClass: SomeSuperClass, SomeProtocol {
    // 因为遵循协议，需要加上 required
    // 因为继承自父类，需要加上 override
    required override init() {
        // 这里是构造器的实现部分
    }
}
```

### 0x04 协议中的可失败初始化器

协议中的 `init?()` 、`init!()`，可以用 `int()` 、`init?()` 、`init!()` 实现。

```swift
protocol SomeProtocol {
    init!()
}

class Person: SomeProtocol {
    // 三个任选其一都可
//    required init() {
//
//    }
    
//    required init?() {
//
//    }
    
    required init!() {

    }
}
```

协议中的 `init()`，可以用 `int()` 、`init!()` 实现。

```swift
protocol SomeProtocol {
    init()
}

class Person: SomeProtocol {
    // 两个任选其一都可
//    required init() {
//
//    }
    
    required init!() {

    }
}
```


<br>

## 四、其他

### 0x01 协议的继承

协议能够继承一个或多个其他协议，可以在继承的协议的基础上增加新的要求。协议的继承语法与类的继承相似，多个被继承的协议间用逗号分隔

```swift
protocol SomeProtocol {
    func someMethod()
}

protocol SubProtocol: SomeProtocol {
    func subMethod()
}

struct TestStruct: SubProtocol {
    func subMethod() {
        
    }
    
    func someMethod() {
        
    }
}
```

### 0x02 协议组合

一个类型同时遵循多个协议，叫协议组合。多个协议之间用 `&` 符号分割起来。

```swift
protocol AnotherProtocol {
    func another()
}

// 同时遵守 SomeProtocol 和 AnotherProtocol 协议
class Person: SomeProtocol & AnotherProtocol {
    func someMethod() {
        
    }
    
    func another() {
        
    }
}
```

除了协议列表，协议组合也能包含类类型

```swift

// 同时遵守 SomeProtocol 和 AnotherProtocol 协议，并是 Person 的子类
class Student: Person & SomeProtocol & AnotherProtocol {
    func someMethod() {
        
    }
    
    func another() {
        
    }
}
```

### 0x03 类的专属协议

在协议后面写上 `AnyObject` ，表示只有类能遵守这个协议。若让结构体或枚举类型遵守这个协议，则在编译时报错

```swift
protocol AnyObjectProtocol: AnyObject {
    func anyObjectProtocolMethod()
}

class Person: AnyObjectProtocol {
    func anyObjectProtocolMethod() {
        
    }
}
```

### 0x04 检查协议一致性

`is` 用于判断是否为某种类型

```swift
let stu = Student()
print("stu is Person: ", stu is Person)  // true
print("stu is SomeProtocol: ", stu is SomeProtocol) // true
```

`as?` 用于做类型装换，也可判断某个示例是否遵守某个协议

```swift
stu as? SomeProtocol
```

### 0x05 默认实现

协议可以通过扩展来为遵循协议的类型提供属性、方法以及下标的实现。通过这种方式，你可以基于协议本身来实现这些功能，而无需在每个遵循协议的类型中都重复同样的实现。

```swift
protocol AnyObjectProtocol: AnyObject {
    func anyObjectProtocolMethod()
}

extension AnyObjectProtocol {
    func anyObjectProtocolMethod() {}
}

class Person: AnyObjectProtocol {
    // Person 类遵守 AnyObjectProtocol
    // 但不是必须要实现 AnyObjectProtocol 协议中的方法
}
```


<br>



<br>

参考：

- [Protocols](https://docs.swift.org/swift-book/LanguageGuide/Protocols.html)

- [协议](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/21_protocols)

<br>