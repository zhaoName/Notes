
# CocoaPods - 卸载与安装

<br>

CocoaPods 管理 Xcode 项目的库依赖项，可以帮助您优雅地扩展项目。项目的依赖项在 Podfile 文件中指定。 CocoaPods 将解析库之间的依赖关系，获取生成的源代码，然后在 Xcode 工作区中将它们链接在一起以构建项目。

## 一、Uninstall

- 列出所有`cocoapods`相关内容

```
$ gem list --local | grep cocoapods

cocoapods (1.6.1)
cocoapods-core (1.6.1, 1.5.0)
cocoapods-deintegrate (1.0.2)
cocoapods-downloader (1.2.2, 1.2.0)
cocoapods-packager (1.5.0)
cocoapods-plugins (1.0.0)
cocoapods-search (1.0.0)
cocoapods-stats (1.0.0)
cocoapods-trunk (1.3.1, 1.3.0)
cocoapods-try (1.1.0)
```

-  卸载所有`cocoapods-*`

```
$ gem uninstall cocoapods-core

Select gem to uninstall:
 1. cocoapods-core-1.5.0
 2. cocoapods-core-1.6.1
 3. All versions
> 3
Successfully uninstalled cocoapods-core-1.5.0

You have requested to uninstall the gem:
	cocoapods-core-1.6.1

cocoapods-1.6.1 depends on cocoapods-core (= 1.6.1)
If you remove this gem, these dependencies will not be met.
Continue with Uninstall? [yN]  y
Successfully uninstalled cocoapods-core-1.6.1


$ sudo gem uninstall cocoapods-deintegrate

You have requested to uninstall the gem:
	cocoapods-deintegrate-1.0.2

cocoapods-1.6.1 depends on cocoapods-deintegrate (< 2.0, >= 1.0.2)
If you remove this gem, these dependencies will not be met.
Continue with Uninstall? [yN]  y
Successfully uninstalled cocoapods-deintegrate-1.0.2

...
```

- 卸载 `cocoapods`

```
## 若有多个 cocoapods 会让你选择想要卸载版本
## 卸载指定版本 sudo gem uninstall cocoapods -v 0.38.0
$ sudo gem uninstall cocoapods 

## 可能会报错 权限问题
Remove executables:
	pod, sandbox-pod

in addition to the gem? [Yn]  y
ERROR:  While executing gem ... (Gem::FilePermissionError)
    You don't have write permissions for the /usr/bin directory.
    

## 正确的姿势
$ sudo gem uninstall cocoapods -n /usr/local/bin

Remove executables:
	pod, sandbox-pod

in addition to the gem? [Yn]  y
Removing pod
Removing sandbox-pod
Successfully uninstalled cocoapods-1.6.1
```

- 确认所有`cocoapods`都已卸载

```
## 没有输出说明卸载完成
$ gem list --local | grep cocoapods
```

- 移除`.cocoapods`

```
## 需要点时间
$ rm -rf ~/.cocoapods/
```

至此`CocoaPods`卸载完成。


## 二、 Install


- 安装

```
$ sudo gem install cocoapods         
Password:

Fetching: cocoapods-core-1.7.5.gem (100%)
Successfully installed cocoapods-core-1.7.5
Fetching: cocoapods-deintegrate-1.0.4.gem (100%)
Successfully installed cocoapods-deintegrate-1.0.4
Fetching: cocoapods-downloader-1.2.2.gem (100%)
Successfully installed cocoapods-downloader-1.2.2
Fetching: cocoapods-plugins-1.0.0.gem (100%)
Successfully installed cocoapods-plugins-1.0.0
Fetching: cocoapods-search-1.0.0.gem (100%)
Successfully installed cocoapods-search-1.0.0
Fetching: cocoapods-stats-1.1.0.gem (100%)
Successfully installed cocoapods-stats-1.1.0
Fetching: cocoapods-trunk-1.4.0.gem (100%)
Successfully installed cocoapods-trunk-1.4.0
Fetching: cocoapods-try-1.1.0.gem (100%)
Successfully installed cocoapods-try-1.1.0
Fetching: xcodeproj-1.12.0.gem (100%)
ERROR:  While executing gem ... (Gem::FilePermissionError)
    You don't have write permissions for the /usr/bin directory.
```

都会报错，那是因为苹果限制了`/usr/bin`文件的权限，`root`权限都没用。可以用如下方式解决

```
$ sudo gem install cocoapods -n /usr/local/bin/

Successfully installed xcodeproj-1.12.0
Fetching: fourflusher-2.3.1.gem (100%)
Successfully installed fourflusher-2.3.1
Fetching: cocoapods-1.7.5.gem (100%)
Successfully installed cocoapods-1.7.5
Parsing documentation for xcodeproj-1.12.0
Installing ri documentation for xcodeproj-1.12.0
Parsing documentation for fourflusher-2.3.1
Installing ri documentation for fourflusher-2.3.1
Parsing documentation for cocoapods-1.7.5
Installing ri documentation for cocoapods-1.7.5
Done installing documentation for xcodeproj, fourflusher, cocoapods after 6 seconds
3 gems installed
```

