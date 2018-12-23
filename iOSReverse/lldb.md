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
 	
 
