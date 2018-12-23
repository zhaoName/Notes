# OpenSSH

本文运行环境：iPhone6、10.2、yalu102越狱

### 0x00 简介
1. ssh
	+ `Secure Shell`的缩写，意为“安全外壳协议”，是一种可以为远程登录提供安全保障的协议 ；
	+ 使用SSH，可以把所有传输的数据进行加密，“中间人”攻击方式就不可能实现，能防止DNS欺骗和IP欺骗。

2. OpenSSH
	- 是SSH协议的免费开源实现
	- 可以通过`OpenSSH`的方式让Mac远程登录到iPhone

这里有个很重要的概念：通过`OpenSSH`的方式让Mac远程登录到iPhone时，**Mac是客户端，iPhone是服务端**。

### 0x01 安装OpenSSH

1. 对于iOS8和iOS9的设备需要在Cydia中添加软件源`http://apt.saurik.com`，然后搜索并安装`OpenSSH`。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181212224503664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)

2. 对于iOS10.0~iOS10.2的设备，则不用手动安装`OpenSSH`。因为yalu内置了一个轻量级的`OpenSSH`服务`dropbear`。在Cydia中添加软件源`http://cydia.ichitaso.com/test`，然后搜索并安装`dropbear`。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181212224748525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)

### 0x01 通过Wifi登录到iPhone

1. SSH是通过TCP协议通信，所以必须要保证mac和iPhone在同一局域网下，如连接到同一个wifi下。默认密码alpine

		 // 登录	
		 $ ssh root@iPhone ip(服务端主机地址)
		 
		 // 退出
		 $ exit (或快捷键command + d)
		 
		 // 修改密码
		 $ passwd
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215155714823.png)

2. 要想使用`OpenSSH`通信必须保证客户端和服务端的`SSH`的版本一致。 查看`SSH`版本也就是查看配置文件的Protocol字段值。

	- 客户端：`/etc/ssh/ssh_config`
	- 服务端：`/etc/ssh/sshd_config`	

3. SSH通信过程 

	 3.1建立安全连接
	 
	- 当客户端向服务端发送连接请求时，服务端会将自己的公钥等信息发送给客户端，来确保所连接的服务端是没被更改过的。客服端在`~/.ssh/known_hosts`中保存服务端的公钥信息。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213222146912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)
	- 如果客户端并无服务器端的公钥信息，就会询问是否连接此服务器(如下图)。如果客户端已保存服务端的公钥信息并和发送过来的公钥信息一致，则直接连接。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213221705760.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)
	- 验证服务端是否将公钥发送给客户端，且客户端保存在`~/.ssh/known_hosts`。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215144605174.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)
**注意：** iOS9设备的服务端公钥路径在`/etc/.ssh/ssh_host_key.pub`

	- 当出现如下错误时，提醒你服务端的身份信息发生改变(同一个ip但公钥信息不一样)，可能会被中间人攻击。![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215150053193.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)
这时可以判断指纹信息是否是你想要链接的服务器，如果是且想要继续登录到iPhone 则有以下办法
		+ 用`vim ~/.ssh/known_hosts` 删除对应ip的公钥信息
		+ 使用命令`ssh-keygen -R 服务器ip` 直接删除对应的公钥信息

	3.2 客户端认证   
&ensp;&ensp;&ensp;&ensp;SSH-2提供了基于密码的客户端验证(账号密码登录)和基于秘钥的客户端验证(免密码登录)。SSH-2优先尝试"秘钥认证"，若秘钥认证失败，才会尝试"密码认证"。

	- 流程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215152057105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)
首先客户端会生成一对公钥和私钥，然后将公钥信息拷贝到授权文件尾部。最后用私钥进行登录认证

	- 具体实现
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215153439237.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)
	- 验证是否追加到尾部
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215154542381.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)

### 0x02 通过USB登录到iPhone

　　默认情况下，由于`SSH`走的是TCP协议，Mac是通过网络连接的方式`SSH`登录到iPhone，要求iPhone连接WiFi。这时数据传输受wifi网速限制，有时输入命令卡半天都出不来。为了加快传输速度，也可以通过USB连接的方式进行SSH登录。 

　　Mac上有个服务程序`usbmuxd`（它会开机自动启动），可以将Mac的数据通过USB传输到iPhone,路径在`/System/Library/PrivateFrameworks/MobileDevice.framework/Resources/usbmuxd`下。 

　　客户端先登录到10010端口，这样客户端每次先往10010端口写数据，然后`usbmuxd`会将10010端口的数据通过USB转到22端口。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215160705282.png)

- `usbmuxd`使用
	+ 下载usbmuxd工具`https://cgit.sukimashita.com/usbmuxd.git/snapshot/usbmuxd-1.0.8.tar.gz`
	+ 将iPhone的22端口（SSH端口）映射到Mac本地的2222端口
	
		```
		$ cd ~/您保存usbmuxd路径/usbmuxd-1.0.8/python-client
		
		// -t参数是为了能够同时支持多个SSH连接
		$ python tcprelay.py -t 22:2222
		```
		
		
		![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215162556202.png)
		**注意：** 要想保持端口映射状态，不能终止此命令行（如果要执行其他终端命令行，请新开一个终端界面）

	- 新开一个终端界面，SSH登录到Mac本地的2222端口

		```
		$ ssh root@localhost -p 2222
		```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215162522745.png)

--

### 0x03 通过iproxy登录到iPhone
`usbmuxd`附带了一个小工具`iproxy`,通过它可以更方便映射端口，连接到iPhone。		

	$ iproxy 2222 22

--

### 0x04 坑

- 文件权限 

如果配置了免密码登录后，还是需要输入密码，需要在服务器端设置文件权限

	$ chmod 755 ~
	$ chmod 755 ~/.ssh
	$ chmod 644 ~/.ssh/authorized_keys
	
	
--

写于2018-12-15