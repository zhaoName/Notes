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

### 0x01 重签名流程

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
	
	
	
### 0x02 动态库注入
	
	
		