

# iOS命令行工具

　　进到手机`/usr/bin/`目录下可以看到很多命令，如常用的`ldid`、`killall`、`find`,那这些命令是到底是咋制作的呢。今天就来搞搞　

## 一、命令行本质

- 将手机中的`killall`命令复制到电脑上，用`file`命令查看


```
$ file killall

killall: Mach-O executable arm
```

从上可以看出命令行的本质是一个`Mach-O`可执行文件，类似于App的可执行文件。那我们就可以用Xcode编写用于iOS的命令行工具

## 二、