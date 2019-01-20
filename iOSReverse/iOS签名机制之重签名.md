# iOS签名机制之重签名

- 查看app是否加密

    ```
    $ otool -l Mach-O文件名 | grep crypt
		
    # cryptid 为1表示加密 为0表示已砸壳
    cryptoff 16384
    cryptsize 35323904
    cryptid 1
    ```

- 查看当前电脑下证书信息

    ```
    $ security find-identity -v -p codesigning -v
    ```
		
- 查看配置文件embedded.mobileprovision信息

    ```
    $ security cms -D -i embedded.mobileprovision
    ```

- 赋予Mach-O文件可执行权限

    ```
    $ chmod +x Mach-O文件名
    ```

- Mach-O文件瘦身
    
    ```
    $ lipo Mach-O文件名 -thin 想要保留的架构 -output 瘦身后的Mach-O文件名
    ```

- 对extension重签名

    ```
    $ codesign -fs "证书信息" xxxExtension.appex/xxxExtension
    ```

- 对app重签名
    
    ```
    $ codesign -fs "证书信息"或证书ID --entitlements entitlements.plist xxx.app
    ```	

## 一、重签名流程

1. 准备一个`embedded.mobileprovision`文件(必须是付费账号下生成的),放在要重签名的xxx.app包内
	
    - 可以通过Xcode自动生成，在编译后的App包内找到
	
        ![输入图片说明](https://images.gitee.com/uploads/images/2019/0118/111805_debde842_1355277.png "Snip20190117_10.png")

    - 到[苹果开发者后台](https://developer.apple.com/account/ios/profile/)生成,下载

    - 无论选哪种方式，最好`embedded.mobileprovision`中的`bundleId`为通配符`*`，否则要和App包内`Info.plist`中的`bundleId`一致。

2. 从`embedded.mobileprovision`提取出`entitlements.plist`权限文件
	
    ```
    # 转化为plist
    $ security cms -D -i embedded.mobileprovision > temp.plist
	
    # 只保留Entitlements
    $ /usr/libexec/PlistBuddy -x -c 'Print :Entitlements' temp.plist > entitlements.plist
    ```
    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0118/112034_459d44e4_1355277.png "Snip20190117_12.png")  

    一般都是对别人的App重签名，假如是微信，那`application-identifier`值应改为`your account teamId.com.tencent.xin`。
    

3. 查看当前电脑所有可用证书

	```
	$ security find-identity -v -p codesigning
	
	# 前面一串为证书ID 后面双引号内为证书信息
	1) 1113EE5AB04F7A08EB111C99BF2B144EAB80DA2A "iPhone Distribution: Kathleen Dewar (F55XASQ42S)"
 	2) D5326C77B7941AC7EC15197E981E571EEFC0E73A "iPhone Developer: Kathleen Dewar (DB5295W2PC)"
	```

4. 对动态库重签名

	```
	# 注意所选证书ID 要和embedded.mobileprovision内的保持一致
	$ codesign -fs 证书ID或"证书信息" xxx.dylib
	```
	
5. 对Entension重签名

	```
	# 注意所选证书ID 要和embedded.mobileprovision内的保持一致
	$ codesign -fs 证书ID或"证书信息" xxxExtension.appex/xxxEntension
	```
	
6. 对App重签名
	
	```
	# 注意所选证书ID 要和embedded.mobileprovision内的保持一致
	$ codesign -fs 证书ID或"证书信息" --entitlements entitlements.plist xxx.app
	
	xxx.app：replacing existing signature
	```
	
	
	
## 二、动态库注入

　　当我们用`Theos`开发好一些好玩的插件(如微信抢红包)，会将其分享给我们的小伙伴。这时你可能兴冲冲的跑到手机`/Library/MobileSubstrate/DynamicLibraries/`下找到自己编写的`xxx.dylib`, 复制到App包内。然后用上述方法对其重签名 安装到手机会发现，然并卵 编写好的插件效果一个都没有。
　　

![输入图片说明](https://images.gitee.com/uploads/images/2019/0120/173718_e0463823_1355277.png "Snip20190120_2.png")

　　如上图，可以看到App启动后，弹框并没有出现！那这是为什么呢？动态库`Tweak_Test.dylib`已经在App包内，重签名也是好的，App也能正常运行，插件的效果呢？会不会是App的可执行文件根本就没执行动态库呢！

### 0x01 查看Mach-O文件加载的所有动态库

- 通过命令查看

	```
	$ otool -L TweakTest（Mach-o文件）

	TweakTest:
		/System/Library/Frameworks/Foundation.framework/Foundation (compatibility version 300.0.0, current version 1452.23.0)
		/usr/lib/libobjc.A.dylib (compatibility version 1.0.0, current version 228.0.0)
		/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1252.50.4)
	/System/Library/Frameworks/UIKit.framework/UIKit (compatibility version 1.0.0, current version 3698.52.10)
	```

- 通过 MachOView 查看


    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0120/173810_748f5e06_1355277.png "Snip20190120_3.png")

　　通过上面两种方式都能看到Mach-O文件并没有加载我们所写的`Tweak_Test.dylib`动态库，所以也就不会出现动态库中的弹框。
　　

### 0x02 Insert_dylib

`insert_dylib`是将动态库注入到Mach-O文件中的工具。可到[这里](https://github.com/Tyilo/insert_dylib)下载。

- 使用`Xcode`编译代码得到可执行文件`insert_dylib`,并将其复制到`/usr/local/bin/`目录下

- 命令行输入`insert_dylib`就可以看到其用法

	```
	$ insert_dylib
	
	# insert_dylib 动态库全路径 Mach-O文件 新Mach-O文件(可选) 可选参数
	Usage: insert_dylib dylib_path binary_path [new_binary_path]
	Option flags: --inplace --weak --overwrite --strip-codesig --no-strip-codesig --all-yes
	```

- 动态库注入

	```
	# @executable_path表示可执行文件所在目录
	$ insert_dylib @executable_path/Tweak_Test.dylib TweakTest
	
	# 若不想每次输入y 可在上句命令中加入 --all-yes
	TweakTest_patched already exists. Overwrite it? [y/n] y
	LC_CODE_SIGNATURE load command found. Remove it? [y/n] y
	Added LC_LOAD_DYLIB to TweakTest_patched
	```

- 执行上述命令后会在当前目录自动生成一个`TweakTest_patched`文件，查看其动态库加载

	```
	$ otool -L TweakTest_patched 
	
	TweakTest_patched:
	/System/Library/Frameworks/Foundation.framework/Foundation (compatibility version 300.0.0, current version 1452.23.0)
	/usr/lib/libobjc.A.dylib (compatibility version 1.0.0, current version 228.0.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1252.50.4)
	/System/Library/Frameworks/UIKit.framework/UIKit (compatibility version 1.0.0, current version 3698.52.10)
	# 可看到Tweak_Test.dylib已经被加载到可执行文件中
	@executable_path/Tweak_Test.dylib (compatibility version 0.0.0, current version 0.0.0)
	```
	
- 这样生成的Mach-O文件，每次就会去加载`Tweak_Test.dylib`,若找不到`Tweak_Test.dylib`程序就会崩溃。可以加个参数 让其不崩~

	```
	$ insert_dylib @executable_path/Tweak_Test.dylib TweakTest --weak
	```
	

### 0x03 插件依赖非系统动态库

我们自己用`Theos`编写的`.dylib`文件都会依赖非系统的动态库`CydiaSubstrate`。

![输入图片说明](https://images.gitee.com/uploads/images/2019/0120/173948_b567b738_1355277.png "Snip20190120_5.png")

从上图可以看出`Tweak_Test.dylib`依赖于`CydiaSubstrate`，那如果我们将注入了`Tweak_Test.dylib`的App装入到非越狱机上就会闪退。因为非越狱机上是没有`/Library/Frameworks/CydiaSubstrate.framework`这个目录，更没有`CydiaSubstrate`这个动态库的。


- 到越狱机的`/Library/Frameworks/CydiaSubstrate.framework/`目录下找到`CydiaSubstrate `，将其复制到电脑上的APP包内

- 用`install_name_tool`命令修改Mach-O文件中的动态库加载地址

    ```
    # @loader_path 表示动态库所在目录
    $ install_name_tool -change /Library/Frameworks/CydiaSubstrate.framework/CydiaSubstrate @loader_path/CydiaSubstrate Tweak_Test.dylib 
    ```

    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0120/174036_0af30b3d_1355277.png "Snip20190120_6.png")
	
<br>
最后再对动态库和App重签名，安装到手机就能看到自己编写的插件


![输入图片说明](https://images.gitee.com/uploads/images/2019/0120/174109_6bf184ae_1355277.png "Snip20190120_7.png")
	
	
<br>
完成于2019-01-20
<br>