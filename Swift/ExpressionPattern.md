# Expression Pattern (表达式模式)

<br>

### 0x01 表达式模式

表达式模式表示表达式的值。表达式模式仅出现在 `switch` 语句 `case` 标签中。

表达式模式代表的表达式会使用 Swift 标准库中的 `~=` 运算符与输入表达式的值进行比较。如果 `~=` 运算符返回 `true`，则匹配成功。默认情况下，`~=` 运算符使用 `==` 运算符来比较两个相同类型的值。它也可以将一个整型数值与一个 `Range` 实例中的一段整数区间做匹配。如下：

```swift
let point = (1, 2)
switch point {
case (0, 0):
    print("(0, 0) is at the origin")
case (-2...2, -2...2):
    print("(\(point.0), \(point.1)) is at the origin")
default:
    print("The Point is at (\(point.0), \(point.1))")
}

// (1, 2) is at the origin
```

<br>

### 0x02 自定义表达式模式

可以重载 `~=` 运算符来提供自定义的表达式匹配规则

```swift
struct Student {
    var score = 0, name = ""
    
    // pattern: case 后面的内容
    // value: switch 后面的内容
    static func ~= (pattern: Int, value: Student) -> Bool {
        print("~= Int:", value.score, pattern)
        return value.score >= pattern
    }
    
    static func ~= (pattern: Range<Int>, value: Student) -> Bool {
        pattern.contains(value.score)
    }
    
    static func ~= (pattern: ClosedRange<Int>, value: Student) -> Bool {
        pattern.contains(value.score)
    }
}

let stu = Student(score: 89, name: "zhao")
switch stu {
case 100: print(">= 100")
case 90: print(">= 90")
case 80..<90: print("[80 90)")
case 60..<79: print("[60 79)")
case 0: print("> 0")
default: break
}

// ~= Int: 89 100
// ~= Int: 89 90
// [80 90) 

```

`case` 后面还可以跟着函数

```swift
func hasPrefix(_ prefix: String) -> ((String) -> Bool) {
    return { str -> Bool in
        str.hasSuffix(prefix)
    }
}

let fn: ((String) -> Bool) = hasPrefix("z")
print(fn("zhao")) // true
```

```swift
extension String {
    static func ~= (pattern: (String) -> Bool, value: String) -> Bool {
        pattern(value)
    }
}

func hasPrefix(_ prefix: String) -> ((String) -> Bool) {
    { $0.hasPrefix(prefix)}
}

func hasSuffix(_ suffix: String) -> ((String) -> Bool) {
    { $0.hasSuffix(suffix)}
}

let str = "zhao"
switch str {
case hasPrefix("z"):
    print("以 z 开头")
case hasSuffix("o"):
    print("以 o 结尾")
default: break
}

// 以 z 开头
```


<br>


<br>


<br>

参考：

- [Pattern](https://docs.swift.org/swift-book/ReferenceManual/Patterns.html)