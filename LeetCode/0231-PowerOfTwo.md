# 231. Power of Two

## 题目

```
Given an integer, write a function to determine if it is a power of two.

Example 1:
Input: 1
Output: true 
Explanation: 2^0 = 1
```

<br>

## 解法

### Python

- 第一思路  转成二进制只有一个 1 其余位都是 0

```
## 将 n 转成二进制，若二进制只有一个1 则 n 是 2 的幂次方
def isPowerOfTwo_bin(self, n: int) -> bool:
	if n <= 0: return False
	return bin(n)[3:].find('1') == -1
```

- 第一思路  除2

```
## 除以 2 有余数时 n 为 1, 则 n 是 2 的幂次方
def isPowerOfTwo_(self, n:int) -> int:
	if n<= 0: return False
	while n & 1 == 0:
		n = n >> 1
	return n == 1
```

- 借鉴思路 

```
## 若 n 是 2 的幂次方
## 则 n 的二进制最高位为1 其余位都是0，n-1除了最高位是0 其余位都是1
def isPowerOfTwo_(self, n:int) -> int:
	return n > 0 and n & (n - 1) == 0
```
