# Error Handling

错误处理 (Error Handling) 是对程序中的错误情况进行响应和恢复的过程。Swift 在运行时提供了抛出、捕获、传递和操作可恢复错误（recoverable errors）的一等支持（first-class support）。


<br>

## 一、


<br>

## 二、

<br>


## 三、`defer`

有些时候我们需要在结束某个代码块执行前 (无论是由于抛出错误而离开，或是由于诸如 `return`、`break` 的语句)，做一些必要的清理工作，如释放内存、关闭文件。

`defer` 语句将代码的执行延迟到当前的作用域退出之前，延迟执行的语句不能包含任何控制转移语句(如 `break`、`return` 语句)。

**延迟执行的操作会按照它们声明的顺序从后往前执行。**

```swift
func processFile(filename: String) throws {
    if exists(filename) {
        let file = open(filename)
        defer {
            close(file)
        }
        while let line = try file.readline() {
            // 处理文件。
        }
        // close(file) 会在这里被调用，即作用域的最后。
    }
}
```

<br>



<br>

参考：

- [Error Handling](https://docs.swift.org/swift-book/LanguageGuide/ErrorHandling.html)

- [错误处理](https://swiftgg.gitbook.io/swift/swift-jiao-cheng/17_error_handling)

<br>