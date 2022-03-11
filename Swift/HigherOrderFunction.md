# 高阶函数

<br>

## 一、数组中常见的高阶函数


### 0x01 `map`

`map` 的定义如下，它返回的结果是个数组。返回数组中元素就是闭包 `(Element) throws -> T` 返回值。

```swift
public func map<T>( _ transform: (Element) throws -> T) rethrows -> [T] {
    let n = self.count
    if n == 0 { return [] }
    
    var result = ContiguousArray<T>()
    result.reserveCapacity(n)
    
    var i = self.startIndex
    
    for _ in 0..<n {
        result.append(try transform(self[i]))
        formIndex(after: &i)
    }
    
    _expectEnd(of: self, is: i)
    return Array(result)
}
```

例：

```swift
let cast = ["Vivien", "Marlon", "Kim", "Karl"]
let mapCast = cast.map { (ele) -> String in
    return ele.lowercased()
}

print(mapCast)
// ["vivien", "marlon", "kim", "karl"]
```


### 0x02 `flatMap`

`flatMap` 的定义如下。闭包 `(Element) throws -> SegmentOfResult` 的返回值是个遵守 `Sequence ` 协议的数组。`flatMap `的返回值是将闭包返回值(数组)的元素，重新组装成一个数组 `[SegmentOfResult.Element] `。

```swift
public func flatMap<SegmentOfResult: Sequence>(_ transform: (Element) throws -> SegmentOfResult) rethrows -> [SegmentOfResult.Element] {
    var result: [SegmentOfResult.Element] = []
    for element in self {
        result.append(contentsOf: try transform(element))
    }
    return result
}
```

例：

```swift
let numbers = [1, 2, 3, 4]

let mapped = numbers.map { Array(repeating: $0, count: $0) }
// // [[1], [2, 2], [3, 3, 3], [4, 4, 4, 4]]

let flatMapped = numbers.flatMap { Array(repeating: $0, count: $0) }
// [1, 2, 2, 3, 3, 3, 4, 4, 4, 4]
```

### 0x03 `compactMap`

`compactMap` 的定义如下。闭包 `transform: (Element) throws -> ElementOfResult?` 的返回值是可选类型。`compactMap` 的返回值是将闭包的返回值解包后放在一个数组中。简单来说 `compactMap` 可以去除值为 `nil` 的元素。

```swift
public func compactMap<ElementOfResult>(_ transform: (Element) throws -> ElementOfResult?) rethrows -> [ElementOfResult] {
    return try _compactMap(transform)
}

public func _compactMap<ElementOfResult>(_ transform: (Element) throws -> ElementOfResult?) rethrows -> [ElementOfResult] {
    var result: [ElementOfResult] = []
    for element in self {
        if let newElement = try transform(element) {
            result.append(newElement)
        }
    }
    return result
}
```

例：

```swift
let possibleNumbers = ["1", "2", "three", "///4///", "5"]

let mapped: [Int?] = possibleNumbers.map { str in Int(str) }
// [1, 2, nil, nil, 5]

let compactMapped: [Int] = possibleNumbers.compactMap { str in Int(str) }
// [1, 2, 5]
```


### 0x04 `filter`

`filter` 可以根据闭包 `_ isIncluded: (Element) throws -> Bool` 设置的条件来过滤数组中的元素。

```swift
public __consuming func filter(_ isIncluded: (Element) throws -> Bool) rethrows -> [Element] {
    return try _filter(isIncluded)
}

public func _filter(_ isIncluded: (Element) throws -> Bool) rethrows -> [Element] {
    var result = ContiguousArray<Element>()
    
    var iterator = self.makeIterator()
    
    while let element = iterator.next() {
        if try isIncluded(element) {
            result.append(element)
        }
    }
    
    return Array(result)
}
```

例：

```swift
let cast = ["Vivien", "Marlon", "Kim", "Karl"]
let shortNames = cast.filter { $0.count < 5 }
print(shortNames)
// Prints "["Kim", "Karl"]"
```

