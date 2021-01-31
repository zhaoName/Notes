# 网络层 - OSPF


<br>

OSPF（Open Shortest Path First）开放式最短路径优先协议：是 IETF 组织开发的一个基于链路状态的内部网关协议（Interior Gateway Protocol）。当前，在 RFC 2328 中定义的 OSPF 版本2 用于 IPv4，在 RFC 2740 中定义的 OSPF 版本 3 用于 IPv6。如无特殊说明，本文中所指的 OSPF 均为 OSPF Version 2。

开放是指 OSPF 是一种基于开放标准的路由选择协议，其中 OSPFV2 参考 RFC2328，OSPFV3 参考 RFC5340. 路径最短优先是指 OSPF 在路由选择上采用 Dijkstra 所提出的最短路径算法

<br>

## 一、基本概念

### 0x01 RIP vs OSPF

当今网络的快速增长和扩展已将RIP推向了极限。RIP 具有某些局限性，可能会导致大型网络出现问题：

- RIP 的限制为 15 跳。跨越 15 个跃点（15 个路由器）的 RIP 网络被认为是不可访问的。
- RIP 无法处理可变长度子网掩码（Variable Length Subnet Masks VLSM）。鉴于 IP 地址的短缺和 VLSM 在有效分配 IP 地址方面的灵活性，这被认为是一个主要缺陷。
- 完整路由表的定期广播会消耗大量带宽。这是大型网络的主要问题，尤其是在慢速链接和WAN云上。
- RIP 的收敛速度比 OSPF 慢。在大型网络中，收敛时间只有几分钟左右。RIP路由器经历了一段时间的压制和垃圾收集，并缓慢地超时了最近未收到的信息。这在大型环境中是不合适的，并且可能导致路由不一致。
- RIP 没有网络延迟和链接成本的概念。路由决策基于跳数。即使更长的路径具有更好的聚合链路带宽和更少的延迟，也总是首选到达目的地的跳数最少的路径。
- RIP 网络是平面网络。没有区域或边界的概念。随着无类路由的引入以及聚合和汇总的智能使用，RIP 网络似乎已经落后了。

RIP 的新版本 RIP2 中引入了一些增强功能。RIP2 解决了 VLSM，身份验证和多播路由更新的问题。RIP2 并不是 RIP（现在称为RIP 1）的重大改进，因为它仍然具有跳数和慢速收敛的局限性，这在当今的大型网络中至关重要。

另一方面，OSPF 解决了先前提出的大多数问题：

- 使用OSPF，跳数没有限制。
- VLSM 的使用在 IP 地址分配中非常有用。
- OSPF 使用 IP 多播发送链接状态更新。这样可确保在不监听 OSPF 数据包的路由器上进行更少的处理。同样仅在发生路由更改的情况下发送更新，而不是定期发送，这样可以确保更好地利用带宽。
- OSPF 具有比 RIP 更好的收敛性。这是因为路由更改是实时的，而不是周期性的。
- OSPF 允许更好的负载平衡。
- OSPF 允许对网络进行逻辑定义，其中可以将路由器划分为多个区域。这限制了整个网络上链接状态更新的爆炸式增长。这也提供了一种聚合路由并减少不必要的子网信息传播的机制。
- OSPF 允许使用不同的密码验证方法进行路由验证。
- OSPF 允许传输和标记注入自治系统的外部路由。这样可以跟踪由外部协议（例如BGP）注入的外部路由。

### 0x02 术语

#### Router-ID

如果要运行 OSPF 协议，必须存在 Router ID。Router ID 是一个32比特无符号整数，用于唯一标识 AS (Autonomous System 自治系统) 中的路由器。Router ID 的设置方式有两种：

- 通过命令手动设置，实际网络部署中建议手动设置以保证 OSPF 协议的稳定性。
- 通过协议自动选取
	- 路由器优先选择 Lookback 地址中最大的 IP 地址作为其 Router ID
	- 如果未配置 Lookback 接口，则路由器将在其他接口地址中选择最大的IP地址作为其 Router ID

在路由器运行了 OSPF 并确定了 Router ID 后，如果该 Router ID 对应的接口 Down 或者接口消失（例如执行了undo interface loopback loopback-number）或者出现更大的 IP 地址，OSPF 将仍然保持原 Router ID。只有在为路由器重新配置了 Router ID 或者重新配置 OSPF 的 Router ID 并且重新启动 OSPF 进程后，路由器才能获取才重新选取的 Router ID。

#### COST

- OSPF 使用 cost 作为路由度量值(metric)。
- 每一个激活 OSPF 的接口都有一个 cost 值。cost 计算公式 = 100M / 接口带宽，其中 100M 为 OSPF 的参考带宽（reference-bandwidth）。如一个 10Mbit/s 的接口，它的 cost =  100000000 / 10000000  = 10，OSPF 认为该接口的 Metric 值为 10。
- cost 的最小值为 1。即有个 1000Mbit/s 的接口，cost 算出来是 0.1，但要被四舍五入为1。
- OSPF 路由的成本是从源路由器到目标路由器的所有传入接口的 cost 值之和

