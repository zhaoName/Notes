
# Property

属性将值与特定的类、结构体或枚举关联。存储属性 (Stored Property) 会将常量和变量存储为实例的一部分，而计算属性则是直接计算（而不是存储）值。计算属性 (Computed property) 可以用于类、结构体和枚举，而存储属性只能用于类和结构体。

存储属性和计算属性通常与特定类型的实例关联。但是，属性也可以直接与类型本身关联，这种属性称为类型属性。

<br>

## 一、属性分类

### 0x01 存储属性

存储属性存在实例的内存中。且 Swift 中明确规定在创建类或结构体时，必须为所有的存储属性设置一个初始值。

- 可以在初始化器中为存储属性设置初始值

- 也可以在声明存储属性的时候为其分配一个默认值

```swift
struct FixedLengthRange {
    var firstValue: Int
    let length: Int
}
```

### 0x02 延时加载存储属性 (Lazy Stored Property)

延时加载存储属性是指当第一次被调用的时候才会计算其初始值的属性。在属性声明前使用 `lazy` 来标示一个延时加载存储属性.

必须将延时加载属性声明成变量（使用 `var` 关键字），因为属性的初始值可能在实例构造完成之后才会得到。而常量属性在构造过程完成之前必须要有初始值，因此无法声明成延时加载。

```
class Car {
    init() {
        print("car init")
    }
    
    func run() {
        print("car is running")
    }
}

class Person {
    lazy var car = Car()
    
    init() {
        print("person init")
    }
    
    func goOut() {
        car.run()
    }
}

let p = Person()
print("-----")
p.goOut()

// 打印结果
person init
-----
car init
car is running
```

当结构体包含一个延迟属性时，若要访问结构体中的延迟属性，则结构体实例必须用 `var` 修饰。

```
struct Point {
    var x = 0
    var y = 0
    lazy var z = 0
}

// 结构体实例 p1 必须用 var 修饰，否则报
var p1 = Point()
p1.z = 1
```

![](../Images/Swift/property/property_images01.png)


### 0x02 计算属性


<br>

## 二、属性观察器

<br>



<br>



<br>

参考：

- [Properties](https://docs.swift.org/swift-book/LanguageGuide/Properties.html)

- [属性](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/10_properties)

<br>