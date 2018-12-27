# lldb命令

1.使程序断开 准备lldb链接

	$debugserver *:1234 -a "processName or pid"
	
2.电脑链接到手机进程

	$process connect connect://localhost:1234

3.加载模块基地址

	$image list -o -f

4.地址断点

	$br s -a '基地址+偏移地址'

5.打印调用信息

	$po $r0  :调用者（类）
	$x/s $r1 :调用方法 SEL
	$po $r2  :第一个参数
	$po $r3  :第二个参数
	
6.打印所有寄存器

	$register read
	
7.修改寄存器的值

	$register write 所要修改的寄存器 值
	
 8.读取地址上的内存
 
 	$memory read 地址
 	
 9.修改地址上的内存
 
 	$memory write 地址 值
 	
 10.查看断点
 
 	$br list
 	
 11.添加命令，在断点断下来的时候，自动执行
 
 	$br com add 断点标号(br list获得)
 	
 	命令输入完时，输入DONE+回车 退出
 	
 <font color=red>12.打印当前断点处的调用堆栈信息</font>
 
 	$bt //获取的是内存中的虚拟地址
 	
 	用p/x 虚拟地址 - 基地址 命令获取方法的偏移地址
 	

写于2018-07-14

<br><br>

- help 查看指令的用法(类似`--help`)


```
(lldb) help breakpoint

...
disable -- Disable the specified breakpoint(s) without deleting them.  If none are specified, disable all breakpoints.
list -- List some or all breakpoints at configurable levels of detail.
set -- Sets a breakpoint or set of breakpoints in the executable.
...
```

- expression：执行一个表达式

    - 常用于调试时不用重新运行程序，动态添加代码

    - 命令选项结束符- -，表示所有命令选择均设置完成，若没有命令选项- -可以省略

    - `expression`和指令`print`、`p`、`call`的效果一样

    - `expression -O`和指令`po`的效果一样

```
(lldb) expression self.view.backgroundColor = [UIColor redColor]
``` 

![输入图片说明](https://images.gitee.com/uploads/images/2018/1227/212759_b8c974c4_1355277.png "Snip20181227_4.png")


- 打印当前调用堆栈信息,类似于`bt`

```
(lldb) thread backtrace
``` 

- 让函数直接执行某个值，不会执行断点后面的代码

```
(lldb) thread return
```

- 打印当前栈帧的变量

```
(lldb) frame variable
```

- 程序继续运行

```
(lldb) thread continue 或 continue􏰁 或 c
```

- 单步运行一句代码，遇到函数不进入

```
(lldb) thread step-over 或 􏰁next 或 􏰁n
```

- 单步运行一句代码，遇到函数进入

```
(lldb) thread step-in 或 􏰁step 或 􏰁s
```

- 􏱃􏱄􏰞􏰟􏰭􏱏􏱐􏰖􏰒􏰆􏰩􏰪􏱍􏱎􏰧􏱅􏱆􏱟􏱢􏰠􏰡􏰖􏰒􏱃􏱄􏰞􏰟􏰭􏱏􏱐􏰖􏰒􏰆􏰩􏰪􏱍􏱎􏰧􏱅􏱆􏱟􏱢􏰠􏰡􏰖􏰒直接执行完当前函数的所有代码，返回到上一个函数

```
(lldb) thread step-out 或 􏰁finish
```

- 单步运行一句汇编指令，遇到函数不进入

```
(lldb) thread step-over-inst-over 或 􏰁nexti 或 􏰁ni
```

- 单步运行一句代码，遇到函数进入

```
(lldb) thread step-inst 或 􏰁stepi 或 􏰁si
```

- 给C函数设置断点

```
(lldb) breakpoint set -n 函数名
```

- 给OC方法设置断点

```
(lldb) breakpoint set -n "-[类名 方法名]"

# 例
(lldb) breakpoint set -n "-[ViewController touchesBegan:withEvent:]"
```

- 模糊查询方法

```
# -r表示正则匹配
(lldb) breakpoint set -r touchesBegan:withEvent:
```

- 给某个动态库的某个函数打断点

```
(lldb) breakpoint set -s 动态库名 -n 方法名
```



<br>