# Optional 中的 == 运算符


<br>

### 0x01 `_OptionalNilComparisonType`

`_OptionalNilComparisonType` 是个结构体类型，它遵守 `ExpressibleByNilLiteral` 协议。意思就是我们可以用 `nil` 字面量来初始化它。

```swift
public struct _OptionalNilComparisonType: ExpressibleByNilLiteral {
    /// Create an instance initialized with `nil`.
    public init(nilLiteral: ()) { }
}
```

<br>

### 0x02 Optional 类型的值和 `nil` 比较

- 若等号左边是可选类型，右边是 `nil`，则走下面的方法：

```swift
extension Optional {
    /// Returns a Boolean value indicating whether the left-hand-side argument is `nil`.
    ///
    /// - Parameters:
    ///   - lhs: A value to compare to `nil`.
    ///   - rhs: A `nil` literal.
    static func ==(lhs: Wrapped?, rhs: _OptionalNilComparisonType) -> Bool {
        print("lhs is Wrapped value, rhs is nil")
        switch lhs {
        case .some:
            return false
        case .none:
            return true
        }
    }
}
```

```swift
let age: Int? = 10
print(age == nil)

// lhs is Wrapped value, rhs is nil
// false
```

需要注意的是等号左边可以不是可选类型，但没必要拿非可选类型的值和 `nil` 作比较，肯定是不相等的。

且非可选类型的值，会自动包装成可选类型再传给 `lhs` 。若已是可选类型，则不会再次包装。

```swift
let num: Int = 10
// Xcode ⚠️：Comparing non-optional value of type 'Int' to 'nil' always returns false
print(num == nil)

// lhs is Optional(10), rhs is nil 
// false
```


- 若等号左边是`nil`，右边是可选类型，则走下面的方法：

```swift
extension Optional {
    /// Returns a Boolean value indicating whether the right-hand-side argument is `nil`.
    ///
    /// - Parameters:
    ///   - lhs: A `nil` literal.
    ///   - rhs: A value to compare to `nil`.
    static func ==(lhs: _OptionalNilComparisonType, rhs: Wrapped?) -> Bool {
        print("lhs is nil, rhs is not nil")
        switch rhs {
        case .some:
            return false
        case .none:
            return true
        }
    }
}
```

```swift
let age: Int? = 10
print(nil == age)

lhs is nil, rhs is not nil
false
```

<br>

### 0x03 可选类型之间的比较 

可选类型之间判断是否相等，会遵守 `Equatable` 协议。

```swift
extension Optional: Equatable where Wrapped: Equatable {
    /// Returns a Boolean value indicating whether two optional instances are equal.
    ///
    /// - Parameters:
    ///   - lhs: An optional value to compare.
    ///   - rhs: Another optional value to compare.
    static func ==(lhs: Wrapped?, rhs: Wrapped?) -> Bool {
        print("lhs is \(lhs) ---- rhs is \(rhs)")
        switch (lhs, rhs) {
        case let (l?, r?):
            return l == r
        case (nil, nil):
            return true
        default:
            return false
        }
    }
}
```

"同级别的"两个可选类型比较，会直接对可选类型解包，然后判断解包后的值是否相等。

```swift
let age: Int? = 10
let num: Int? = 10
print(age == num)

lhs is Optional(10) ---- rhs is Optional(10)
true
```

也可以对一个是可选类型和一个非可选类型判断是否相等。非可选类型在比较前会包装成可选类型。然后传到 `==` 函数中，解包比较值是否相等。

```swift
let age: Int? = 10
let num: Int = 10
print(age == num)

lhs is Optional(10) ---- rhs is Optional(10)
true
```

同样，两个"不同级别的"可选类型，如一个是 `Int?`，一个是 `Int???` 。在比较前会将"低级别的"可选类型包装成同样的嵌套可选类型。

```swift
let age: Int? = 10
let num: Int??? = 10
print(age == num)

// lhs is Optional(Optional(Optional(10))) ---- rhs is Optional(Optional(Optional(10)))
// true
```



<br>

参考

- [Optional.swift](https://github.com/apple/swift/blob/main/stdlib/public/core/Optional.swift)