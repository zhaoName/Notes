
# CocoaPods - Podfile

<br>


## 一、Podfile

`Podfile`是一个描述一个或多个 Xcode 项目的目标依赖项的规范。该文件应该命名为`Podfile`。

```
target 'MyApp' do

  pod 'AFNetworking', '~> 3.0'
  
1. end
```


### 0x01 版本

- `pod 'AFNetworking'`  使用最新版本

- `pod 'AFNetworking', '0.9'` ：使用指定版本 `0.9`

- `pod 'AFNetworking', '> 0.9'` : 使用版本 `(0.9, )`

- `pod 'AFNetworking', '>= 0.9'` : 使用版本 `[0.9, )`

- `pod 'AFNetworking', '< 1'` : 使用版本 `( , 1)`

- `pod 'AFNetworking', '<= 1'` : 使用版本`( , 1]`

- `pod 'AFNetworking', '~> 0.1.2'` : 使用版本 `[0.1 , 0.2)`

- `pod 'AFNetworking', '~> 0.1'` : 使用版本 `[0.1 , 1)`

- `pod 'AFNetworking', '~> 0'` : 使用版本 `[0 , )`


### 0x02 分支

- 使用`master`分支

```
pod 'Alamofire', :git => 'https://github.com/Alamofire/Alamofire.git'
```

- 使用指定分支 如`dev`

```
pod 'Alamofire', :git => 'https://github.com/Alamofire/Alamofire.git', :branch => 'dev'
```

- 使用某个`tag`

```
pod 'Alamofire', :git => 'https://github.com/Alamofire/Alamofire.git', :tag => '3.1.1'
```

- 使用特定`commit`

```
pod 'Alamofire', :git => 'https://github.com/Alamofire/Alamofire.git', :commit => '0f506b1c45'
```


### 0x03 `Build configurations`

默认情况下，依赖库会安装所有的`Build configurations`，但处于某些特定的目的可以指定依赖库的`Build configurations`。

```
# 只在 Debug 模式下使用 AFNetworking
pod 'AFNetworking', :configuration => 'Debug'

# 指定多个 configurations
pod 'PonyDebugger', :configurations => ['Debug', 'Beta']
```


### 0x04 `Source`

指定`spec`仓库地址,默认是官方的地址`https://github.com/CocoaPods/Specs.git`，可以不写。但当你使用私有库时需要加上私有仓库的地址，同时你也需要把官方的地址加上。

```
# 默认地址
source 'https://github.com/CocoaPods/Specs.git'
# 私有地址
source 'https://github.com/Artsy/Specs.git'

target 'MyApp' do

  pod 'AFNetworking', '~> 3.0''
  
end
```

也可以单独指定某个依赖库的`source`

```
pod 'AFNetworking', :source => 'https://github.com/CocoaPods/Specs.git'
```


### 0x05 `Subspecs `


有时一个依赖库可以分成几个模块单独使用，安转的时候默认是全部安装。但我们也可以指定安装某个特定的模块。比如分享时只想分享到 QQ 微信。 

```
pod 'mob_sharesdk'
pod 'mob_sharesdk/ShareSDKPlatforms/QQ'
pod 'mob_sharesdk/ShareSDKPlatforms/WeChat'
```

或者指定要安装的子模块的集合

```
pod 'mob_sharesdk', :subspecs => ['ShareSDKPlatforms/QQ', 'ShareSDKPlatforms/WeChat']
```


### 0x06 本地仓库

`cocoapods` 会在指定的文件夹下找 `podspec`，所以当前文件夹下一定要有`podspec`文件

```
pod 'AFNetworking', :path => '~/Documents/AFNetworking'
```


### 0x07 `inhibit_all_warnings!`


`inhibit_all_warnings!` 忽略`CocoaPods`中的所有警告。你也可以单独忽略某个依赖库的警告

```
pod 'AFNetworking', :inhibit_warnings => true
```

或者不忽略某个依赖库的所有警告

```
pod 'AFNetworking', :inhibit_warnings => false
```

### 0x08 `use_frameworks!`

在使用动态库或将 Swift 框架导入到 Swift 项目或 OC 项目中都必须使用`use_frameworks!`。`Cocoapods` 会生成相应的 `.frameworks`文件，使用 `dynamic frameworks` 来取代 `static libraries `方式。


