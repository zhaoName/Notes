# iOS签名机制之机制简介

<br>

经过前面两篇文章:[iOS签名机制之非对称加密](https://gitee.com/zhaoName0x01/Notes/blob/master/iOSReverse/iOS签名机制之非对称加密.md)、[iOS签名机制之数字签名与证书](https://gitee.com/zhaoName0x01/Notes/blob/master/iOSReverse/iOS签名机制之数字签名与证书.md)的介绍，已经了解iOS签名机制的基础 -- 数字签名相关内容，下面介绍iOS签名机制的具体流程。


发布过iOS程序的同学都知道，在发布前要去购买苹果开发者账号并到苹果后台配置`.cer / .mobileprovision`，添加`AppId`和`UDID`，并且还有`CertificateSigningRequest.certSigningRequest`、`p12`、`entitlements`等各种概念。那这些证书、概念到底在iOS签名机制中有什么作用，扮演什么角色？


### 0x01 AppStore下载App的签名验证


苹果官方会生成一对公私钥，私钥自己保存，公钥在每台iPhone设备上。当我们上传App到AppStore时, 苹果会用私钥对其进行签名。用户下载App到手机上，iPhone设备上自带的公钥就会对App签名进行验证，若验证成功则保证此App是经过苹果后台验证的，数据没有被修改，App能正常安装到手机上。

![输入图片说明](https://images.gitee.com/uploads/images/2019/0117/094542_60a65256_1355277.png "Snip20190116_3.png")

这样就到达了苹果的要求:保证安装的每一个App都是经过苹果官方允许的。

但实际上因为除了从 AppStore 下载，我们还可以有三种方式安装一个 App: 

- 开发 App 时可以直接把开发中的应用安装进手机进行调试。
- In-House 企业内部分发，可以直接安装企业证书签名后的 APP。
- AD-Hoc 相当于企业分发的限制版，限制安装设备数量，较少用

### 0x02 开发中的App安装进手机

在开发过程中，我们要随时安装App到手机，以便查看。但每次安装都要上传到App Store是不现实的。苹果又要保证对安装到手机上的App有绝对控制权，只凭上述方法显然是不行的。为解决这个问题，苹果提出**双重认证**解决方案，具体流程如下

![输入图片说明](https://images.gitee.com/uploads/images/2019/0117/162329_25c5b5eb_1355277.png "Snip20190117_4.png")

- Mac电脑上生成一对公私钥，苹果后台生成一对公私钥并自己保存私钥，公钥保存在每台iPhone设备上；

- 用苹果私钥去签名本地Mac的公钥，生成证书（上传`CertificateSigningRequest.certSigningRequest`到苹果后台生成`.cer`证书）；

- 本地Mac的私钥对App进行签名(用Xcode编译App)，然后将证书和签名一起打包到手机；

- 安装时手机上的公钥会验证证书的签名是否正确

- 验证证书后确保了Mac公钥是苹果认证过的，再用Mac公钥去验证 APP 的签名，这里就间接验证了这个 APP 安装行为是否经过苹果官方允许。


上述流程虽然能保证安装到手机的App都是经过苹果允许的，但新问题又来了 -- 所有的iPhone能安装。那要咋办呢？

你也可能灵光一闪，平时除了生成`.cer`证书之外，还要注册`UDID`、`App ID`，注册`App ID`时还要勾选`APNS`之类的权限。最后会生成一个包含`.cer`、`UDID`、`App ID`、`entitlements`的`.mobileprovision`文件。


### 0x03 最终流程

![输入图片说明](https://images.gitee.com/uploads/images/2019/0117/170111_15184dd7_1355277.png "Snip20190117_5.png")


- Mac电脑上生成一对公私钥，苹果后台生成一对公私钥并自己保存私钥，公钥保存在每台iPhone设备上；

- 用苹果私钥去签名本地Mac的公钥，生成证书（上传`CertificateSigningRequest.certSigningRequest`到苹果后台生成`.cer`证书）；

- 在苹果后台注册`App ID`并勾选APP所需权限，上传`UDID`配置设备列表，再加上上述步骤生成的证书，组成的数据用苹果的私钥进行签名，生成`.mobileprovision`文件

- 编译完APP后，用Mac私钥对这个APP进行签名，同时把上面得到的`.mobileprovision`文件打包进 APP 里，文件名为`embedded.mobileprovision`，把 APP 安装到手机上。

- 安装时手机上的公钥会验证`embedded.mobileprovision`的签名是否正确，若正确再去验证里面正的正确性

- 确保了 embedded.mobileprovision 里的数据都是苹果授权以后，就可以取出里面的数据，做各种验证，包括用Mac公钥验证APP签名，验证设备`UDID`是否在`UDID`列表上，`AppID`是否对应得上，权限开关是否跟 APP 里的`Entitlements`对应等。


### 0x04 流程解释

![输入图片说明](https://images.gitee.com/uploads/images/2019/0117/175450_86012552_1355277.png "Snip20190117_8.png")

- 官方安全机制

![输入图片说明](https://images.gitee.com/uploads/images/2019/0117/174709_76bf6cf1_1355277.png "Snip20190117_7.png")

<br>
写于2019-01-17
<br>