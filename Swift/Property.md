
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


### 0x03 计算属性

计算属性不直接存储值，而是提供一个 getter 和一个可选的 setter，来间接获取和设置其他属性或变量的值。

定义计算属性必须用 `var` 声明，因为 `let` 表示常量，值时一成不变的。而计算属性的值是可能发生变化的。

```
struct Point {
    var x = 0.0, y = 0.0
}
struct Size {
    var width = 0.0, height = 0.0
}
struct Rect {
    var origin = Point()
    var size = Size()
    var center: Point {
        get {
            let centerX = origin.x + (size.width / 2)
            let centerY = origin.y + (size.height / 2)
            return Point(x: centerX, y: centerY)
        }
        set(newCenter) {
            origin.x = newCenter.x - (size.width / 2)
            origin.y = newCenter.y - (size.height / 2)
        }
    }
}
```

如果计算属性的 `setter` 没有定义表示新值的参数名，则可以使用默认名称 `newValue`。

```
struct AlternativeRect {
    var origin = Point()
    var size = Size()
    var center: Point {
        get {
            let centerX = origin.x + (size.width / 2)
            let centerY = origin.y + (size.height / 2)
            return Point(x: centerX, y: centerY)
        }
        set {
            origin.x = newValue.x - (size.width / 2)
            origin.y = newValue.y - (size.height / 2)
        }
    }
}
```

**只读计算属性**只有 `getter` 没有 `setter` 的计算属性叫只读计算属性。只读计算属性的声明可以去掉 `get` 关键字和花括号

```
struct Cuboid {
    var width = 0.0, height = 0.0, depth = 0.0
    var volume: Double {
        return width * height * depth
    }
}
```


<br>

## 二、属性观察器

### 0x01 存储属性观察器

Swift 中可以为非 `lazy` 的 `var` 存储属性设置属性观察器。

```
class StepCounter {
    var totalSteps: Int = 0 {
        willSet(newTotalSteps) {
            print("将 totalSteps 的值设置为 \(newTotalSteps)")
        }
        didSet {
            if totalSteps > oldValue  {
                print("增加了 \(totalSteps - oldValue) 步")
            }
        }
    }
}

let stepCounter = StepCounter()
stepCounter.totalSteps = 200

// 打印结果
将 totalSteps 的值设置为 200
增加了 200 步
```

- `willSet` 会传递新值，默认叫`newValue`

- `didSet` 会传递旧值，默认叫`oldValue`
- 在初始化器中设置属性值不会触发 `willSet` 和 `didSet` 
- 在存储属性定义时设置初始值也不会触发 `willSet` 和 `didSet` 


### 0x02 全局变量、局部变量

属性观察器对全局变量、局部变量同样有效

```
// 全局变量
var year: Int = 2020{
    willSet {
        print("newValue: \(newValue)")
    }
    didSet {
        print("oldValue: \(oldValue)")
    }
}

year = 2021

// 打印结果
newValue: 2021
oldValue: 2020
```


```
func test11() {
    var age: Int = 10 {
        willSet {
            print("newValue: \(newValue)")
        }
        didSet {
            print("oldValue: \(oldValue)")
        }
    }
    age = 100
}

// 打印结果
newValue: 100
oldValue: 10
```




<br>



<br>



<br>

参考：

- [Properties](https://docs.swift.org/swift-book/LanguageGuide/Properties.html)

- [属性](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/10_properties)

<br>