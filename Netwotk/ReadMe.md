<br>

![](../Images/Network/ReadMe/ComputerNetwork.png)

<br>

#### OSI 参考模型每层的功能

- 应用层：根据互联网中需要的应用程序的功能，定义客户端和服务端程序通信的规范，应用层向表示层发出请求

- 表示层：定义数据格式、是否加密或压缩。如 FTP 允许你选择以二进制或 ASCII 格式传输。若选择二进制，那发送方和接收方都不改变文件的内容，若选择 ASCII 格式，发送方将把文本从发送方的字符集转换成标准 ASCII 后发送数据，接收方收到数据后需要把标准 ASCII 转换成接收方计算机的字符集。
- 会话层：它定义如何开始、控制、结束一个会话，包括对多个双向小时的控制和管理，以便在只完成连续消息的一部分时可以通知应用，从而使表示层看到数据是连续的。
- 传输层：常规数据传输，面向连接或无连接。
- 网络层：根据网络地址为数据包选择转发路径。网络层为传输层提供服务，只是尽力转发数据包，不保证不丢包，也不保证按顺序到达接收端。
- 数据链路层：两个主机间的数据传输，总是在一段一段的链路上传送的，这就需要专门的链路层协议。
- 物理层：在物理层所传数据的单位是比特，发送发发送 1 (或 0)，接收方应当接收到 1 (或 0) 而不是 0 (或 1)。因此物理层要考虑用多大的电压代表 "1" 或 "0"，以及接收方如何识别发送方所发送的比特。物理层还要确定连接电缆的插头应当有多少根引脚以及各引脚应如何连接。



#### 将网络互相连接起来要使用一些中间设备，根据中间设备所在的层次不同，可以有一下四种设备

- 物理层使用的中间设备叫转发器 (repeater)

- 数据链路层使用的中间设备叫网桥或桥接器 (bridge)
- 网络层使用的中间设备叫路由器 (router)
- 网络层以上使用的中间设备叫网关 (gateway)


### 网络层

- [IP 地址](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-IP%E5%9C%B0%E5%9D%80.md)
- [子网、超网](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-%E5%AD%90%E7%BD%91%E8%B6%85%E7%BD%91.md)
- [静态路由](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-%E9%9D%99%E6%80%81%E8%B7%AF%E7%94%B1.md)
- [RIP](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-RIP.md)
- [OSPF](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-OSPF.md)
- [首部](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-%E9%A6%96%E9%83%A8.md)
- [ICMP & ARP](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-ICMP-ARP.md)
- [IPv6](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-IPv6.md)

### 传输层

- [传输层 - 概述](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E4%BC%A0%E8%BE%93%E5%B1%82-%E6%A6%82%E8%BF%B0.md)
- [UDP](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E4%BC%A0%E8%BE%93%E5%B1%82-UDP.md)
- [TCP 首部](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E4%BC%A0%E8%BE%93%E5%B1%82-TCP%E9%A6%96%E9%83%A8.md)
- [TCP 可靠传输](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E4%BC%A0%E8%BE%93%E5%B1%82-TCP%E5%8F%AF%E9%9D%A0%E4%BC%A0%E8%BE%93.md)
- [TCP 流量控制](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E4%BC%A0%E8%BE%93%E5%B1%82-TCP%E6%B5%81%E9%87%8F%E6%8E%A7%E5%88%B6.md)
- [TCP 拥塞控制](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E4%BC%A0%E8%BE%93%E5%B1%82-TCP%E6%8B%A5%E5%A1%9E%E6%8E%A7%E5%88%B6.md)
- [TCP 连接与释放](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E4%BC%A0%E8%BE%93%E5%B1%82-TCP%E8%BF%9E%E6%8E%A5%E4%B8%8E%E9%87%8A%E6%94%BE.md)

### 应用层

- [DNS](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E5%BA%94%E7%94%A8%E5%B1%82-DNS.md)
- [DHCP](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E5%BA%94%E7%94%A8%E5%B1%82-DHCP.md)
- [HTTP](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E5%BA%94%E7%94%A8%E5%B1%82-HTTP.md)
	

<br>

<br>