# Cocoapods - Private Pods

<br>

## 一、创建私有 Spec Repo


### 0x01 创建远程私有 Spec Repo

先在GitHub上创建一个仓库当做私有`Spec Repo`

![](https://images.gitee.com/uploads/images/2019/0918/115218_839b17dc_1355277.png "PrivatePods_images01.png")


### 0x02 添加本地的私有 Spec Repo

先查看本地`Spec Repo`,可以看到只有一个官方的`Specs`

```
$ pod repo

master
- Type: git (master)
- URL:  https://github.com/CocoaPods/Specs.git
- Path: ~/.cocoapods/repos/master

1 repo
```

将`Github`上创建的`ZZPrivateSpecs`添加到本地

```
# pod repo add REPO_NAME SOURCE_URL
$ pod repo add ZZPrivateSpecs https://github.com/zhaoName/ZZPrivateSpecs.git

Cloning spec repo `ZZPrivateSpecs` from `https://github.com/zhaoName/ZZPrivateSpecs.git`
```

再查看索引库,这时自己创建的远程`Spec Repo`已添加到本地。

```
$ pod repo                                                                  

master
- Type: git (master)
- URL:  https://github.com/CocoaPods/Specs.git
- Path: ~/.cocoapods/repos/master

ZZPrivateSpecs
- Type: git (master)
- URL:  https://github.com/zhaoName/ZZPrivateSpecs.git
- Path: ~/.cocoapods/repos/ZZPrivateSpecs

2 repos
```

![](https://images.gitee.com/uploads/images/2019/0918/115251_7710b969_1355277.png "PrivatePods_images02.png")

### 0x03 验证 repo

```
$ cd ~/.cocoapods/repos/ZZPrivateSpecs
$ pod repo lint .

Linting spec repo `ZZPrivateSpecs`

.

Analyzed 1 podspecs files.

All the specs passed validation.
```


## 二、创建自己的 Pods


在 GitHub 上以`ZZBase`为例创建仓库

![](https://images.gitee.com/uploads/images/2019/0918/115457_b1ad1f4b_1355277.png "PrivatePods_images03.png")


### 0x01 创建模板库

`cd`到你想要创建模板库的地方，执行下面命令。使用模板库需要你自行配置一下信息。

```
$ cd Desktop
$ pod lib create ZZBase
                       
Cloning `https://github.com/CocoaPods/pod-template.git` into `ZZBase`.
Configuring ZZBase template.
! Before you can create a new library we need to setup your git credentials.

 What is your email?
 > zhao1529835@126.com

! Setting your email in git to zhao1529835@126.com
  git config user.email "zhao1529835@126.com"

------------------------------

To get you started we need to ask a few questions, this should only take a minute.

If this is your first time we recommend running through with the guide: 
 - https://guides.cocoapods.org/making/using-pod-lib-create.html
 ( hold cmd and double click links to open in a browser. )


What platform do you want to use?? [ iOS / macOS ]
 > iOS

What language do you want to use?? [ Swift / ObjC ]
 > ObjC

Would you like to include a demo application with your library? [ Yes / No ]
 > Yes

Which testing frameworks will you use? [ Specta / Kiwi / None ]
 > None

Would you like to do view based testing? [ Yes / No ]
 > No

What is your class prefix?
 > ZZ

Running pod install on your new library.

Analyzing dependencies
Fetching podspec for `ZZBase` from `../`
Downloading dependencies
Installing ZZBase (0.1.0)
Generating Pods project
Integrating client project

[!] Please close any current Xcode sessions and use `ZZBase.xcworkspace` for this project from now on.
Sending stats
Pod installation complete! There is 1 dependency from the Podfile and 1 total pod installed.

 Ace! you're ready to go!
 We will start you off by opening your project in Xcode
  open 'ZZBase/Example/ZZBase.xcworkspace'

To learn more about the template see `https://github.com/CocoaPods/pod-template.git`.
To learn more about creating a new pod, see `https://guides.cocoapods.org/making/making-a-cocoapod`.
```

到此会自动打开`Xcode`，创建的模板库目录结构如下图

![](https://images.gitee.com/uploads/images/2019/0918/115521_e6e38c7b_1355277.png "PrivatePods_images04.png")


将`UIColor+Add`放入到`Classes`文件夹，在执行`pod install`

```
$ cd Example
$ pod install

Analyzing dependencies
Fetching podspec for `ZZBase` from `../`
Downloading dependencies
Using ZZBase (0.1.0)
Generating Pods project
Integrating client project
Sending stats
Pod installation complete! There is 1 dependency from the Podfile and 1 total pod installed.
```

这样我们在`Classes`文件加入的代码就自动安装到`Example `工程的`Pods`中。

![](https://images.gitee.com/uploads/images/2019/0918/115559_745dc991_1355277.png "PrivatePods_images05.png")


### 0x02 修改`.podseec`文件

```
Pod::Spec.new do |s|
  s.name             = 'ZZBase'
  s.version          = '0.1.0'
  
  #*************下面这两个必须要修改下***************#
  s.summary          = 'ZZBase.'
  s.description      = <<-DESC
                        here is descroption.
                       DESC

  s.homepage         = 'https://github.com/zhaoName/ZZBase'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'zhaoName' => 'zhao1529835@126.com' }
  s.source           = { :git => 'https://github.com/zhaoName/ZZBase.git', :tag => s.version.to_s }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.ios.deployment_target = '8.0'

  s.source_files = 'ZZBase/Classes/**/*'
  
  # s.resource_bundles = {
  #   'ZZBase' => ['ZZBase/Assets/*.png']
  # }

  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.frameworks = 'UIKit', 'MapKit'
  # s.dependency 'AFNetworking', '~> 2.3'
