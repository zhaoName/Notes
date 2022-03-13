# POP 实现类似 `xxx.rx.xxx` 的功能


在 RxSwift 中给 `UIButton` 添加点击事件的写法`button.rx.tap.subscribe {}` 。这里的 `rx` 一方面可以代表 RxSwift 这个三方库，另一方面还可以防止三方库自定义的计算属性 `tap` 和系统属性冲突(若有)。那 `rx` 是怎么实现的呢？

<br>

### 0x01 自定义 `zz`

若我们想判断给定的字符串中阿拉伯数字的个数，一般情况下直接给 `String` 扩展个属性就可以，如下：

```swift
extension String {
    var numberCount: Int {
        var count = 0
        for c in self where ("0"..."9").contains(c) {
            count += 1
        }
        return count
    }
}
```

但这次我们想向 RxSwift 学习，实现 `"".zz.numberCount ` 这样的功能。那首先字符串要有一个类型为 `struct ZZ` 的计算属性 `zz`，而且我们要将字符串传到 `ZZ` 中。

```swift
struct ZZ {
    var string: String
    
    init(_ string: String) {
        self.string = string
    }
}

extension String {
    var zz: ZZ { ZZ(self) }
}
```

然后我们给 `struct ZZ` 扩展个计算属性 `numberCount` 计算传进来字符串中包含数字的个数

```swift
extension ZZ {
    var numberCount: Int {
        var count = 0
        for c in string where ("0"..."9").contains(c) {
            count += 1
        }
        return count
    }
}
```

这样我们就可以调用 `"1a2b3c".zz.numberCount`

```swift
print("1a2b3c".zz.numberCount) // 3
```

<br>

### 0x02 给任意实例添加 `zz` 属性

给自定义类型 `Person` 也添加一个 `zz` 属性

```swift
struct ZZ {
    var string: String?
    var person: Person?
    
    init(_ string: String) {
        self.string = string
    }
    
    init(_ person: Person) {
        self.person = person
    }
}

extension Person {
    var zz: ZZ { ZZ(self) }
}
```

这样虽然能实现我们的需求，但我们不能每次都给结构体 `ZZ` 扩展一个初始化方法，太麻烦了。

给任意类型都扩展计算属性 `zz`，首先想到的应该是泛型。将结构体 `ZZ` 中持有属性的类型泛型化，如下：

```swift
struct ZZ<Base> {
    var base: Base

    init(_ base: Base) {
        self.base = base
    }
}

extension String {
    var zz: ZZ<String> { ZZ(self) }
}

extension Person {
    var zz: ZZ<Person> { ZZ(self) }
}
```

然后在给结构体 `ZZ ` 扩展时，需要注意限定属性或方法的使用对象。如 `Person` 不能持有计算字符串中数字个数的属性，它是 `String` 需要的。

```swift
extension ZZ where Base == String {
    var numberCount: Int {
        var count = 0
        for c in base where ("0"..."9").contains(c) {
            count += 1
        }
        return count
    }
}
```

同样的 `Person` 中的属性或方法，`String`也不需要

```swift
extension ZZ where Base: Person {
    func testPerson() {
        print(#function)
    }
}
```

```swift
print("12a3b".zz.numberCount)  // 3

Person().zz.testPerson()  // testPerson()
```

<br>

### 0x03 给任意类型添加 `static zz` 属性

上面都是通过实例调用属性或方法，我们还可以添加 `static zz` 属性调用对应的 `static` 方法和属性。

```
extension String {
    ...
    static var zz: ZZ<String>.Type { ZZ<String>.self }
}

extension Person {
    ...
    static var zz: ZZ<Person>.Type { ZZ<Person>.self }
}
```

在 `ZZ` 的扩展中给 `Person` 和 `String` 添加 `static` 方法

```swift
extension ZZ where Base == String {
    ...
    static func testStaticStringFunc() {
        print(#function)
    }
}

extension ZZ where Base: Person {
    ...
    static func testStaticPersonFunc() {
        print(#function)
    }
}

String.zz.testStaticStringFunc()  // testStaticStringFunc()

Person.zz.testStaticPersonFunc()  // testStaticPersonFunc()
```

<br>

### 0x04 POP

我们在给每个类型扩展`zz` 和 `static zz` 时都需要重写一遍如下代码


```swift
extension String {
    var zz: ZZ<String> { ZZ(self) }
    
    static var zz: ZZ<String>.Type { ZZ<String>.self }
}

extension Person {
    var zz: ZZ<Person> { ZZ(self) }
    
    static var zz: ZZ<Person>.Type { ZZ<Person>.self }
}
```

这样代码很冗余，我们可以把 `zz` 和 `static zz` 抽到 `ZZCompatiable `协议中


```swift
protocol ZZCompatible {}

extension ZZCompatible {
    var zz: ZZ<Self> { ZZ(self) }
    static var zz: ZZ<Self>.Type { ZZ<Self>.self }
}
```

然后让需要增加 `zz` 和 `static zz` 的类型遵守 `ZZCompatiable `协议，完整代码如下：

```swift
extension String: ZZCompatible { }
extension ZZ where Base == String {
    var numberCount: Int {
        var count = 0
        for c in base where ("0"..."9").contains(c) {
            count += 1
        }
        return count
    }
    
    static func testStaticStringFunc() {
        print(#function)
    }
}
```

```swift
extension Person: ZZCompatible { }
extension ZZ where Base: Person {
    func testPerson() {
        print(#function)
    }
    
    static func testStaticPersonFunc() {
        print(#function)
    }
}
```
<br>

### 0x05 RxSwift 中关于`rx`的实现

RxSwift 中关于`rx`的实现在 `Reactive.swift` 中

```swift
public struct Reactive<Base> {
    /// Base object to extend.
    public let base: Base

    /// Creates extensions with base object.
    ///
    /// - parameter base: Base object.
    public init(_ base: Base) {
        self.base = base
    }
}

/// A type that has reactive extensions.
public protocol ReactiveCompatible {
    /// Extended type
    associatedtype ReactiveBase

    @available(*, deprecated, renamed: "ReactiveBase")
    typealias CompatibleType = ReactiveBase

    /// Reactive extensions.
    static var rx: Reactive<ReactiveBase>.Type { get set }

    /// Reactive extensions.
    var rx: Reactive<ReactiveBase> { get set }
}

extension ReactiveCompatible {
    /// Reactive extensions.
    public static var rx: Reactive<Self>.Type {
        get {
            return Reactive<Self>.self
        }
        // swiftlint:disable:next unused_setter_value
        set {
            // this enables using Reactive to "mutate" base type
        }
    }

    /// Reactive extensions.
    public var rx: Reactive<Self> {
        get {
            return Reactive(self)
        }
        // swiftlint:disable:next unused_setter_value
        set {
            // this enables using Reactive to "mutate" base object
        }
    }
}

import class Foundation.NSObject

/// Extend NSObject with `rx` proxy.
extension NSObject: ReactiveCompatible { }

```

Alamofire 中也有类似的实现，可参考`AlamofireExtended.swift`

<br>
