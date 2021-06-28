# enum 

枚举为一组相关的值定义了一个共同的类型，使你可以在你的代码中以类型安全的方式来使用这些值。

在 C 语言中枚举只能用整形变量来声明其成员，但在 Swift 中不必给每一个枚举成员提供一个值。如果给枚举成员提供一个值（称为原始值），则该值的类型可以是字符串、字符，或是一个整型值或浮点数。另外 swift 中的枚举成员可以指定任意类型的关联值存储到枚举成员中。

<br>

## 一、常规用法

### 0x01 语法

Swift 中枚举定义使用的是 `enum` 关键字

```
enum CompassPoint {
    case north
    case south
    case east
    case west
}


// 多个成员值可以出现在同一行上，用逗号隔开
enum CompassPoint {
    case north, case south, case east, case west
}
```

> NOTE
> 
> 与 C 和 Objective-C 不同，Swift 的枚举成员在被创建时不会被赋予一个默认的整型值。在上面的 CompassPoint 例
> 子中，north，south，east 和 west 不会被隐式地赋值为 0，1，2 和 3。相反，这些枚举成员本身就是完备的值，这值的类型是已经明确定义好的 CompassPoint 类型。


### 0x02 枚举成员的遍历

让枚举遵循 `CaseIterable` 协议。Swift 会生成一个 `allCases` 属性，用于表示一个包含枚举所有成员的集合。

```
enum CompassPoint: CaseIterable {
    case north, case south, case east, case west
}

let allCases = CompassPoint.allCases
print(allCases)

// 打印结果
[swift_basic.CompassPoint.north, swift_basic.CompassPoint.south, swift_basic.CompassPoint.east, swift_basic.CompassPoint.west]
```

`allCases` 的使用方法和其它一般集合一样——集合中的元素是枚举类型的实例。我们可以用 `for-in`来遍历枚举成员

```
for ca in allCases {
    print(ca)
}

//  打印结果
north
south
east
west
```


### 0x02 关联值 (Associated Values)

有时候把其他类型的值和成员值一起存储起来会很有用。这种额外的信息称为关联值，并且你每次在代码中使用该枚举成员时，还可以修改这个关联值。 Swift 中的枚举可以存储任意类型的关联值，且每个枚举成员的关联值类型可以各不相同。

如日期可以用数字年月日来表示，也可用字符串来表示

```
enum Date {
    case digit(year: Int, month: Int, day: Int)
    case dateString(str: String)
}
```

定义两个 `Date` 类型的变量

```
let dateDigit = Date.digit(year: 2021, month: 6, day: 28)
let dateString = Date.dateString(str: "2021-06-18")
```

使用 Switch 语句来匹配枚举值时，关联值可以被提取出来作为 switch 语句的一部分

```
switch dateDigit {
case .digit(let year, let month, var day):
    day = 29
    print("digit====year:\(year) month:\(month) day:\(day)")
case .dateString(let str):
    print("dateString====\(str)")
}

// digit====year:2021 month:6 day:29
```

如果一个枚举成员的所有关联值都被提取为常量，或者都被提取为变量，你可以只在成员名称前标注一个 let 或者 var

```
switch dateDigit {
case let .digit(year, month, day):
    print("digit====year:\(year) month:\(month) day:\(day)")
case .dateString(let str):
    print("dateString====\(str)")
}

// digit====year:2021 month:6 day:28
```


### 0x03 原始值 (Raw Values)




<br>

## 二、内存


<br>



<br>



<br>

参考：

- [枚举](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/08_enumerations)

- [Enumerations](https://docs.swift.org/swift-book/LanguageGuide/Enumerations.html)

<br>