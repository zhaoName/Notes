# 应用层 - DNS
 
<br> 

## 一、DNS

### 0x01 为什么需要 DNS

用户和互联网某天主机通信时，必须知道对方的 IP 地址。然后用户很难记住长达 32 位的主机地址，即使是点分十进制 IP 地址也不方便记忆。

DNS (Domain Name System) 域名系统是互联网上作为域名和 IP 地址相互映射的一个分布式数据库，能够把互联网上的主机名转换为 IP 地址。

### 0x0 DNS 服务器

![](../Images/Network/DNS/DNS_images01.png)

- 根域名服务器 (root name server)：最高层次的域名服务器，所有的根域名服务器都知道所有的顶级域名服务器的域名和 IP 地址。

- 顶级域名服务器 (top level domain server)：管理着在该顶级域名服务器注册的所有二级域名

- 权限域名服务器 (authoritative name server)：用来保护该区中所有主机的域名到 IP 地址的映射。

- 本地域名服务器：

<br> 

## 二、域名解析过程

主机向本地域名服务器的查询一般是递归查询 (recursive query)，本地域名服务器向根域名服务器的查询一般是迭代查询 (iterative query)。

递归查询：若主机所询问的本地域名服务器不知道被查询的 IP 地址，那么本地域名服务器就以 DNS 客户的身份，向其他根域名服务器发出查询报文，而不是该主机进行下一步查询。

迭代查询：当根域名服务器收到本地域名服务器发出的迭代查询请求后，要么给出所要查询的 IP 地址，要么告诉本地域名服务器自己所知道的顶级域名服务器的 IP 地址。然后本地域名服务器向顶级域名服务器发出查询请求。本地域名服务器就这样本地进行查询迭代。

![](../Images/Network/DNS/DNS_images02.png)

如上图 (a) 假设域名为 m.xyz.com 的主机想知道另一台主机 (y.abc.com) 的 IP 地址 (忽略缓存)

- 主机 m.xyz.com 先向本地域名服务器 dns.xyz.com 进行递归查询 (本地域名服务器缓存中有，会直接返回对应的 IP 地址)

- 本地域名服务器采用迭代查询，先向根域名服务器发出查询 dns.xyz.com 对应 IP 地址的请求
- 根域名服务器会告诉本地域名服务器自己所知道的 dns.com 顶级域名服务器对应的 IP 地址
- 本地域名服务器知道顶级域名服务器 IP 地址，就会向其发出查询  dns.xyz.com 对应 IP 地址的请求
- 顶级域名服务器 dns.com 告诉本地域名服务器，下一次应查询权限域名服务器 dns.abc.com 的 IP 地址
- 本地域名服务器向权限域名服务器 dns.abc.com 进行查询
- 权限域名服务器 dns.abc.com 告诉本地域名服务器，所查询主机的 IP 地址
- 本地域名服务器将查询的结果告诉主机 m.xyz.com (并缓存到本地)


命令查看域名解析过程 

```
$ dig +trace www.baidu.com

; <<>> DiG 9.10.6 <<>> +trace www.baidu.com
;; global options: +cmd
.			2393	IN	NS	h.root-servers.net.
.			2393	IN	NS	j.root-servers.net.
.			2393	IN	NS	c.root-servers.net.
.			2393	IN	NS	e.root-servers.net.
.			2393	IN	NS	l.root-servers.net.
.			2393	IN	NS	i.root-servers.net.
.			2393	IN	NS	k.root-servers.net.
.			2393	IN	NS	d.root-servers.net.
.			2393	IN	NS	b.root-servers.net.
.			2393	IN	NS	f.root-servers.net.
.			2393	IN	NS	g.root-servers.net.
.			2393	IN	NS	m.root-servers.net.
.			2393	IN	NS	a.root-servers.net.
;; Received 239 bytes from 192.168.0.1#53(192.168.0.1) in 15 ms

com.			172800	IN	NS	m.gtld-servers.net.
com.			172800	IN	NS	b.gtld-servers.net.
com.			172800	IN	NS	l.gtld-servers.net.
com.			172800	IN	NS	f.gtld-servers.net.
com.			172800	IN	NS	e.gtld-servers.net.
com.			172800	IN	NS	c.gtld-servers.net.
com.			172800	IN	NS	h.gtld-servers.net.
com.			172800	IN	NS	j.gtld-servers.net.
com.			172800	IN	NS	g.gtld-servers.net.
com.			172800	IN	NS	d.gtld-servers.net.
com.			172800	IN	NS	i.gtld-servers.net.
com.			172800	IN	NS	a.gtld-servers.net.
com.			172800	IN	NS	k.gtld-servers.net.
com.			86400	IN	DS	30909 8 2 E2D3C916F6DEEAC73294E8268FB5885044A833FC5459588F4A9184CF C41A5766
com.			86400	IN	RRSIG	DS 8 1 86400 20210328050000 20210315040000 42351
;; Received 1173 bytes from 202.12.27.33#53(m.root-servers.net) in 83 ms

baidu.com.		172800	IN	NS	ns2.baidu.com.
baidu.com.		172800	IN	NS	ns3.baidu.com.
baidu.com.		172800	IN	NS	ns4.baidu.com.
baidu.com.		172800	IN	NS	ns1.baidu.com.
baidu.com.		172800	IN	NS	ns7.baidu.com.
CK0POJMG874LJREF7EFN8430QVIT8BSM.com. 86400 IN NSEC3 1 1 0 - CK0Q1GIN43N1ARRC9OSM6QPQR81H5M9A  NS SOA RRSIG DNSKEY NSEC3PARAM
CK0POJMG874LJREF7EFN8430QVIT8BSM.com. 86400 IN RRSIG NSEC3 8 2 86400 
HPVUSBDNI26UDNIV6R0SV14GC3KGR4JP.com. 86400 IN NSEC3 1 1 0 - HPVVN3Q5E5GOQP2QFE2LEM4SVB9C0SJ6  NS DS RRSIG
HPVUSBDNI26UDNIV6R0SV14GC3KGR4JP.com. 86400 IN RRSIG NSEC3 8 2 86400 20210321071459 20210314060459 58540 com. 
;; Received 761 bytes from 192.48.79.30#53(j.gtld-servers.net) in 244 ms

www.baidu.com.		1200	IN	CNAME	www.a.shifen.com.
a.shifen.com.		1200	IN	NS	ns5.a.shifen.com.
a.shifen.com.		1200	IN	NS	ns1.a.shifen.com.
a.shifen.com.		1200	IN	NS	ns2.a.shifen.com.
a.shifen.com.		1200	IN	NS	ns3.a.shifen.com.
a.shifen.com.		1200	IN	NS	ns4.a.shifen.com.
;; Received 239 bytes from 220.181.33.31#53(ns2.baidu.com) in 31 ms
```

<br> 

## 三、DNS 数据报格式


<br> 


<br> 