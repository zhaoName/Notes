

# class-dump

`class-dump`的作用就是把`Mach-O`文件的class信息给dump出来（把类信息给导出来），生成对应的.h头文件

- 官网：`http://stevenygard.com/projects/class-dump/`
- 源码：`https://github.com/nygard/class-dump`

#### 0x01 安装

- class-dump的下载地址：`http://stevenygard.com/download/class-dump-3.5.tar.gz`

- 点击下载后解压后会有`class-dump`和源码文件。将`class-dump` 复制到`/usr/bin/class-dump`。如果是OS X 10.11以后的mac版本，因为没有`/usr/bin`文件夹的写权限，所以将`class-dump`复制到`/usr/local/bin/class-dump`,这样也能在终端的任一位置使用`class-dump`命令。

#### 0x02 使用

	$ class-dump -H 需要导出的框架路径 -o 导出的头文件存放路径

写于2018-12-16