#### Link

Link（链路）就是路由器上的接口，在这里特指运行在 OSPF 进程下的接口。

#### Link-State

OSPF 是一种链路状态协议。链路状态(LSA) 是对接口及接口与相邻路由器的关系的描述。接口的信息包括接口的 IP 地址、掩码、所连接的网络的类型、连接的邻居等。**OSPF 路由器之间交换的不是路由表信息，而是链路状态通告( Link-State Advertisemen LSA)。**

#### 报文类型

- Hello 报文，用于 OSPF 邻居动态发现，邻居参数协商，邻居关系维护。
- DBD 报文（Database Description Packet），数据库描述数报文。
	- 用于传递 LSDB（Link State Database）摘要信息，实际上是 LSDB 中 所有 LSA 的头部信息。OSPF 路由器通过交换 DBD 报文，来统计自己缺少的 LSA 并向对端发送 LSR 报文进行请求。
	- 用于主从路由器的选举
- LSR 报文（Link State Request packet），用于向邻居请求自己 LSDB 中没有的 LSA
- LSU 报文（Link State Update packet），LSU 用于给邻居发送它们请求的 LSA，只有 LSU 报文中会包含完整的 LSA
- LSAck 报文（Link State Acknowledgment packet），LSAck 用于向邻居确认收到的 LSU，如果没收到 LSAck 的话，邻居会不断重传请求的 LSU。注意区分 LSA 和 LSAck


### 0x03 邻居（Neighbor）、邻接（Adjacency）

在 OSPF 网络中，为了交换路由信息，邻居设备之间首先要建立邻接关系，邻居（Neighbors）关系和邻接（Adjacencies）关系是两个不同的概念。

- 邻居关系：OSPF 设备启动后，会通过 OSPF 接口向外发送 Hello 报文，收到 Hello 报文的 OSPF 设备会检查报文中所定义的参数，如果双方一致就会形成邻居关系，两端设备互为邻居。
- 邻接关系：形成邻居关系后，如果两端设备成功交换 DBD 报文和 LSA，才建立邻接关系。

OSPF 共有 8 种状态机，分别是：Down、Attempt、Init、2-way、Exstart、Exchange、Loading、Full。

- Down：邻居会话的初始阶段，表明没有在邻居失效时间间隔内收到来自邻居路由器的Hello数据包。
- Attempt：该状态仅发生在 NBMA 网络中，表明对端在邻居失效时间间隔（dead interval）超时前仍然没有回复 Hello 报文。此时路由器依然每发送轮询 Hello 报文的时间间隔（poll interval）向对端发送 Hello 报文。
- Init：收到 Hello 报文后状态为 Init。
- 2-way：收到的 Hello 报文中包含有自己的 Router ID，则状态为 2-way；如果不需要形成邻接关系则邻居状态机就停留在此状态，否则进入 Exstart 状态。
- Exstart：开始协商主从关系，并确定 DBD 的序列号，此时状态为 Exstart。
- Exchange：主从关系协商完毕后开始交换 DBD 报文，此时状态为 Exchange。
- Loading：DBD 报文交换完成即 Exchange done，此时状态为 Loading。
- Full：LSR 重传列表为空，此时状态为 Full

### 0x04 OSPF报文认证

OSPF 支持报文验证功能，只有通过验证的 OSPF 报文才能接收，否则将不能正常建立邻居。路由器支持两种验证方式：

- 区域验证方式
- 接口验证方式

当两种验证方式都存在时，优先使用接口验证方式

<br>

## 二、OSPF 原理

### 0x01 建立临接关系

### 0x02 建立临接关系

### 0x03 建立临接关系

<br>

## 三、OSPF 实验

