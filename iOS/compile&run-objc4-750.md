# 编译 & 运行 objc4-750


可以在这找到 [objc4](http://opensource.apple.com/tarballs/objc4/) 的源码下载。

<br>

## 一、编译

### 0x01 准备工作

进入[苹果开源网站](https://opensource.apple.com/tarballs/)，下载依赖的开源项目：

- xnu-4903.270.47

- dyld-551.4
- libdispatch-1008.270.1
- libpthread-416.60.2
- Libc-825.40.1


### 0x01 架构问题

提示 `The i386 architecture is deprecated. You should update your ARCHS build setting to remove the i386 architecture. ` 错误。需要到 `Targets -> Build Settings  -> Architecture` 设置为 `Standard arcgutectures`

![](../Images/iOS/CompileObjc4/CompileObjc4_image01.png)

### 0x02 设置缺失文件检索路径

在当前项目下创建一个文件夹 MissingFile，里面用于存放所有缺失的头问题件。
然后在 `Targets -> Build Settings -> Header Serach Paths` 中添加搜索路径 `$(SRCROOT)/MissingFile`


![](../Images/iOS/CompileObjc4/CompileObjc4_image02.png)


### 0x03 头文件找不到

- 编译提示：`'sys/reason.h' file not found `

打开文件夹，搜索 `sys/reason.h`，然后将 `./xnu-4903.270.47/bsd/sys/reason.h`  路径下的 `reason.h` 文件复制到 objc4-750 工程下的 `/MissingFile/sys/` 文件夹中

![](../Images/iOS/CompileObjc4/CompileObjc4_image02.png)

接下来重复上述步骤

- 编译提示：`'mach-o/dyld_priv.h' file not found`

选择 `./dyld-551.3/include/mach-o/dyld_priv.h` 

- 编译提示：`'os/lock_private.h' file not found`

选择 `./libdispatch-1008.270.1/private/os/lock_private.h` 

- 编译提示：`'os/base_private.h' file not found`

选择 `./libdispatch-1008.270.1/private/os/base_private.h` 

- 编译提示：`'pthread/tsd_private.h' file not found`

选择 `./libpthread-416.60.2/private/tsd_private.h` 

- 编译提示：`'System/machine/cpu_capabilities.h' file not found`

选择 `./xnu-4903.270.47/osfmk/machine/cpu_capabilities.h` 

- 编译提示：`'os/tsd.h' file not found`

选择 `./libdispatch-1008.270.1/src/shims/tsd.h` 

- 编译提示：`'pthread/spinlock_private.h' file not found`

选择 `./libdispatch-1008.270.1/private/spinlock_private.h` 

- 编译提示：`'System/pthread_machdep.h' file not found`

选择 `./Libc-825.40.1/pthreads/pthread_machdep.h` 

- 编译提示：`'CrashReporterClient.h' file not found`

选择 `./Libc-825.40.1/include/CrashReporterClient.h` 

- 编译提示：`'Block_private.h' file not found`

选择 `./libdispatch-1008.270.1/src/BlocksRuntime/Block_private.h`

- 编译提示：`Typedef redefinition with different types ('int' vs 'volatile OSSpinLock' (aka 'volatile int'))`

重复定义 `pthread_lock_t`，在 pthread_machdep.h 中删除 `pthread_lock_t` 的定义就可以。

- 编译提示：`Static declaration of '_pthread_has_direct_tsd' follows non-static declaration`

这里有三个函数重复定义了

- `_pthread_has_direct_tsd`

- `_pthread_getspecific_direct`
- `_pthread_setspecific_direct`

在 pthread_machdep.h 中删除对应的函数就好。


- 编译提示：`'objc-shared-cache.h' file not found`

选择 `./dyld-551.4/include/objc-shared-cache.h`





```Objective-C

```

```Objective-C

```

<br>

## 二、运行

<br>


<br>



<br>