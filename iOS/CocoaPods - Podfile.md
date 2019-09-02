
# CocoaPods - Podfile

<br>

`Podfile`是一个描述一个或多个 Xcode 项目的目标依赖项的规范。该文件应该命名为`Podfile`。


### 版本

- `pod 'AFNetworking'`  使用最新版本

- `pod 'AFNetworking', '0.9'` ：使用指定版本 `0.9`

- `pod 'AFNetworking', '> 0.9'` : 使用版本 `(0.9, )`

- `pod 'AFNetworking', '>= 0.9'` : 使用版本 `[0.9, )`

- `pod 'AFNetworking', '< 1'` : 使用版本 `( , 1)`

- `pod 'AFNetworking', '<= 1'` : 使用版本`( , 1]`

- `pod 'AFNetworking', '~> 0.1.2'` : 使用版本 `[0.1 , 0.2)`

- `pod 'AFNetworking', '~> 0.1'` : 使用版本 `[0.1 , 1)`

- `pod 'AFNetworking', '~> 0'` : 使用版本 `[0 , )`


### 分支

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


### 方法


<br>

写于 2019-09-01

<br>