搭建如下图配置的实验环境，计算机和路由器关于 IP 地址的设置可以查看上一篇文章[RIP](https://github.com/zhaoName/Notes/blob/master/Netwotk/%E7%BD%91%E7%BB%9C%E5%B1%82-RIP.md)

![](../Images/Network/RIP/RIP_image09.png)


以 R1 为例让路由器启动 OSPF，并宣告网段。OSPF 协议宣告网段时要带上子网掩码信息0.0.0.255 表示 255.255.255.0。

```
// 进入配置路由器
R1# configure t 
Enter configuration commands, one per line.  End with CNTL/Z.

// 启动 ospf
R1(config)# router ospf 1

// 宣告网段 
R1(config-router)#network 192.168.0.0 0.0.0.255 area 0 
R1(config-router)#network 192.168.1.0 0.0.0.255 area 0
R1(config-router)#network 192.168.4.0 0.0.0.255 area 0
```

同样配置 R2、R3、R4、R5 后查看 OSPF 协议和路由器学习到的路由信息。

```
// 查看被配置在路由器上的所有动态路由协议
R1# show ip protocols 
*** IP Routing is NSF aware ***

Routing Protocol is "ospf 1"
  Outgoing update filter list for all interfaces is not set
  Incoming update filter list for all interfaces is not set
  Router ID 192.168.4.1
  Number of areas in this router is 1. 1 normal 0 stub 0 nssa
  Maximum path: 4
  Routing for Networks:
    192.168.0.0 0.0.0.255 area 0
    192.168.1.0 0.0.0.255 area 0
    192.168.4.0 0.0.0.255 area 0
  Routing Information Sources:
    Gateway         Distance      Last Update
    192.168.2.1          110      00:03:24
    192.168.6.2          110      00:01:50
    192.168.6.1          110      00:00:15
  Distance: (default is 110)

```

```
// 查看所有路由
R1# show ip route 
Codes: L - local, C - connected, S - static, R - RIP, M - mobile, B - BGP
       D - EIGRP, EX - EIGRP external, O - OSPF, IA - OSPF inter area 
       N1 - OSPF NSSA external type 1, N2 - OSPF NSSA external type 2
       E1 - OSPF external type 1, E2 - OSPF external type 2
       i - IS-IS, su - IS-IS summary, L1 - IS-IS level-1, L2 - IS-IS level-2
       ia - IS-IS inter area, * - candidate default, U - per-user static route
       o - ODR, P - periodic downloaded static route, H - NHRP, l - LISP
       + - replicated route, % - next hop override

Gateway of last resort is not set

      192.168.0.0/24 is variably subnetted, 2 subnets, 2 masks
C        192.168.0.0/24 is directly connected, FastEthernet0/0
L        192.168.0.1/32 is directly connected, FastEthernet0/0
      192.168.1.0/24 is variably subnetted, 2 subnets, 2 masks
C        192.168.1.0/24 is directly connected, Serial2/0
L        192.168.1.1/32 is directly connected, Serial2/0
O     192.168.2.0/24 [110/128] via 192.168.1.2, 00:01:27, Serial2/0
O     192.168.3.0/24 [110/129] via 192.168.1.2, 00:01:27, Serial2/0
      192.168.4.0/24 is variably subnetted, 2 subnets, 2 masks
C        192.168.4.0/24 is directly connected, Serial2/1
L        192.168.4.1/32 is directly connected, Serial2/1
O     192.168.5.0/24 [110/256] via 192.168.1.2, 00:01:27, Serial2/0
O     192.168.6.0/24 [110/192] via 192.168.1.2, 00:01:27, Serial2/0


// 只查看通过 OSPF 协议学习到的路由信息
R1# show ip route ospf
Codes: L - local, C - connected, S - static, R - RIP, M - mobile, B - BGP
       D - EIGRP, EX - EIGRP external, O - OSPF, IA - OSPF inter area 
       N1 - OSPF NSSA external type 1, N2 - OSPF NSSA external type 2
       E1 - OSPF external type 1, E2 - OSPF external type 2
       i - IS-IS, su - IS-IS summary, L1 - IS-IS level-1, L2 - IS-IS level-2
       ia - IS-IS inter area, * - candidate default, U - per-user static route
       o - ODR, P - periodic downloaded static route, H - NHRP, l - LISP
       + - replicated route, % - next hop override

Gateway of last resort is not set

O     192.168.2.0/24 [110/128] via 192.168.1.2, 00:01:43, Serial2/0
O     192.168.3.0/24 [110/129] via 192.168.1.2, 00:01:43, Serial2/0
O     192.168.5.0/24 [110/256] via 192.168.1.2, 00:01:43, Serial2/0
O     192.168.6.0/24 [110/192] via 192.168.1.2, 00:01:43, Serial2/0
R1#
```

然后 PC1 和 PC2 就可以相互通信了

```
PC1> ping 192.168.3.2
84 bytes from 192.168.3.2 icmp_seq=1 ttl=61 time=58.922 ms
84 bytes from 192.168.3.2 icmp_seq=2 ttl=61 time=79.237 ms
84 bytes from 192.168.3.2 icmp_seq=3 ttl=61 time=57.636 ms
84 bytes from 192.168.3.2 icmp_seq=4 ttl=61 time=58.326 ms
84 bytes from 192.168.3.2 icmp_seq=5 ttl=61 time=58.091 ms
```

<br>

## 四、抓包 OSPF 通信过程

<br>

参考：

- [Cisco - OSPF Design Guide](https://www.cisco.com/c/en/us/support/docs/ip/open-shortest-path-first-ospf/7039-1.html)
- [Huawe - i What Is OSPF and How Is It Configured?](https://support.huawei.com/enterprise/en/doc/EDOC1100082074)
- [动态路由 - OSPF 一文详解](https://www.cnblogs.com/michael9/p/13638479.html#%E9%80%89%E5%8F%96-dr-%E5%92%8C-bdr-%E7%9A%84%E5%8C%BA%E5%88%AB)