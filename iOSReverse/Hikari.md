# Hikari

<br>

## 一、安装

- 到 `https://github.com/HikariObfuscator/Hikari/releases`,下载最新版`.pkg`格式的安装包

- 双击安装包，将`Hikari.xctoolchain`安装到`~/Library/Developer/Toolchains/`或`/Library/Developer/Toolchains/`目录下。区别是前者只有当前用户可用，后者所有用户都可使用。

- 重启`Xcode`就能在`Xcode -> Toolchains`中看到`Hikari`


## 二、使用


### 0x01 使用前提

- 选择`Hikari`编译器 `Xcode -> Toolchains -> Hikari`

- 关闭`Xcode`编译器优化, `Target -> Build Settings -> Optimization Level`的值改为`None[-O0]`

- 关闭`bitcode`, `工程 -> Build Settings -> Enable Bitcode`的值改为`NO`

- 将所有要参与编译的taget(包括pod进来的三方库)的`Index-While-Building`改为`NO`
	- 工程在`Target -> Build Settings -> Enable Index-While-Building`
	
	- 三方库在`Pods -> AFNetworking -> Build Settings -> Enable Index-While-Building`


### 0x02 全局混淆

全局设置混淆要在`Target -> Build Settings -> Other C Flags`加上混淆标记

- `-mllvm -enable-bcfobf` 		启用伪控制流  
- `-mllvm -enable-cffobf`		启用控制流平坦化
- `-mllvm -enable-splitobf` 	启用基本块分割  
- `-mllvm -enable-subobf` 		启用指令替换  
- `-mllvm -enable-acdobf` 		启用反class-dump  
- `-mllvm -enable-indibran `	启用基于寄存器的相对跳转，配合其他加固可以彻底破坏IDA/Hopper的伪代码(俗称F5)  
- `-mllvm -enable-strcry `		启用字符串加密  
- `-mllvm -enable-funcwra 	`	启用函数封装
- `-mllvm -enable-allob`      一次性启用上述所有标记

然后`Command + B`编译就可以对项目进行混淆了。


### 0x03 混淆指定函数、方法

- 混淆`C/C++`函数,

```
int foo() __attribute((__annotate__(("fla"))));
int foo() {
   return 2;
}
```

- 混淆`OC`方法

新建继承自`NSObject`的`Const`文件

```
// Const.h
extern void hihari_fla(void) __attribute((__annotate__(("fla"))));
extern void hihari_strenc(void) __attribute((__annotate__(("strenc"))));
extern void hikari_bcf(void) __attribute((__annotate__(("bcf"))));

// Const.m
void hihari_fla(void){};
void hihari_strenc(void){};
void hikari_bcf(void){};
```

然后在想要混淆的方法中调用`Const`中声明的函数

```
- (void)fetchData
{
    hihari_fla();
    hihari_strenc();
    hikari_bcf();
    
    ...
}
```

- 混淆执行函数支持标记

	- sub　　　　指令替换 (Instruction Substitution)
	
	- bcf　　　　伪控制流 (Bogus Control Flow)
	
	- fla　　　　　控制流平坦化 (Control Flow Flattening)

	- fco　　　　函数调用逻辑混淆(Function Call Obfuscate)

	- fw　　　　　函数封装 (FunctionWrapper）

	- indibr　　　启用基于寄存器的相对跳转，配合其他加固可以彻底破坏IDA/Hopper的伪代码(俗称F5)(Indirect Branching)

	- split　　　　基本块分割 (Split Basic Block)

	- strenc　　　字符串加密 (String Encryption)

![输入图片说明](https://images.gitee.com/uploads/images/2019/0416/192816_ad625b0a_1355277.png "Screen Shot 2019-04-16 at 5.54.59 PM.png")


## 三、AppStore


由于使用的是`Hikari`编译器不是苹果默认的`XcodeDefault`编译器，所以将程序上传到App Store时会出问题。如`Upload`按钮是灰色的等。解决办法有两种

- 将`Hikari.xctoolchain/usr`覆盖到`/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr`。**覆盖前一定要先备份`XcodeDefault.xctoolchain/usr`**。然后使用默认`XcodeDefault.xctoolchain`正常打包上传

- 使用`Hikari`编译器`archive`后，找到生成的`.xcarchive`文件，右键`show Package Contents`,里面的`info.plsit`中有`DefaultToolchainOverrideInfo`,删掉它，再上传

![输入图片说明](https://images.gitee.com/uploads/images/2019/0416/192730_34ca6741_1355277.png "Snip20190416_1.png")

<br>

写于2019-04-16



