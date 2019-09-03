
# CocoaPods - Podfile

<br>

`Podfile`是一个描述一个或多个 Xcode 项目的目标依赖项的规范。该文件应该命名为`Podfile`。

```
target 'MyApp' do

  pod 'AFNetworking', '~> 3.0''
  
end
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

<br>

写于 2019-09-01

<br>
