# iOS签名机制之机制简介

<br>

经过前面两篇文章:[iOS签名机制之非对称加密](https://gitee.com/zhaoName0x01/Notes/blob/master/iOSReverse/iOS签名机制之非对称加密.md) [iOS签名机制之数字签名与证书](https://gitee.com/zhaoName0x01/Notes/blob/master/iOSReverse/iOS签名机制之数字签名与证书.md)的介绍，已经了解iOS签名机制的基础 -- 数字签名相关内容，下面介绍iOS签名机制的具体流程。


发布过iOS程序的同学都知道，在发布前要去购买苹果开发者账号并到苹果后台配置`.cer`、`.mobileprovision`证书，添加`AppId`和`UDID`，并且还有`CertificateSigningRequest.certSigningRequest`、`p12`、`entitlements`等各种概念。那这些证书、概念到底在iOS签名机制中有什么作用，扮演什么角色？


### 0x01 AppStore下载App的签名验证


苹果官方会生成一对公私钥，私钥自己保存，公钥在每台iPhone设备上。当我们上传App到AppStore时, 苹果会用私钥对其进行签名。用户下载App到手机上，iPhone设备上自带的公钥就会对App签名进行验证，若验证成功则保证此App是经过苹果后台验证的，数据没有被修改。

![输入图片说明](https://images.gitee.com/uploads/images/2019/0117/094542_60a65256_1355277.png "Snip20190116_3.png")

这样就到达了苹果的要求:保证安装的每一个App都是经过苹果官方允许的。