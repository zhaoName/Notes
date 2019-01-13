

# iOS命令行工具

本文测试环境：iPhone6、10.2、yalu102越狱

进到手机`/usr/bin/`目录下可以看到很多命令，如常用的`ldid`、`killall`、`find`,那这些命令是到底是咋制作的呢。今天就来了解一下　

<br>

## 一、命令行工具本质

- 将手机中的`killall`命令复制到电脑上，用`file`命令查看


```
$ file killall

killall: Mach-O executable arm
```

从上可以看出命令行的本质是一个`Mach-O`可执行文件，类似于App的可执行文件。这样我们就可以用Xcode编写用于iOS的命令行工具

<br>

## 二、命令行功能键制作

### 0x01 创建

- 打开`Xcode`正常创建一个`iOS`工程，然后将除了`main.m`、`info.plist`之外的文件全部删除。

- 修改`main.m`文件返回值`return 0;`

![输入图片说明](https://images.gitee.com/uploads/images/2018/1225/193456_846964df_1355277.png "Snip20181225_1.png")


### 0x02 使用

- 编写好的代码选择真机，然后`command+B`编译获取可执行文件

- 将可执行文件复制到手机`/usr/bin/`目录下（`scp` 或 ifunbox）

    ```
    $ scp -P 2222 ~/Desktop/TestCommandLine root@localhost:/usr/bin/

    TestCommandLine                     100%  161KB  12.7MB/s   00:00
    ```

- `ssh`登录到手机并赋予其可执行权限

    ```
    iPhone:~ root# chmod +x /usr/bin/TestCommandLine
    ```
- 执行命令(就是你创建iOS工程时起的名字)

    ```
    # 获取手机中安装的所有App名字和bundleId
    iPhone:~ root# TestCommandLine

    yalu                   ca.yalu.g2
    个人所得税               cn.gov.tax.its
    Cydia                  com.saurik.Cydia
    Ximalaya               com.gemd.iting
    咖咖播放器               com.kka.video
    CTMediatorDemo         zhao.CTMediatorDemo
    触动精灵                com.touchsprite.ios
    ```

### 0x02 main函数参数解释

- argc : 参数的个数

- argv[] : 存放参数的数组

- argv[0] : 当前可执行文件的路径


```
main.m

nt main(int argc, char * argv[]) {
    @autoreleasepool {
        
        if (argc == 3) {
            printf("参数个数：%d\n 参数：%s\n %s\n %s\n", argc, argv[0], argv[1], argv[2]);
        }
        return 0;
    }
}
```

在终端执行命令`TestCommandLine -l -d`


```
iPhone:~ root# TestCommandLine -l -d
参数个数：3
参数：TestCommandLine
 -l
 -d
```
那我们就可以判断当`argc=1`的时候，实现类似`--help`的功能


<br>

## 三、权限

**更新于2019-01-13**

使用Xcode编译出来的命令行工具，和APP一样只能访问自己的沙河路径，权限太少。这样编写出来的命令行工具用处就不大，那咋办呢。。。

我们都知道iPhone中有个APP叫`SpringBoard` -- 也就是iPhone的桌面管理App,所有的App都是放在`SpringBoard`上的。那可以猜测`SpringBoard`的权限应该会有很高，我们拿到`SpringBoard`的权限重签给我们自己编写的命令行工具不就可以了吗！！！

- 查看`Command + B`编译的命令行原始权限

    ```
    # 会在当前目录生成t.entitlements文件
    $ ldid -e TestCommandLine > t.entitlements
    ```

    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0113/144225_873471d8_1355277.png "Snip20190113_9.png")


- 在手机`/System/Library/CoreServices/`目录下找到`SpringBoard.app`文件夹，将其可执行文件复制到电脑，查看其权限，部分截图如下

    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0113/144911_579561ba_1355277.png "Snip20190113_10.png")


- 将`SpringBoard`的权限重签给编译好的命令行


    ```
    $ ldid -Ss.entitlements TestCommandLine 
    ```

这样对命令行重签名之后，其权限就很高了，可以访问任意App。


<br>

## 四、Makefile

每次都用`command+B`编译然后去去ipa包中找可执行文件有点麻烦，我们可以用`Makefile`来装个逼(有点类似Tweak安装)

- 创建Makefile

```
# Config
ARCH = arm64
IOS_VERSION = 8.0
EXECUTABLE_NAME = TestCommandLine

# 生成命令行的路径
RELEASE_DIR = Release
# 工程目录
PROJECT_DIR = TestCommandLine
# 命令行名字
EXECUTABLE_FILE = $(RELEASE_DIR)/$(EXECUTABLE_NAME)


# 头文件路径
HEADER_DIR1 = $(PROJECT_DIR)
HEADER_DIR2 = $(PROJECT_DIR)/AppInfo


# Source Files 
SOURCE_FILES = $(PROJECT_DIR)/*.m
SOURCE_FILES += $(HEADER_DIR2)/*.m


# 签名所用权限文件Entitlements
ENTITLEMENTS_FILE = $(RELEASE_DIR)/TestCL.entitlements


# 签名
codesign: compile
	@codesign -fs- --entitlements $(ENTITLEMENTS_FILE) $(EXECUTABLE_FILE)

# 编译
compile: 
	@xcrun -sdk iphoneos \
		clang -arch $(ARCH) \
		-mios-version-min=$(IOS_VERSION) \
		-fobjc-arc \
		-framework Foundation \
		-framework UIKit \
		-framework MobileCoreServices \
		-Os \
		-I $(HEADER_DIR1) \
		-I $(HEADER_DIR2) \
		$(SOURCE_FILES) \
		-o $(EXECUTABLE_FILE)
```

- 在Xcode的`your project > target > Build Phases`中添加`Run Script`

    ![输入图片说明](https://images.gitee.com/uploads/images/2018/1226/220015_55f6c90e_1355277.png "Snip20181226_2.png")

- `Command+B`编译Xcode或用Terminal进入到当前目录执行`make`,就会在`TestCommandLine/Release/`下生成命令行

    ![输入图片说明](https://images.gitee.com/uploads/images/2018/1226/220331_3fe11ca0_1355277.png "Snip20181226_3.png")

<br>

## 坑

### 0x01 命令行工具签名错误


执行命令报错

```
iPhone:~ root# TestCommandLine 

Killed: 9
```

 **解决方法**: 选择证书和`bundleId `时，要选择在苹果开发者后台手动生成的`bundleId `,不要`Xcode`自动管理生成的通用`bundleId `


写于2018-12-24 完成与2018-12-26

<br>

**更新于2019-01-13**

在iOS11执行命令行工具报错

```
iPhone:~ root# TestCommandLine 

Killed: 9
```
**解决方法**:给命令行加入新权限`platform-application`

```
<key>platform-application</key>
<true/>
```