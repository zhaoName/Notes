# 342. Power of Four


## 题目

```
Given an integer (signed 32 bits), write a function to check whether it is a power of 4.

Example:
Input: 16
Output: true

Follow up: Could you solve it without loops/recursion?
```

<br>

## 解法

### Python

- 第一思路

```
## 4的幂次方 1出现一次 且出现在奇数位
def isPowerOfFour_odd(self, num: int) -> bool:
	if num <= 0: return False
	return num & (num - 1) == 0 and len(bin(num)) % 2 != 0
```


- 借鉴思路

```
## 2^2k mod 3 = 4^k mod 3 = (3+1)^K mod 3 = 1
## 2^(2k+1) mod 3 = (4^k * 2) mod 3 = (2* (3+1)^k) mod 3 = 2
def isPowerOfFour_(self, num: int) -> bool:
	return num > 0 and num & (num - 1) == 0 and num % 3 == 1
```