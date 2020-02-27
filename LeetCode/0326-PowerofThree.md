
# 326. Power of Three


## 题目

```
Given an integer, write a function to determine if it is a power of three.

Example:
Input: 27
Output: true

Follow up:
Could you do it without using any loop / recursion?
```


<br>

## 解法


### Python

```
## 32为有符号整数 3 的幂次方最大整数位 3^19 = 1162261467
def isPowerOfThree(self, n: int) -> bool:
	return n > 0 and 1162261467 % n == 0
```