### 0x05 `reduce`

`reduce` 定义如下。在第一次循环时，会将初始值 `initialResult ` 传到闭包中，得到返回值 `partialResult`。后续循环都会将 `partialResult ` 传到闭包中，得到最终结果`partialResult `。

```swift
public func reduce<Result>(_ initialResult: Result,
                           _ nextPartialResult:(_ partialResult: Result, Element) throws -> Result) rethrows -> Result {
    var accumulator = initialResult
    for element in self {
        accumulator = try nextPartialResult(accumulator, element)
    }
    return accumulator
}
```

例：

```swift
let numbers = [1, 2, 3, 4]
let numberSum = numbers.reduce(10, { $0 + $1 })

// numberSum == 20
```

使用 `reduce` 实现 `map` 的功能

```swift
let numbers = [1, 2, 3, 4]

let mapNums = numbers.map { $0 * 2 }

let reduceNums = numbers.reduce([], { initResult, ele in
    return initResult + [ele * 2]
})

// [2, 4, 6, 8]  [2, 4, 6, 8]
```
使用 `reduce` 实现 `filter` 的功能

```swift
let filterNums = numbers.filter { $0 % 2 == 0 } 

let reduceNums = numbers.reduce([]) { initResult, ele in
    return ele % 2 == 0 ? initResult + [ele] : initResult
}
// [2, 4]  [2, 4]
```

<br>

## 二、`Optional` 常见的高阶函数


### 0x01 `map`

`map` 会将闭包返回的结果包装成可选类型返回出去。

```swift
public func map<U>(_ transform: (Wrapped) throws -> U) rethrows -> U? {
    switch self {
    case .some(let y):
        return .some(try transform(y))
    case .none:
        return .none
    }
}
```

```swift
var num1: Int? = 10

var num2 = num1.map { $0 * 2 }
// Optional(20) 

var num4 = num1.map { Optional.some($0 * 2) }
// Optional(Optional(20))
```

### 0x02 `flatMap`

`flatMap` 会将闭包返回的结果直接返回出去。

```swift
public func flatMap<U>(_ transform: (Wrapped) throws -> U?) rethrows -> U? {
    switch self {
    case .some(let y):
        return try transform(y)
    case .none:
        return .none
    }
}
```

```swift
var num1: Int? = 10

var num4 = num1.map { Optional.some($0 * 2) }
// Optional(20) 

var num5 = num1.flatMap { Optional.some($0 * 2) }
// Optional(20) 
```

<br>

## 三、`lazy` 在高级函数中的使用

```swift
let cast = ["Vivien", "Marlon", "Kim", "Karl"]
let mapCast = cast.map { (ele) -> String in
    print("mapping...\(ele)")
    return ele.lowercased()
}

print("map begin-----")
print("mapped...\(mapCast[0])")
print("mapped...\(mapCast[1])")
print("mapped...\(mapCast[2])")
print("mapped...\(mapCast[3])")
print("map end-----")

// 打印结果
mapping...Vivien
mapping...Marlon
mapping...Kim
mapping...Karl
map begin-----
mapped...vivien
mapped...marlon
mapped...kim
mapped...karl
map end-----
```
可以看到在调用 `map` 函数前，就已经对开始处理闭包。如果闭包中是一个很耗时的操作，就很浪费性能。

`lazy` 能帮助我们懒处理 `map` 的闭包

```swift
let mapCast = cast.lazy.map { (ele) -> String in
    print("mapping...\(ele)")
    return ele.lowercased()
}

print("map begin-----")
print("mapped...\(mapCast[0])")
print("mapped...\(mapCast[2])")
print("map end-----")

// 打印结果
map begin-----
mapping...Vivien
mapped...vivien
mapping...Kim
mapped...kim
map end-----
```


<br>

参考

- [SequenceAlgorithms.swift](https://github.com/apple/swift/blob/main/stdlib/public/core/SequenceAlgorithms.swift)

- [Optional.swift](https://github.com/apple/swift/blob/main/stdlib/public/core/Optional.swift)