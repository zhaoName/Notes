
##掉钱眼儿分渠道打包脚本简介



#### 0x01 渠道名设置

- 在`ChannelList.txt`中设置渠道名 可多个 如下：

    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0226/180642_548b4111_1355277.png "Screen Shot 2019-02-26 at 5.21.57 PM.png")

<br>

#### 0x02 脚本简介

只介绍需要修改的部分

- 每个马甲包会对应不同的工程名，分渠道打包时，要修改脚本代码

	```
	# 改为对应马甲包的工程名 现在为SoundRecord
	name="SoundRecord" 
	```

- 换账号或正在使用的账号被封时，需修改

	```
	codesign -fs 证书信息 --entitlements "entitlements.plist" "Payload/${appName}"
	```
	
- 证书信息可用如下方法获取,选择对应证书信息

	```
	$ security find-identity -v -p codesigning -v 
	
	1) 1113EE5AB04F7A08EB111C99BF2B144EAB80DA2A "iPhone Distribution: Kathleen Dewar (F55XASQ42S)"
	2) D5326C77B7941AC7EC15197E981E571EEFC0E73A "iPhone Developer: Kathleen Dewar (DB5295W2PC)"
	```


#### 0x03 脚本使用


- 脚本运行前：

    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0226/180728_8365634e_1355277.png "Screen Shot 2019-02-26 at 5.20.32 PM.png")


- 脚本运行后:

    ![输入图片说明](https://images.gitee.com/uploads/images/2019/0226/180808_cb95d19f_1355277.png "Screen Shot 2019-02-26 at 5.23.01 PM.png")





