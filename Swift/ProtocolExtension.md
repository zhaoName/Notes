
# Protocol Extension


<br>

### 0x01 protocol extension 可以为协议提供的默认实现

Swift 2 中引入了一个非常重要的特性，那就是 protocol extension。在 Swift 2 中我们可以对一个已有的 protocol 进行扩展，而扩展中实现的方法将作为实现扩展的类型的默认实现。

```swift
protocol MyProtocol {
    func method()
}

extension MyProtocol {
    func method() {
        print("Called in extension")
    }
}
```

在具体的实现这个接口的类型中，即使我们什么都不写，也可以编译通过。进行调用的话，会直接使用 extension 中的实现

```swift
struct MyStruct: MyProtocol { }

MyStruct().method()  // Called in extension
```

当然，如果我们需要在类型中进行其他实现的话，可以像以前那样在具体类型中添加这个方法

```swift
struct MyStruct: MyProtocol {
    func method() {
        print("Called in struct")
    }
}

MyStruct().method()  // Called in struct
```

<br>

### 0x02 调用协议扩展中方法的注意点

对于 protocol extension 来说，有一种会非常让人迷惑的情况，就是在接口的扩展中实现了接口里没有定义的方法时的情况。

```swift
protocol A1 {
    func method1() -> String
}

struct B1: A1 {
    func method1() -> String {
        return "hello"
    }
}
```

在使用的时候，无论我们将实例的类型为 A1 还是 B1，因为实现只有一个，所以没有任何疑问，调用方法时的输出都是 "hello"

```swift
// b1 is B1
let b1 = B1() 
b1.method1()  // hello

// a1 is A1
let a1: A1 = B1() 
a1.method1()  // hello
```

但是如果在接口里只定义了一个方法，而在接口扩展中实现了额外的方法的话，事情就变得有趣起来了。考虑下面这组接口和它的扩展：


```swift
protocol A2 {
    func method1() -> String
}

extension A2 {
    func method1() -> String {
        return "hi"
    }

    func method2() -> String {
        return "hi"
    }
}
```

扩展中除了实现接口定义的 `method1` 之外，还定义了一个接口中不存在的方法 `method2`。我们尝试来实现这个接口

```swift
struct B2: A2 {
    func method1() -> String {
        return "hello"
    }

    func method2() -> String {
        return "hello"
    }
}
```

`B2` 中实现了 `method1` 和 `method2`。接下来，我们尝试初始化一个 `B2` 对象，然后对这两个方法进行调用

```swift
let b2 = B2()

b2.method1() // hello
b2.method2() // hello
```

结果在我们的意料之中，虽然在 protocol extension 中已经实现了这两个方法，但是它们只是默认的实现，我们在具体实现接口的类型中可以对默认实现进行覆盖，这非常合理。但是如果我们稍作改变，在上面的代码后面继续添加:

```swift
let a2 = b2 as A2

a2.method1() // hello
a2.method2() // hi
```

`a2` 和 `b2` 是同一个对象，只不过我们通过 `as` 告诉编译器我们在这里需要的类型是 `A2`。但是这时候在这个同样的对象上调用同样的方法调用却得到了不同的结果，发生了什么？

我们可以看到，对 `a2` 调用 `method2` 实际上是接口扩展中的方法被调用了，而不是 `a2` 实例中的方法被调用。我们不妨这样来理解：对于 `method1`，因为它在 protocol 中被定义了，因此对于一个被声明为遵守接口的类型的实例 (也就是对于 `a2`) 来说，可以确定实例必然实现了 `method1`，我们可以放心大胆地用动态派发的方式使用最终的实现 (不论它是在类型中的具体实现，还是在接口扩展中的默认实现)；但是对于 `method2` 来说，我们只是在接口扩展中进行了定义，没有任何规定说它必须在最终的类型中被实现。在使用时，因为 `a2` 只是一个符合 `A2` 接口的实例，编译器对 `method2` 唯一能确定的只是在接口扩展中有一个默认实现，因此在调用时，无法确定安全，也就不会去进行动态派发，而是转而编译期间就确定的默认实现。

也许在这个例子中你会觉得无所谓，因为实际中估计并不会有人将一个已知类型实例转回接口类型。但是要考虑到如果你的一些泛型 API 中有类似的直接拿到一个接口类型的结果的时候，调用它的扩展方法时就需要特别小心了：一般来说，如果有这样的需求的话，我们可以考虑将这个接口类型再转回实际的类型，然后进行调用。

<br>

**总结**

- 如果类型推断得到的是实际的类型

	- 那么类型中的实现将被调用；如果类型中没有实现的话，那么接口扩展中的默认实现将被使用
- 如果类型推断得到的是接口，而不是实际类型

	- 并且方法在接口中进行了定义，那么类型中的实现将被调用；如果类型中没有实现，那么接口扩展中的默认实现被使用
	
	- 否则 (也就是方法没有在接口中定义)，扩展中的默认实现将被调用


<br>

本文取自王巍的 [PROTOCOL EXTENSION](https://swifter.tips/protocol-extension/)