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
func flatMap<SegmentOfResult>(_ transform: (Element) throws -> SegmentOfResult) rethrows -> [SegmentOfResult.Element] where SegmentOfResult : Sequence
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
func compactMap<ElementOfResult>(_ transform: (Element) throws -> ElementOfResult?) rethrows -> [ElementOfResult]
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

<br>


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


<br>


<br>