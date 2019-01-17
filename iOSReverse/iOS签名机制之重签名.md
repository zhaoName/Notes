# iOS签名机制之重签名

- 查看app是否加密

		$ otool -l Mach-O文件名 | grep crypt
		
		# cryptid 为1表示加密 为0表示已砸壳
		cryptoff 16384
    	cryptsize 35323904
      	cryptid 1
  
- 查看当前电脑下证书信息

		$ security find-identity -v -p codesigning -v
		
- 查看配置文件embedded.mobileprovision信息

		$ security cms -D -i embedded.mobileprovision
		
- 赋予Mach-O文件可执行权限

		$ chmod +x Mach-O文件名
		
- Mach-O文件瘦身

		$ lipo Mach-O文件名 -thin 想要保留的架构 -output 瘦身后的Mach-O文件名
		
- 对extension重签名

		$ codesign -fs "证书信息" xxxExtension.appex/xxxExtension
		
- 对app重签名

		$ codesign -fs "证书信息"或证书ID --entitlements entitlements.plist xxx.app
		

### 0x01 重签名流程

1. 准备一个`embedded.mobileprovision`文件(必须是付费账号下生成的),放在要重签名的xxx.app包内
	
	- 可以通过Xcode自动生成，在编译后的App包内找到
	
	- 到[苹果开发者后台](https://developer.apple.com/account/ios/profile/)生成,下载

2. 从`embedded.mobileprovision`提取出`entitlements.plist`权限文件
	
	
		
