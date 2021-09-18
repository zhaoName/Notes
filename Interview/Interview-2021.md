

## 上海镁信健康

- `viewContrllor` 生明周期
- `layoutSubview` 调用时机
- 两个类中属性名相同 相互赋值
- GET 和 POST 的区别
- 三次握手
- 堆栈区别

<br>

## 上海商米科技 2021-08-27

- 项目难点、亮点
- Charles 原理
- HTTPS 握手
- 内存管理
- `Tagged Pointer`
- `autoreleasePool`
- NSTimer 解循环引用
- socket send 之后发生什么
- socket 多路复用


<br>

## 微盟 

#### 一面 2021-08-30
- 静态库
- 私有库、资源管理
- pod install vs pod update
- 组件化
- 组件二进制化
- 对 aop 理解
- 数组的完全深拷贝
- NSTimer
- NSProxy 怎么实现解循环引用
- 卡顿检测
- objc_msgSend()
- 检测发送消息崩溃
- 链式编程 Masonry
- 除了继承 代码复用

#### 二面 2021-09-01
- cocoapods 相关
- NSNotification 实现原理
- NSTimer 循环引用
- 事件响应链
- APM、CI/CD
- 数据安全
- 合并两个有序数组

<br>

## 安居客 

##### 一面 2021-08-31

- 内存管理、AutoreleasePool
- UIView layoutSubviews
- Layout 约束冲突
- HTTPS 证书、Charles 抓包
- 自旋锁、互斥锁、条件锁
- swift 逃逸闭包和非逃逸闭包的区别
- swift associatedtype
- IMSDK 架构设计
- IMSDK 数据库表设计
- 保证消息的有序性
- 如何保证一个 SDK 的安全（逆向）

#### 二面 2021-08-31

- `UIView layoutSubviews`
- 5个群，1s收100条消息，怎么保证 tableView 的刷新
- 从 mian 函数到首页全部加载完成所需时间
- 反调试，反反调试
- HTTPS 证书、Charles 抓包
- 若崩溃到自己注入的动态库，如何隐藏

<br>

## 小红书

### 一面 2021-09-08
- 分类怎么添加属性，怎么添加 `weak` 效果的属性
- 多线程理解、锁
- `@synchronized`为什么效率低
- 如何实现文件多读单写
- `dealloc` 释放过程
- `tagged pointer` 存储格式
- `block` 什么时候 `copy` 到堆上
- https 握手过程
- `protobuf` 编码格式
- `mmkv` 原理(mmap)、数据存储格式
- 优化相关 - 大图加载优化
- leetcode 199 和 5 任选一题


<br>

## 蔚来

### 一面 2021-09-09

- swift 中 `struct` 的初始化方法和 `class` 的初始化方法
- swift `convenience init` 和 `init` 的区别
- swift 属性包装器
- swift 协议、`associatedtype `
- swift 泛型
- swift 协议的`extension` 和类的`extension` 中方法调用
- 引用计数如何存储，关联对象如何存储
- `weak` 如何实现
- `dealloc` 释放过程
- 异步线程 3秒之内成功就返回，超过3秒也返回
- runtime 消息机制用途
- runloop 用到的地方
- Mach-O 结构、静态分析、动态调试
- https 握手过程
- leetcode 53-最大子序和


### 二面 2021-09-10

- swift 中 `struct` 和 `class`  的区别
- AutoLayout `left` 和 `leading` 的区别
- swift `map`、 `flatMap`、`compactMap` 的区别
- iOS 中各种锁
- 单例模式优缺点
- 策略模式、工厂模式
- WWDC - 大图加载
- swift 中 `Combine`
- 下一份工作有什么要求

<br>

## 得物 2021-09-13

- 自我介绍
- swift 方法调用流程(类和结构体 方法调用不同)
- OC super 调用本质
- RunLoop 简介
- RunLoop 监测卡顿
- 监听崩溃
- 链表中间节点

<br>

## 币安 

### 一面 2021-09-14

- 自我介绍
- swift struct 和 class 的区别
- swift struct 和 class 内存结构的区别
- swift 中 `Set` 的底层数据结构
- swift `weak`、 `unowned` 的区别，解循环引用
- swift 方法派发
- swift 虚表结构
- K 线优化方案 从后台回来 socket 吐 1000 条数据，怎么保证无卡顿、快速绘制到界面
- 多线程、锁
- 进程、线程
- 内存优化 (我说的大图加载)
- 实时展示 FPS 值
- leetcode 53-最大子序和

<br>

## 比心 2021-09-15

### 一面

- 自我介绍
- OC 泛型的使用
- 分类中关联对象添加、存储、释放
- 子类调用 [super class] 返回内容是什么 (super 本质)
- `copy`、`strong`、`weak`、`_unsafe_unretained`、`assgin` 的区别
- weak 如何存储、释放的时候如何置为 nil
- runloop 的流程、应用
- autoreleasePool 和 runloop 有什么关系
- runtime 项目中使用到的地方
- OC 对象本质 isa、方法列表等
- 多线程、死锁问题
- 自旋锁、条件锁、递归锁
- IM 如何保证消息的准确率
- IM 如何保证消息顺序，失序后如何再次获取
- messageId 是服务端生成的话，服务端回复 ack 如何对应到某个发送的消息
- IM 中表的设计
- ipa 包的重签名


### 二面

- UBT 设计架构
- UBT 两个版本之间技术演进，如何推动优化 (protobuffer、encrypt、mmap) 
- UBT 两个版本之间性能差距，如数据压缩比、数据 I/O
- 平安好学 APP 中扮演的角色，最近项目的技术亮点
- 如何检测越狱状态
- 如何从一个 APP 中查找苹果私有接口
- 有做过优化相关的事吗 (说了大图加载优化、函数调用栈获取崩溃信息)
- 有没有做过技术分享

### 三面

- 自我介绍
- 主要围绕做过两个SDK: UBT 和 IM 来问
- UBT 设计架构，两个版本的技术演进，拿神策参考
- IM 心跳机制优化方案
- websockt 进入后台之后断开连接的问题
- 有没有做过技术分享
- 未来技术规划


<br>