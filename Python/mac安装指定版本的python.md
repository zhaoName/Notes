# mac 安装指定版本的 python

<br>

转载自：[mac安装指定版本的python](https://www.cnblogs.com/hziwei/p/13364589.html)


<br>


#### 0x01 安装 homebrew

```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

<br>

#### 0x02 使用 homebrew 安装 pyenv

```
$ brew install pyenv
```

添加到 `.bash_profile` 中

```
$ echo -e 'if which pyenv > /dev/null; then eval "$(pyenv init -)"; fi' >> ~/.bash_profile
```

生效

```
$ exec "$SHELL"
```

<br>

#### 0x03 使用 pyenv 安装指定版本的 python

- 查看已安装的版本

```
$ pyenv versions
* system (set by ~/.pyenv/version)
  3.8.2
```

system 则是系统本身的 Python 环境，而下方的 3.8.2 则是使用 Pyenv 安装的，带 * 的则是当前正在使用的 Python 环境。

- 查看可安装的版本

```
$ pyenv install -l
Available versions:
  ...
  2.4.2
  2.4.3
  ...
```

- 安装 python

```
$ pyenv install 3.8.2
python-build: use openssl@1.1 from homebrew
python-build: use readline from homebrew
Downloading Python-3.8.2.tar.xz...
-> https://www.python.org/ftp/python/3.8.2/Python-3.8.2.tar.xz
Installing Python-3.8.2...
python-build: use readline from homebrew
python-build: use zlib from xcode sdk
Installed Python-3.8.2 to ~/.pyenv/versions/3.8.2
```

- 指定全局的 python 环境

```
$ pyenv global 3.8.2
```

也可针对某个目录指定 python 环境

```
# 指定之前，首先要使用 cd 命令进入要指定的目录，然后输入以下命令。
$ pyenv local 3.8.2

# 如果要取消某个目录的Python环境，可以使用以下命令：
$ pyenv local --unset
```

再查看当前版本

```
$ pyenv versions
  system
* 3.8.2 (set by ~/.python-version)


$ python
Python 3.8.2 (default, Jul 27 2021, 23:15:58)
[Clang 11.0.0 (clang-1100.0.33.8)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

<br>

#### 0x04 卸载 python

- 卸载

```
$ pyenv uninstall 3.8.2
```

- 常用指令

```
pyenv commands  查看pyenv命令
pyenv install --list  查看可安装的python版本
pyenv versions  查看已安装版本
pyenv version   查看当前使用的python版本
pyenv install -v 版本号  安装python
pyenv rehash  刷新python已安装列表
pyenv uninstall 版本号  删除python
pyenv global 版本号  设置当前全局python版本
pyenv local  版本号  设置局部python版本
pyenv local --unset 取消局部python版本设置,或者删除当前目录下的.python-version文件
```

<br>

#### 0x05 安装 pip

到 [官网教程](https://pip.pypa.io/en/stable/installation/) 选择 MacOS 平台，下载 `get-pip.py` 文件，然后 `cd` 到包含`get-pip.py` 文件的目录下，执行下面的命令

```
$ python get-pip.py
Collecting pip
  Downloading pip-21.2.1-py3-none-any.whl (1.6 MB)
     |████████████████████████████████| 1.6 MB 438 kB/s
Collecting wheel
  Downloading wheel-0.36.2-py2.py3-none-any.whl (35 kB)
Installing collected packages: wheel, pip
  Attempting uninstall: pip
    Found existing installation: pip 19.2.3
    Uninstalling pip-19.2.3:
      Successfully uninstalled pip-19.2.3
Successfully installed pip-21.2.1 wheel-0.36.2 
``` 

查看 pip 版本

```
$  pip --version
pip 21.2.1 from ~/.pyenv/versions/3.8.2/lib/python3.8/site-packages/pip (python 3.8)
```

<br>

#### 0x06 安装 requests

```
$ pip install requests

Collecting requests
  Downloading requests-2.26.0-py2.py3-none-any.whl (62 kB)
     |████████████████████████████████| 62 kB 138 kB/s
Collecting charset-normalizer~=2.0.0
  Downloading charset_normalizer-2.0.3-py3-none-any.whl (35 kB)
Collecting urllib3<1.27,>=1.21.1
  Downloading urllib3-1.26.6-py2.py3-none-any.whl (138 kB)
     |████████████████████████████████| 138 kB 675 kB/s
Collecting idna<4,>=2.5
  Downloading idna-3.2-py3-none-any.whl (59 kB)
     |████████████████████████████████| 59 kB 1.9 MB/s
Collecting certifi>=2017.4.17
  Downloading certifi-2021.5.30-py2.py3-none-any.whl (145 kB)
     |████████████████████████████████| 145 kB 413 kB/s
Installing collected packages: urllib3, idna, charset-normalizer, certifi, requests
Successfully installed certifi-2021.5.30 charset-normalizer-2.0.3 idna-3.2 requests-2.26.0 urllib3-1.26.6
```

<br>

#### 0x07 坑

若使用 `pyenv global 3.8.2` 指定全局 python 环境没有效果，则需要到 `.bash_profile` 文件中查看

```
export PATH="$HOME/.rbenv/bin:$PATH"
eval "$(pyenv init --path)"
if which pyenv > /dev/null; then eval "$(pyenv init -)"; fi
```

要包含上述内容才行，若出错则需要把少了的代码手动加上去，再执行`source ~/.bash_profile`。


若你使用的事 `.zshrc` 文件，则需要将上述代码拷贝一份到 `.zshrc`中, 再执行`source ~/.zshrc `。



<br>

<br>