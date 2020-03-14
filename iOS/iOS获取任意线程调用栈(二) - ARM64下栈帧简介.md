# iOS获取任意线程调用栈(二) - ARM64下栈帧简介

<br>

由 [iOS获取任意线程调用栈(一) - 8086 CPU下栈帧简介](https://github.com/zhaoName/Notes/blob/master/iOS/iOS%E8%8E%B7%E5%8F%96%E4%BB%BB%E6%84%8F%E7%BA%BF%E7%A8%8B%E8%B0%83%E7%94%A8%E6%A0%88(%E4%B8%80)%20-%208086CPU%E4%B8%8B%E6%A0%88%E5%B8%A7%E7%AE%80%E4%BB%8B.md) 知道了 8086CPU 环境函数调用过程栈的变化和栈帧的概念。本篇文章介绍 ARM64 环境下函数调用过程栈的变化和栈帧的概念，从而得出为什么可以根据当前栈帧找到函数调用栈。

## 一、ARM64 寄存器简介

### 0x01 通用寄存器

- 64bit 的寄存器 x0 ~ x28 (w0 ~ w28 是 x0 ~ x28 的低32位)

- x0 ~ x7 一般用于存放参数，参数超过7个就存放在栈中

- x0 一般用做函数返回值(8086 用 ax 存储函数返回值)

### 0x02 程序计数器

- pc􏰍 (Program Counter􏰑): 程序计数器

- 存储着当前 CPU 正在执行指令的地址 类似于 8086 的 ip 

### 0x03 堆栈指针

- sp (Stack Pointer): 指向栈顶

- fp (Frame Pointer): 也就是 x29, 类似 8086 的 bp


### 0x04 链接寄存器

- lr (Link Register): 也就是x30 存储着函数的返回地址

- 类似 8086 `call func_name`将函数返回地址入栈，ARM64 直接将返回地址存储在专门的寄存器中


### 0x05 程序状态器

- cpsr􏰍(Current Program Status Register􏰑): 包含条件码标志，中断禁止位，当前处理器模式以及其他状态和控制信息。

- spsr􏰍(Saved Program Status Register􏰑): 异常状态下使用


<br>

## 二、


<br>


<br>

<br>


<br>

<br>

