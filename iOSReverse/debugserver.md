**动态调试**是指将程序运行起来，通过下断点、打印等方式，查看参数、返回值、函数调用流程等。

## 一、Xcode动态调试原理

`Xcode`内置调试器`lldb`将输入的指令通过`debugserver`传输给App，App执行指令后将反馈信息再通过`debugserver`传送给`lldb`。
　　
![](https://user-gold-cdn.xitu.io/2018/12/23/167da061cf611ade?w=777&h=255&f=png&s=35416)

- 关于GCC、LLVM、GDB、LLDB

    - `Xcode`编译器发展历程：`GCC -> LLVM`
    - `Xcode`调试器发展历程：`GDB -> LLDB`
    
- `debugserver`一开始存放在电脑中的 Xcode 里面(注意版本号)

    - `/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/DeviceSupport/9.1/DeveloperDiskImage.dmg/usr/bin/debugserver`
    
- 当`Xcode`识别到手机设备时，会将`debugserver`安装到手机上
 
    - `/Developer/usr/bin/debugserver`

- `Xcode`动态调试局限性

    - 一般情况下，只能调试通过`Xcode`安装的App
    

## 二、动态调试任意App

通过`Xcode`安装到手机上的`debugserver`缺少一定权限，只能调试通过`Xcode`安装的APP。若想调试任意APP，则需要对`debugserver`做一些改动。

手机上的`/Developer`目录是只读的，无法直接对`/Developer/usr/bin/debugserver`文件修改，需要先把其复制到电脑。

### 0x01 debugserver瘦身

- 默认情况`debugserver`是支持全部`arm`架构
    
```
$ lipo -info debugserver
    
Architectures in the fat file: debugserver are: armv7 armv7s arm64 
```
    
- 减小`debugserver`的大小，只支持当前手机架构


```
$ lipo debugserver -thin arm64 -output debugserver_64
$ lipo -info debugserver_64 
    
Non-fat file: debugserver_64 is architecture: arm64
```
    
### 0x02 给debugserver新增权限

- 获取`debugserver`初始权限

```
$ codesign -d --entitlements - debugserver
    
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>com.apple.backboardd.debugapplications</key>
    <true/>
    <key>com.apple.backboardd.launchapplications</key>
    <true/>
    <key>com.apple.diagnosticd.diagnostic</key>
    <true/>
    <key>com.apple.frontboard.debugapplications</key>
    <true/>
    <key>com.apple.frontboard.launchapplications</key>
    <true/>
    <key>com.apple.security.network.client</key>
    <true/>
    <key>com.apple.security.network.server</key>
    <true/>
    <key>com.apple.springboard.debugapplications</key>
    <true/>
    <key>run-unsigned-code</key>
    <true/>
    <key>seatbelt-profiles</key>
    <array>
	    <string>debugserver</string>
    </array>
</dict>
</plist>
```

- 将上面获取的原始权限保存到`debugserver.entitlements`中，并添加两个调试相关权限

![](https://user-gold-cdn.xitu.io/2018/12/23/167da43164974a5b?w=503&h=336&f=png&s=71338)

### 0x03 对debugserver重签名

- codesign

```
# 签名权限
$ codesign -f -s - --entitlements debugserver.entitlements debugserver_64
    
debugserver_64: replacing existing signature
    
# 或简写为
$ codesign -fs- --entitlements debugserver.entitlements debugserver_64
```
    
- ldid


```
# 获取debugserver之前那的权限(可到当前目录下查看debugserver.entitlements内容)
$ ldid -e debugserver > debugserver.entitlements
    
# 添加两个权限
    
# 重签名
$ ldid -Sdebugserver.entitlements debugserver
    
debugserver_64: replacing existing signature
```
    
任选上述两种方法的一种重签名方式，将签好权限的`debugsever_64`重命名为`debugserver`，复制到手机的`/usr/bin/debugserver`，便于找到`debugserver`指令。

- `ssh`登录到手机给`debugserver`执行权限


```
iPhone:~ root# chmod +x /usr/bin/debugserver
``` 



## 三、附加到某个App

- 借助`usbmuxd`开启端口映射

```
$ cd iOSReserve/usbmuxd-1.0.8/python-client
$ python tcprelay.py -t 22:2222 1234:1234
    
Forwarding local port 2222 to remote port 22
Forwarding local port 1234 to remote port 1234
```

- `ssh`登录到手机

```
$ ssh root@localhost -p 2222
    
iPhone:~ root# 
```

- 附加

```
# 先打开要附加的App
iPhone:~ root# debugserver *:1234 -a WeChat(进程名或进程ID)
    
# 说明附加成功
Attaching to process WeChat...
Listening to port 1234 for a connection from *...
```

- 动态调试

```
# 新建terminal窗口 进入lldb
$ lldb
    
(lldb)
    
# 链接debugserver服务(需要时间)
(lldb) process connect connect://localhost:1234
    
Process 818 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = signal SIGSTOP
frame #0: 0x000000019247416c libsystem_kernel.dylib`mach_msg_trap + 8
libsystem_kernel.dylib`mach_msg_trap:
->  0x19247416c <+8>: ret    

libsystem_kernel.dylib`mach_msg_overwrite_trap:
    0x192474170 <+0>: mov    x16, #-0x20
    0x192474174 <+4>: svc    #0x80
    0x192474178 <+8>: ret    
Target 0: (ting) stopped.
```

执行` process connect`命令后，被附加的App会自动断住，App无法响应任何事件，按`c`恢复。
    
- 用debugserver启动App

```
iPhone:~ root# debugserver -x auto *:1234 App的可执行路径
```
    
## 坑

### 0x01 debugserver

- App未打开

```
$ debugserver *:1234 -a 进程名
    
Attaching to process 进程名...
error: failed to attach to process named: "" 
Exiting.
```

- App做了反调试

```
$ debugserver *:1234 -a 进程名
    
Attaching to process 进程名...
Segmentation fault: 11
```


写于2018-12-23