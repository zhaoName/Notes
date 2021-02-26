
# 网络层 - ICMP & ARP

<br>

## 一、 ICMP

ICMP（Internet Control Message Protocol）Internet控制报文协议。它是 TCP/IP 协议簇的一个子协议，用于在 IP 主机、路由器之间传递控制消息。控制消息是指网络通不通、主机是否可达、路由是否可用等网络本身的消息。这些控制消息虽然并不传输用户数据，但是对于用户数据的传递起着重要的作用。

### 0x01 ICMP 报文格式

ICMP 报文包含在 IP 数据报中，IP 首部在 ICMP 报文的最前面。一个 ICMP 报文包括 IP 首部（至少 20 字节）、ICMP报头（至少 8 字节）和 ICMP 报文的数据部分。当 IP 首部中的协议字段值为 1 时，就说明这是一个 ICMP 报文。ICMP 报文格式如下

![](../Images/Network/ICMP&ARP/ICMP&ARP_images00.png)

![](../Images/Network/ICMP&ARP/ICMP&ARP_images01.png)

常用 ICMP 报文类型和代码

![](../Images/Network/ICMP&ARP/ICMP&ARP_images04.png)

抓包查看 ICMP 请求报文格式

![](../Images/Network/ICMP&ARP/ICMP&ARP_images02.png)

### 0x02 ICMP 差错报告报文格式

![](../Images/Network/ICMP&ARP/ICMP&ARP_images03.png)

- TTL 耗尽

```
$ ping baidu.com -m 2
PING baidu.com (39.156.69.79): 56 data bytes
92 bytes from 192.168.1.1 (192.168.1.1): Time to live exceeded
Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst
 4  5  00 5400 da0e   0 0000  01  01 b19c 192.168.0.107  39.156.69.79
```

![](../Images/Network/ICMP&ARP/ICMP&ARP_images05.png)


- 应用程序返回 ICMP 差错报告

不仅是 `ping` 命令可以使用 ICMP 协议。当我们在浏览器中输入一个不可访问的 IP 地址，也能得到一个 ICMP 差错报告。如访问 192.168.20.20， 192.168.0.1 网关返回 192.168.20.20 不可到达

![](../Images/Network/ICMP&ARP/ICMP&ARP_images06.png)

- 路由重定向

如果一个主机在通信时将数据报发送给了路由器 R1，而 R1 将这个数据报转发给了另一个路由器R2，且主机到路由器 R2 之间有一条直连的路径（也就是说此主机和路由器 R2 处于同一以太网段上），那么路由器R1会发送一条重定向报文给主机，来通知它到路由器 R2 可用路径里有一条更短、更优化的路径。这个主机在接收到这个重定向报文之后应该改变其路由至这个优化版本的路由信息，来将抵达这个目的地的数据报直接发送到路由器 R2，并且路由器仍将原始数据报发送到预期目的地。

但如果一个数据报携带有路由信息，那么即使有更加优化的路径，路由器也不会发送重定向报文。并且，RFC 1122 指出，重定向报文应该只由网关发送，而不应该由互联网主机发送。

![](../Images/Network/ICMP&ARP/ICMP&ARP_images07.png)

![](../Images/Network/ICMP&ARP/ICMP&ARP_images08.png)

<br>

## 二、 ARP

<br>


<br>


<br>