end
```


### 0x03 验证本地`lib`

```
$ pod lib lint --allow-warnings          

 -> ZZBase (0.1.0)
    - WARN  | url: The URL (https://github.com/zhaoName/ZZBase) is not reachable.
    - NOTE  | xcodebuild:  note: Using new build system
    - NOTE  | [iOS] xcodebuild:  note: Planning build
    - NOTE  | [iOS] xcodebuild:  note: Constructing build description

ZZBase passed validation.
```

### 0x04 验证`spec`

```
$ pod spec lint --allow-warnings                 

 -> ZZBase (0.1.0)
    - WARN  | url: The URL (https://github.com/zhaoName/ZZBase) is not reachable.
    - NOTE  | xcodebuild:  note: Using new build system
    - NOTE  | [iOS] xcodebuild:  note: Planning build
    - NOTE  | [iOS] xcodebuild:  note: Constructing build description

Analyzed 1 podspec.

ZZBase.podspec passed validation.
```


### 0x05 将代码提交到仓库

完善代码，确认无误后再将代码提交到`ZZBase`仓库

```
$ git add .
$ git commit -m "UIcolor+Add"
$ git remote add origin https://github.com/zhaoName/ZZBase.git
$ git push origin master
```

第一次提交试用`git push origin master`会报错。

```
$ git push origin master

To https://github.com/zhaoName/ZZBase.git
 ! [rejected]        master -> master (fetch first)
error: failed to push some refs to 'https://github.com/zhaoName/ZZBase.git'
hint: Updates were rejected because the remote contains work that you do
hint: not have locally. This is usually caused by another repository pushing
hint: to the same ref. You may want to first integrate the remote changes
hint: (e.g., 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.
```

需要用如下命令(以后再提交就不用加 `-f`)。

```
$ git push -f origin master

Enumerating objects: 84, done.
Counting objects: 100% (84/84), done.
Delta compression using up to 4 threads
Compressing objects: 100% (74/74), done.
Writing objects: 100% (84/84), 28.70 KiB | 2.05 MiB/s, done.
Total 84 (delta 19), reused 0 (delta 0)
remote: Resolving deltas: 100% (19/19), done.
To https://github.com/zhaoName/ZZBase.git
 + b4fa9f2...afbdcc0 master -> master (forced update)
```

### 0x06 打标签

标签的值`0.1.0`与`.podspec`文件中`s.version`的值保持一致。

```
$ git tag '0.1.0'
$ git push --tags
```


## 三、提交`podspec`到私有`spec repo`


```
$ pod repo push ZZPrivateSpecs ZZBase.podspec                               

Validating spec
 -> ZZBase (0.1.0)
    - WARN  | url: The URL (https://github.com/zhaoName/ZZBase) is not reachable.
    - NOTE  | xcodebuild:  note: Using new build system
    - NOTE  | [iOS] xcodebuild:  note: Planning build
    - NOTE  | [iOS] xcodebuild:  note: Constructing build description

Updating the `ZZPrivateSpecs' repo


Adding the spec to the `ZZPrivateSpecs' repo

 - [Add] ZZBase (0.1.0)

Pushing the `ZZPrivateSpecs' repo
```

我们再到 GitHub 上查看`ZZPrivateSpecs`会发现多一个文件夹`ZZBase/0.1.0`。

![](https://images.gitee.com/uploads/images/2019/0918/115640_f4eae838_1355277.png "PrivatePods_images06.png")

同样在本地的私有`spec repo`中也能看到`ZZBase`索引库。

![](https://images.gitee.com/uploads/images/2019/0918/115653_a216864e_1355277.png "PrivatePods_images07.png")

我们还可以`pod search`来测试下。

```
$ pod search ZZBase

-> ZZBase (0.1.0)
   ZZBase.
   pod 'ZZBase', '~> 0.1.0'
   - Homepage: https://github.com/zhaoName/ZZBase
   - Source:   https://github.com/zhaoName/ZZBase.git
   - Versions: 0.1.0 [ZZPrivateSpecs repo]
```


## 四、使用私有库


创建一个`TestZZBase`工程，`cd`到当前目录

```
$ cd ~/Desktop/TestZZBase
$ pod init 
$ open ./Podfile
```

修改`Podfile `文件

```
source 'https://github.com/CocoaPods/Specs.git'
source 'https://github.com/zhaoName/ZZPrivateSpecs.git'

platform :ios, '9.0'

target 'TestZZBase' do
  use_frameworks!

  pod 'ZZBase'

end
```

在执行`pod install`

```
$ pod install

Analyzing dependencies
Downloading dependencies
Installing ZZBase (0.1.0)
Generating Pods project
Integrating client project

[!] Please close any current Xcode sessions and use `TestZZBase.xcworkspace` for this project from now on.
Sending stats
Pod installation complete! There is 1 dependency from the Podfile and 1 total pod installed.
```

这样我们就成功的将`ZZBase`安装到`TestZZBase`工程中。

![](https://images.gitee.com/uploads/images/2019/0918/115815_6ccef808_1355277.png "PrivatePods_images08.png")

## 五、坑


### 0x01 `summary: The summary is not meaningful.`

```
$ pod lib lint --private                      

 -> ZZBase (0.1.0)
    - WARN  | summary: The summary is not meaningful.
    - WARN  | url: The URL (https://github.com/zhaoName/ZZBase) is not reachable.
    - NOTE  | xcodebuild:  note: Using new build system
    - NOTE  | [iOS] xcodebuild:  note: Planning build
    - NOTE  | [iOS] xcodebuild:  note: Constructing build description

[!] ZZBase did not pass validation, due to 2 warnings (but you can use `--allow-warnings` to ignore them).
You can use the `--no-clean` option to inspect any issue.
```

这是由于没有修改`.podsepc`文件中的`s.summary`导致的。去修改它的内容，但内容不要太长。否则还会包警告。


### 0x02 `XXX did not pass validation, due to 1 warning or ERROR`


```
$ pod lib lint           

 -> ZZBase (0.1.0)
    - WARN  | url: The URL (https://github.com/zhaoName/ZZBase) is not reachable.
    - NOTE  | xcodebuild:  note: Using new build system
    - NOTE  | [iOS] xcodebuild:  note: Planning build
    - NOTE  | [iOS] xcodebuild:  note: Constructing build description

[!] ZZBase did not pass validation, due to 1 warning (but you can use `--allow-warnings` to ignore it).
You can use the `--no-clean` option to inspect any issue.
```

无论是验证`lib`还是`spec`都要求没有任何错误和警告。错误需要你一点点调试，警告可以用`--allow-warnings`忽略。更多命令可以到[这里查看](https://guides.cocoapods.org/terminal/commands.html#pod_spec_lint)。



### 0x03 `Remote branch 0.1.0 not found in upstream origin`


在提交`podspec`到私有`spec repo`，可能会遇到如下错误

```
$ pod repo push ZZPrivateSpecs ZZBase.podspec                               

Validating spec
 -> ZZBase (0.1.0)
 - ERROR | [OSX] unknown: Encountered an unknown error ([!] /usr/bin/git clone https://git.coding.net/zhongdaima/TestTool.git /var/folders/gt/l6g6khk5053fhhw0d5f7qjw80000gn/T/d20180521-4470-1c18v0s --template= --single-branch --depth 1 --branch 0.1.0

Cloning into '/var/folders/gt/l6g6khk5053fhhw0d5f7qjw80000gn/T/d20180521-4470-1c18v0s'...
warning: Could not find remote branch 0.1.0 to clone.
fatal: Remote branch 0.1.0 not found in upstream origin
) during validation.

[!] The `ZZBase.podspec` specification does not validate.
```

这是因为没有找到`0.1.0`版本，我们需要给`ZZBase`打`tag`。标签的值`0.1.0`与`.podspec`文件中`s.version`的值保持一致。

```
$ git tag '0.1.0'

# git push --tags
$ git push origin 0.1.0
```



<br>

**参考：**

- [Private Pods](https://guides.cocoapods.org/making/private-cocoapods.html)

- [Using Pod Lib Create](https://guides.cocoapods.org/making/using-pod-lib-create.html)

- [iOS 组件化开发（一）：远程私有库的基本使用](https://juejin.im/post/5ac5d5abf265da2396129e63#heading-6)

- [CocoaPods模块化应用-私有库](https://juejin.im/post/5b037b43f265da0b7c079852)

- [CocoaPods系列 - 制作](https://www.jianshu.com/p/bc278f6fae45)


<br>

写于 2019-09-18

<br>