## 二、Podfile.lock


`Podfile.lock`中记录着所有已安装的`pod`的描述。其结构如下(以`AFNetworking `为例):

```
PODS:
  - AFNetworking (3.2.1):
    - AFNetworking/NSURLSession (= 3.2.1)
    - AFNetworking/Reachability (= 3.2.1)
    - AFNetworking/Security (= 3.2.1)
    - AFNetworking/Serialization (= 3.2.1)
    - AFNetworking/UIKit (= 3.2.1)
  - AFNetworking/NSURLSession (3.2.1):
    - AFNetworking/Reachability
    - AFNetworking/Security
    - AFNetworking/Serialization
  - AFNetworking/Reachability (3.2.1)
  - AFNetworking/Security (3.2.1)
  - AFNetworking/Serialization (3.2.1)
  - AFNetworking/UIKit (3.2.1):
    - AFNetworking/NSURLSession

DEPENDENCIES:
  - AFNetworking

SPEC REPOS:
  https://github.com/cocoapods/specs.git:
    - AFNetworking

SPEC CHECKSUMS:
  AFNetworking: b6f891fdfaed196b46c7a83cf209e09697b94057
  
PODFILE CHECKSUM: 49d7221d986ce89bac56ac859e057be2f4eac621

COCOAPODS: 1.6.1
```

`Podfile.lock`在第一次执行`pod install`命令是创建，会将每个`pod`的描述信息写入`Podfile.lock`中。其主要作用是**指定每个`pod`的版本号**。


## 三、pod install vs pod update


### 0x01 pod install

`pod install`不仅在第一次使用`CocoaPods`时使用，也在添加、移除`pod`时使用。

- 每次执行`pod install`命令下载安装新的`pod`时，都会在`Podfile.lock`文件中为每个`pod`写入已安装的版本，并锁定`pod`的版本号。

- 我们执行`pod install`，`CocoaPods`只会解析`Podfile.lock`中尚未列出的`pod`。

	- 对于`Podfile.lock`中列出的`pod`，它会下载`Podfile.lock`中特定指出版本，而不尝试检查是否有更新的版本。

	- 对于尚未在`Podfile.lock`中列出的`pod`，它会搜索与`Podfile`文件中相匹配的版本并下载。


### 0x02 pod update


当你执行`pod update PodName`时，`CocoaPods`将会搜索`PodName`更新的版本而不考虑`Podfile.lock`中指定的版本。然后将`PodName`更新到最新版本(前提是要与`Podfile`文件中指定的版本匹配)。

当你执行`pod update`时，`CocoaPods`将会将`Podfile`中列出的每个`pod`都更新为最新版本(前提是要与`Podfile`文件中指定的版本匹配)。


你可以执行`pod outdated`查看比当前`Podfile.lock`中列出的`pod`更新的版本。


**总结:**

- 若你想安装或移除某个`pod`时，使用`pod install`。

- 若你想更新某个已存在的`pod`时，使用`pod update PodName`。


## 四、提交`Podfile.lock`


一般情况我们不会将`Pods`中的内容提交到远程仓库。但是必须要将`Podfile.lock`提交到远程仓库，否则多人合作开发时会造成`pod`版本冲突。


可能有人会说在`Podfile`指定的`pod`的版本，如`pod 'PodName', '1.0.0'`，就可以保证团队合作时每个用户使用的都是同一个版本。


但实际上这样并不能保证所有用户获得所有`pod`有完全相同的版本。下面我们来用个例子说明：

有个`podA`依赖于`podA2`并在`A.podspec`中声明``

```
s.dependency 'podA2', '~> 3.0'
```

这样用户`user1`和`user2`获取的`podA`都是`1.0.0`版本，但是`podA2`可能会出现版本冲突

- `user1`可能使用`3.4`版本的`podA2`

- `user2`可能是后来加入的，从远程服务器上拉取代码后执行`pod install`，获取的`podA2`可能是`3.5`版本。


<br>

**参考：**

- [The Podfile](https://guides.cocoapods.org/using/the-podfile.html)

- [Podfile Syntax Reference](https://guides.cocoapods.org/syntax/podfile.html)

- [pod install vs. pod update](https://guides.cocoapods.org/using/pod-install-vs-update.html)

<br>

写于 2019-09-01

<br>
