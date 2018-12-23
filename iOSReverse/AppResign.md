# App重签名

1. 查看app是否加密

		$ otool -l Mach-O文件名 | grep crypt
		
		// cryptid 为1表示加密 为0表示已砸壳
  
2. 查看当前电脑下证书信息

		$security find-identity -v -p codesigning -v
		
3. 查看配置文件embedded.mobileprovision信息

		$security cms -D -i embedded.mobileprovision
		
4. 改变Mach-O文件权限

		$chmod +x Mach-O文件名
		
5. Mach-O文件瘦身

		$lipo Mach-O文件名 -thin 想要保留的架构 -output 瘦身后的Mach-O文件名
		
6. 对extension重签名

		$codesign -fs "证书信息" extension名
		
7. 对app重签名

		$codesign -fs "证书信息" --entitlements=../entitlements Mach-O文件名
		
