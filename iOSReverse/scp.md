
# scp

`scp`是 `secure copy`的缩写, `scp`是`linux`系统下基于`ssh`登陆进行安全的远程文件拷贝命令。

#### 0x00语法
```
scp [可选参数] file_source file_target 
```
#### 0x01参数

```
-1： 强制scp命令使用协议ssh1
-2： 强制scp命令使用协议ssh2
-4： 强制scp命令只使用IPv4寻址
-6： 强制scp命令只使用IPv6寻址
-B： 使用批处理模式（传输过程中不询问传输口令或短语）
-C： 允许压缩。（将-C标志传递给ssh，从而打开压缩功能）
-p：保留原文件的修改时间，访问时间和访问权限。
-q： 不显示传输进度条。
-r： 递归复制整个目录。
-v：详细方式显示输出。scp和ssh(1)会显示出整个过程的调试信息。这些信息用于调试连接，验证和配置问题。
-c cipher： 以cipher将数据传输进行加密，这个选项将直接传递给ssh。
-F ssh_config： 指定一个替代的ssh配置文件，此参数直接传递给ssh。
-i identity_file： 从指定文件中读取传输时使用的密钥文件，此参数直接传递给ssh。
-l limit： 限定用户所能使用的带宽，以Kbit/s为单位。
-o ssh_option： 如果习惯于使用ssh_config(5)中的参数传递方式，
-P port：注意是大写的P, port是指定数据传输用到的端口号
-S program： 指定加密传输时所使用的程序。此程序必须能够理解ssh(1)的选项
```
#### 0x02安装
对iOS10的用户来说，使用yalu越狱后就没有scp这个工具了。这时我们可以取巧。
 + 安装`OpenSSH`，然后到手机目录`/usr/bin/scp`，将`scp`复制到电脑备用。
+ 将`OpenSSH`卸载(iOS10 yalu越狱的设备不需要安装)。
+ 将电脑上的`scp`复制到手机的`/usr/bin/`目录下
+ `ssh`登录到手机执行以下命令 赋予其权限
	 
```
$ cd /usr/bin/
$ ldid -S scp
$ chmod 777 scp
```

#### 0x03使用

- wifi

	```
	$ scp 本地文件 root@ip:目标路径
	```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215191416494.png)

- USB

	- iPhone默认是使用22端口进行SSH通信，采用的是TCP协议（可以查看/etc/ssh/sshd_config的Port字段）。

	- 要保证开启端口映射`iproxy 2222 22`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215192457905.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)



写于2018-12-15