# Method Dispatch in Swift



<br>

## 一、方法派发

编译的编程语言有三种主要的调度方法可供使用：直接调度、表调度和消息调度，我将在下面解释。 大多数语言都支持其中的一种或两种。 Java 默认使用表分派，但您可以使用 final 关键字选择直接分派。 C++ 默认使用直接分派，但您可以通过添加 virtual 关键字来选择表分派。 Objective-C 总是使用消息派发，但允许开发人员回退到 C 以获得直接派发的性能增益。 Swift 承担了支持所有三种调度类型的崇高目标。 这非常有效，但对许多开发人员来说是一个困惑的根源，并且是大多数 Swift 开发人员遇到的许多陷阱的背后。


### 0x01 Direct Dispatch

Direct dispatch (直接派发) 也称 static dispatch (静态派发)。直接派发是最快和最有效的方法分派类型。 直接分派的方法可以在运行时立即执行，因为编译器在编译时就知道确切的内存地址。 这也意味着编译器可以执行各种优化，例如内联，从而在运行时实现更快的执行时间。

直接派发的缺点是不允许子类化，动态性不好。


```Swift
struct Student {
    func directMethod() {
        print(#function)
    }
}

let stu = Student()
stu.directMethod()
```

将上述代码使用 `swiftc -emit-silgen main.swift -Onone > main.sil` 命令转换成 sil 文件

```Swift-sil
// main
sil [ossa] @main : $@convention(c) (Int32, UnsafeMutablePointer<Optional<UnsafeMutablePointer<Int8>>>) -> Int32 {
bb0(%0 : $Int32, %1 : $UnsafeMutablePointer<Optional<UnsafeMutablePointer<Int8>>>):
  alloc_global @$s4main3stuAA7StudentVvp          // id: %2
  %3 = global_addr @$s4main3stuAA7StudentVvp : $*Student // users: %8, %7
  %4 = metatype $@thin Student.Type               // user: %6
  // function_ref Student.init()
  %5 = function_ref @$s4main7StudentVACycfC : $@convention(method) (@thin Student.Type) -> Student // user: %6
  %6 = apply %5(%4) : $@convention(method) (@thin Student.Type) -> Student // user: %7
  store %6 to [trivial] %3 : $*Student            // id: %7
  %8 = load [trivial] %3 : $*Student              // user: %10
  // function_ref Student.directMethod()
  %9 = function_ref @$s4main7StudentV12directMethodyyF : $@convention(method) (Student) -> () // user: %10
  %10 = apply %9(%8) : $@convention(method) (Student) -> ()
  %11 = integer_literal $Builtin.Int32, 0         // user: %12
  %12 = struct $Int32 (%11 : $Builtin.Int32)      // user: %13
  return %12 : $Int32                             // id: %13
} // end sil function 'main'
```

- `@main` 是 `main.swift` 的入口函数，SIL 中标识符名称以 @ 作为前缀，入口函数有两个参数，一个 32 位 Int，一个指针，一个返回值。

- `%0`、`%1`......在 SIL 中叫做寄存器，可以视为长量，赋值后不可更改。在 SIL 中以累加数字的模式继续使用。这里所说的寄存器是虚拟的，最终运行到我们的机器上会使用真的寄存器。
- `alloc_global` 创建一个全局变量
- `global_addr` 拿到全局变量的地址，赋值给 `%3`
- `metatype` 拿到 `Student` 的 `Metadata` 赋值给 `%4`
- `function_ref` 将 `Student.init()` 的函数地址赋值给 `%5`
- `apply` 调用 `Student.init()` 方法，将 `%4` 传进去，返回值赋值给 `%6`
- 将 `%6` 存储到全局变量 `%3` 中，也就是 `stu`
- 读取 `%3` 的值并赋值给 `%8`
- `function_ref ` 获取 `Student.directMethod()` 的地址，赋值给 `%9`
- `apply` 调用`Student.directMethod()`，传参 `%8` 也就是 `stu`


从 `SIL` 文件可以看到是直接获取 `Student.directMethod()` 函数地址，然后调用。


### 0x02 




```Swift

```


```Swift

```

```Swift

```

<br>



<br>

```Swift

```

```Swift

```


```Swift

```


<br>

**Reference**

- [A Deep Dive Into Method Dispatches in Swift](https://betterprogramming.pub/a-deep-dive-into-method-dispatches-in-swift-65a8e408a7d0)

- [Method Dispatch in Swift](https://www.rightpoint.com/rplabs/switch-method-dispatch-table)

<br>

