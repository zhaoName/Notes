# dsc_extractor
**环境配置:iPhone10.2  yalu102越狱**

本文只介绍如何用`dsc_extractor`将二进制文件从`dyld_share_cache_arm64`中取出来。

在我们简单了解逆向，会使用ida64、Hopper之后，可能会想到尝试反编译系统库文件。如查看`UIKit`框架下的`UITableView`的重用机制到底是怎么实现的。然后就兴冲冲的跑到`Xcode`的`Frameworks`路径下，接下来就很懵逼了。`UIKit`的可执行文件呢？

	/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/System/Library/Frameworks/UIKit.framework
	

原来从iOS3.1开始，包括frameworks在内的许多库文件被存在一个大cache中，这个cache文件位于手机的`System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64(或_armv7)`下，我们可以用dsc_extractor将其中的二进制文件提取出来。这样才能拿到想要的可执行文件。

### 一、准备工作

 使用iFunBox将`dyld_shared_cache_arm64`复制到电脑

使用homebrew安装wget

	$brew install wget
	
### 二、下载dyld-421.2

wget安装完成执行以下命令

	$cd ~
	
	$mkdir dsc_extractor
	
	$cd dsc_extractor
	
	$wget http://opensource.apple.com/tarballs/dyld/dyld-421.2.tar.gz
	
	$tar xvf dyld-421.2.tar.gz

如下图说明成功

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181106212520130.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)


### 三、dsc_extractor.patch

	$cd dyld-421.2/launch-cache/
	
	touch dsc_extractor.patch

然后将下面的复制到`dsc_extractor.patch`中

```
--- dyld-210.2.3/launch-cache/dsc_extractor.cpp  2012-05-21 02:35:15.000000000 -0400
+++ dyld-210.2.3/launch-cache/dsc_extractor.cpp	2013-07-26 16:05:03.000000000 -0400
@@ -37,6 +37,7 @@
 #include <mach-o/arch.h>
 #include <mach-o/loader.h>
 #include <Availability.h>
+#include <dlfcn.h>
 
 #define NO_ULEB 
 #include "Architectures.hpp"
@@ -456,7 +457,7 @@
 }
 
 
-#if 1 
+/* #if 1 */
 
 typedef int (*extractor_proc)(const char* shared_cache_file_path, const char* extraction_root_path,
 													void (^progress)(unsigned current, unsigned total));
@@ -468,7 +469,7 @@
 		return 1;
 	}
 	
-	void* handle = dlopen("/Developer/Platforms/iPhoneOS.platform/usr/lib/dsc_extractor.bundle", RTLD_LAZY);
+	void* handle = dlopen("/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/usr/lib/dsc_extractor.bundle", RTLD_LAZY);
 	if ( handle == NULL ) {
 		fprintf(stderr, "dsc_extractor.bundle could not be loaded\n");
 		return 1;
@@ -484,7 +485,7 @@
 	fprintf(stderr, "dyld_shared_cache_extract_dylibs_progress() => %d\n", result);
 	return 0;
 }
-#endif
+/* #endif */

```

然后执行如下命令：

	$patch < dsc_extractor.patch

**注意如下报错**
	
```
launch-cache zhao$ patch < dsc_extractor.patch
patching file dsc_extractor.cpp
Reversed (or previously applied) patch detected!  Assume -R? [n] yes
Hunk #2 FAILED at 456.
Hunk #3 FAILED at 468.
Hunk #4 FAILED at 484.
3 out of 4 hunks FAILED -- saving rejects to file dsc_extractor.cpp.rej
```
解决办法：找到并打开`dsc_extractor.cpp`文件，将652行`if 0`改成`if 1`，再次执行`patch < dsc_extractor.patch`命令。

### 四、编译

执行如下命令将会生成一个`dsc_extractor`可执行文件个

	$clang++ -o dsc_extractor dsc_extractor.cpp dsc_iterator.cpp

### 五、提取

在当前目录下执行

	$dsc_extractor  动态库共享缓存文件的路径  输出路径

示例

	$dsc_extractor ~/Desktop/dyld_shared_cache_arm64 ~/Desktop/systemLib
	0/1203
	1/1203
	2/1203
	3/1203
	4/1203
	5/1203
	...

然后就可以找到UIKit的可执行文件，这里注意若UIKit没有可执行权限，可用`chmod +x UIkit` 修改其权限。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181106214851759.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)

将可执行文件拖到ida64中可查看反编译代码

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181106215202582.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)


### 六、坑

- 安装wget报错，可参考[这里](https://github.com/Homebrew/brew/issues/4093)

```
~ zhao$ brew install wget
Updating Homebrew...
^C==> Installing dependencies for wget: gettext, libunistring, libidn2, openssl@1.1
Error: Calling keg_only :shadowed_by_osx is disabled!
Use keg_only :shadowed_by_macos instead.
/usr/local/Homebrew/Library/Homebrew/formula.rb:1034:in `keg_only?'
Or, even better, submit a PR to fix it!
```


**本文参考：**

http://iosre.com/t/when-dyld-decache-fails-on-dyld-shared-cache-arm64-dsc-extractor-saves-our-days/1974

https://09jianfeng.github.io/2015/04/05/dyld_decache%20%E5%AF%BC%E5%87%BA%E7%B3%BB%E7%BB%9F%E5%BA%93%E7%9A%84%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6/

-
**写于2018-11-07**

### 七、快速获取dsc_extractor可执行文件

- 下载最新版本dyld:`https://opensource.apple.com/tarballs/dyld/dyld-635.2.tar.gz`

- Xcode打开源码，找到`/dyld-635.2/launch-cache/dsc_extractor.cpp`文件
- 删除`if 0`之前的代码，再删除`#endif`
- `cd`到`dsc_extractor.cpp`所以目录

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216143515749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxMjQ0NTk2,size_16,color_FFFFFF,t_70)

- 导出

		$dsc_extractor  动态库共享缓存文件的路径  输出路径
		
**更新于2018-12-16**