你会看到卸载的`cocoapods-*`又都重装改回来了！

```
$ gem list --local | grep cocoapods

cocoapods (1.7.5)
cocoapods-core (1.7.5)
cocoapods-deintegrate (1.0.4)
cocoapods-downloader (1.2.2)
cocoapods-plugins (1.0.0)
cocoapods-search (1.0.0)
cocoapods-stats (1.1.0)
cocoapods-trunk (1.4.0)
cocoapods-try (1.1.0)
```

- `pod setup`


`pod setup`作用是将`https://github.com/CocoaPods/SpecsCocoapods`克隆`~/.cocoapods/repos`目录下。但此过程很慢，可以采取如下方式

```
$ git clone https://git.coding.net/CocoaPods/Specs.git  ~/.cocoapods/repos/master 

Cloning into '/Users/chuangqi/.cocoapods/repos/master'...
remote: Enumerating objects: 1130099, done.
remote: Total 1130099 (delta 0), reused 0 (delta 0)iB | 3.54 MiB/s   
Receiving objects: 100% (1130099/1130099), 408.72 MiB | 4.50 MiB/s, done.
Resolving deltas: 100% (563214/563214), done.
Checking out files: 100% (141834/141834), done.
```

可以看到克隆速度可以达到几M/s。这时可以用`pod search *** `搜索三方库

```
$ pod search AFNetworking

-> AFNetworking (3.1.0)
   A delightful iOS and OS X networking framework.
   pod 'AFNetworking', '~> 3.1.0'
   - Homepage: https://github.com/AFNetworking/AFNetworking
   - Source:   https://github.com/AFNetworking/AFNetworking.git
   - Versions: 3.1.0, 3.0.4, 3.0.3, 3.0.2, 3.0.1, 3.0.0, 3.0.0-beta.3, 3.0.0-beta.2, 3.0.0-beta.1, 2.6.3, 2.6.2, 2.6.1, 2.6.0, 2.5.4, 2.5.3,
   2.5.2, 2.5.1, 2.5.0, 2.4.1, 2.4.0, 2.3.1, 2.3.0, 2.2.4, 2.2.3, 2.2.2, 2.2.1, 2.2.0, 2.1.0, 2.0.3, 2.0.2, 2.0.1, 2.0.0, 2.0.0-RC3,
   2.0.0-RC2, 2.0.0-RC1, 1.3.4, 1.3.3, 1.3.2, 1.3.1, 1.3.0, 1.2.1, 1.2.0, 1.1.0, 1.0.1, 1.0, 1.0RC3, 1.0RC2, 1.0RC1, 0.10.1, 0.10.0, 0.9.2,
   0.9.1, 0.9.0, 0.7.0, 0.5.1 [master repo]
   - Subspecs:
     - AFNetworking/Serialization (3.1.0)
     - AFNetworking/Security (3.1.0)
     - AFNetworking/Reachability (3.1.0)
     - AFNetworking/NSURLSession (3.1.0)
     - AFNetworking/UIKit (3.1.0)
```

至此`CocoaPds`安装完成。


## 三、使用

`cd`到工程目录下，执行`pod init`，可在当前文件夹下找到`Podfile`文件

```
# Uncomment the next line to define a global platform for your project
# platform :ios, '9.0'

target 'Your_Project_Name' do
  # Comment the next line if you don't want to use dynamic frameworks
  use_frameworks!

  # Pods for Your_Project_Name

end
```

然后在`Podfile `中添加`pod 'ThildLib_Name'`

```
platform :ios, '9.0'

target 'Your_Project_Name' do

  use_frameworks!
  pod 'AFNetworking'

end
```

在执行`pod install`,就会在当前文件夹看到生成的`Your_Project_Name.xcworkspace`，打开它就能正常使用`cocoapods`管理三方库了。



## 四 坑

### 0x01 换源

若你执行`$ sudo gem install cocoapods`很久之后都没反应，那是因为`cocoapods.org`被墙了。我们需要换源。

- 查看当前源

```
$ gem source -l 
                                                                 
*** CURRENT SOURCES ***

https://rubygems.org/
```

- 移除现有源

```
$ gem source --remove https://rubygems.org/
      
https://rubygems.org/ removed from sources
```

- 添加新源

```
$ gem source --a https://gems.ruby-china.com

https://gems.ruby-china.com added to sources
```

以前添加的是`https://ruby.taobao.org/`，但后来淘宝由于域名的问题停止更新了。

<br>

**参考：**

- [How to fully uninstall the Cocoapods from the Mac Machine?](https://superuser.com/questions/686317/how-to-fully-uninstall-the-cocoapods-from-the-mac-machine)

- [Getting Started](https://guides.cocoapods.org/using/getting-started.html)

- [Using CocoaPods](https://guides.cocoapods.org/using/using-cocoapods.html)


<br>

写于 2019-08-30

<br>