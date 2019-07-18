`theos`是可以用来编写注入、拦截别人App函数的工具。

## 一、准备工作

- 安装`brew`

```
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
	
- 利用`brew`安装签名工具`ldid`

```
$ brew install ldid
```
	
- 设置环境变量

```
export THEOS=/opt/theos

// 在所有路径下都能使用nic.pl命令(当然也可不设置 直接用/opt/theos/bin/nic.pl)
export PATH=$THEOS/bin:$PATH
```

- 使`.bash_profile`文件设置的环境变量生效

```
$ source ~/.bash_profile
```

## 二、下载Theos

不要直接到GitHub上下载，使用`recursive`命令以保证拉取到所有子模块

```
$ git clone --recursive https://github.com/theos/theos.git $THEOS
```

## 三、Tweak

### 0x01 创建`Tweak`

`Tweak`默认会在执行`nic.pl`命令的目录下生成，那你先`cd`到你常用的目录，执行`nic.pl`选择 11 创建`Tweak`

![](https://img-blog.csdnimg.cn/20181220213307573.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)

- [Choose a Template]：选择你创建`theos`的类型(必填项)
	
- [Project Name]：工程的名字(必填项)
- [Package Name]：工程ID 类似于`bundleId`
- [Author/Maintainer Name]：作者 默认是mac的用户名
- [iphone/tweak] MobileSubstrate Bundle filter：你想要修改app的bundleId
- [iphone/tweak] List of applications to terminate upon installation：直接回车默认做法就行。

### 0x02 编辑Makefile文件
	
- 在前面加入环境变量，写清楚通过那个IP和端口访问手机

```
export THEOS_DEVICE_IP=127.0.0.1

// 若是用wifi安装 则不用设置端口号
export THEOS_DEVICE_PORT=10010
```
	
- 若不想每次新建`Tweak`都添加IP和端口号，则可将其添加到`.bash_profile`中

```
$ vim ~/.bash_profile
 	
export THEOS=~/theos
export PATH=$THEOS/bin:$PATH
export THEOS_DEVICE_IP=127.0.0.1
export THEOS_DEVICE_PORT=10010
 	
$ source ~/.bash_profile
```
	 
### 0x03 编译、打包、安装

```
$ make 
		
$ make package 
	
$ make install
```		
		
### 0x04 图片

在我们编写Tweak程序中可能会用到图片资源，这时要在工程中根目录下创建一个`layout`文件夹，工程安装到手机时`layout`文件夹相当于手机的根目录。
	
![](https://img-blog.csdnimg.cn/20181220223458871.png)
   
安装之后就可以在手机的`/Library/Caches/`下找到工程中的图片,使用`UIImage`的方法获取图片

```   
[UIImage imageWithContentsOfFile:@"/Library/Caches/xxx.png"]
```

##  坑

#### 0x01 make错误 

- 由于安装多个`Xcode`导致的错误

		$ make
	
		Error: You do not have an SDK in 
		/Library/Developer/CommandLineTools/Platforms/iPhoneOS.platform/Developer/SDKs
	
 
 	解决办法：需要指定一下`Xcode`
 
		$ sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer/

- 之前编译过有缓存导致的错误

		$ make
	
		> Making all for tweak xxx...
		make[2]: Nothing to be done for `internal-library-compile'.

	解决办法：clean一下即可
	
		$ make clean
		
		$ make
		
- No such file or directory

		$ make  
		       
		Makefile:6: /makefiles/common.mk: No such file or directory
		Makefile:13: /tweak.mk: No such file or directory
		make: *** No rule to make target `/tweak.mk'.  Stop.
		
	解决办法：更改Makefile文件
		
		将 include $(THEOS)/makefiles/common.mk
		
		改为 include /opt/theos/makefiles/common.mk
	
	
	
#### 0x02 make package错误

- 打包压缩方式问题

		$ make package
		
		Can't locate IO/Compress/Lzma.pm in @INC (you may need to install the
		IO::Compress::Lzma module) (@INC contains: /Library/Perl/5.18/darwin-
		thread-multi-2level /Library/Perl/5.18 /Network/Library/Perl/5.18/darwin-
		thread-multi-2level /Network/Library/Perl/5.18 /Library/Perl/Updates/5.18.2
		/System/Library/Perl/5.18/darwin-thread-multi-2level
		/System/Library/Perl/5.18 /System/Library/Perl/Extras/5.18/darwin-thread-
		multi-2level /System/Library/Perl/Extras/5.18 .) at
		/Users/mj/theos/bin/dm.pl line 12.
		BEGIN failed--compilation aborted at /Users/mj/theos/bin/dm.pl line 12.
		make: *** [internal-package] Error 2
	
	解决办法：改成zip压缩就行
	
	- 修改`dm.pl`文件，用#注释掉下面两句
		
		 
			$ vim $THEOS/vendor/dm.pl/dm.pl
			
			#use IO::Compress::Lzma;
			#use IO::Compress::Xz;
			
	- 修改`deb.mk`第6行的压缩方式为gzip
					 
			$ vim $THEOS/makefiles/package/deb.mk
			
			_THEOS_PLATFORM_DPKG_DEB_COMPRESSION ?= gzip
			
**写于2018-12-20**
