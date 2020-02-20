
# 263. Ugly Number

##  题目

```
Write a program to check whether a given number is an ugly number.
Ugly numbers are positive numbers whose prime factors only include 2, 3, 5.

Example 1:
Input: 8
Output: true
Explanation: 8 = 2 × 2 × 2

Example 2:
Input: 14
Output: false 
Explanation: 14 is not ugly since it includes another prime factor 7.

Note:
- 1 is typically treated as an ugly number.
- Input is within the 32-bit signed integer range: [−231,  231 − 1].
```


<br>

## 解法

### Python

- 借鉴思路

```
def isUgly(self, num) -> bool:
	if num <=0: return False
	for a in [2, 3, 5]:
		while num % a == 0:
			num //= a
	return num == 1
```