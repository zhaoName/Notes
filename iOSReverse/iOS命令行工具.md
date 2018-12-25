

# iOS命令行工具

本文测试环境：iPhone6、10.2、yalu102越狱

进到手机`/usr/bin/`目录下可以看到很多命令，如常用的`ldid`、`killall`、`find`,那这些命令是到底是咋制作的呢。今天就来了解一下　

## 一、命令行工具本质

- 将手机中的`killall`命令复制到电脑上，用`file`命令查看


```
$ file killall

killall: Mach-O executable arm
```

从上可以看出命令行的本质是一个`Mach-O`可执行文件，类似于App的可执行文件。这样我们就可以用Xcode编写用于iOS的命令行工具


## 二、命令行功能键制作

### 0x01 创建

- 打开`Xcode`正常创建一个`iOS`工程，然后将除了`main.m`、`info.plist`之外的文件全部删除。

- 修改`main.m`文件返回值`return 0;`

![输入图片说明](https://images.gitee.com/uploads/images/2018/1225/193456_846964df_1355277.png "Snip20181225_1.png")


### 0x02 使用

- 编写好的代码选择真机，然后`command+B`编译获取可执行文件

- 将可执行文件复制到手机`/usr/bin/`目录下

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




## 坑

### 0x01 命令行工具签名错误


执行命令报错

```
iPhone:~ root# TestCommandLine 

Killed: 9
```

 **解决方法**: 选择证书和`bundleId `时，要选择在苹果开发者后台手动生成的`bundleId `,不要`Xcode`自动管理生成的通用`bundleId `






