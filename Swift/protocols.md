# Protocols

协议规定了用来实现某一特定任务或者功能的方法、属性，以及其他需要的东西。类、结构体或枚举都可以遵循协议，并为协议定义的这些要求提供具体实现。某个类型能够满足某个协议的要求，就可以说该类型遵循这个协议


<br>

## 一、语法

协议的定义方式与类、结构体和枚举的定义非常相似

```
protocol SomeProtocol {

}
```

某个类遵守某个协议时，需要在类型名称后加上协议名称，中间以冒号（`:`）分隔。遵循多个协议时，各协议之间用逗号（`,`）分隔


```
struct SomeStructure: FirstProtocol, AnotherProtocol {

}
```

若是一个类拥有父类，应该将父类名放在遵循的协议名之前，以逗号分隔

```
class SomeClass: SomeSuperClass, FirstProtocol, AnotherProtocol {
    
}
```

<br>

## 二、协议中的属性





<br>


<br>



<br>



<br>

参考：

- [Protocols](https://docs.swift.org/swift-book/LanguageGuide/Protocols.html)

- [协议](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/21_protocols)

<br>