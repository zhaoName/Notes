
# 258. Add Digits


## 题目

```
Given a non-negative integer num, repeatedly add all its digits until the result has only one digit.

Example:
Input: 38
Output: 2 
Explanation: The process is like: 3 + 8 = 11, 1 + 1 = 2. 
             Since 2 has only one digit, return it.
Follow up:
Could you do it without any loop/recursion in O(1) runtime?
```

<br>


## 解法


### python

- 第一思路 

```
def addDigits_str(self, num: int) -> int:
	if num < 0: return -1
	while num >= 10:
		sum = 0
		for a in str(num):
			sum += ord(a)-48
		num = sum
	return num
```


- 借鉴思路

```
## abc = a*100 + b*10 + c
## result = a + b + c
## abc - result = a*99 + b*9
## result = abc - 9*(11*a + b)
## 也就是说 结果是 num 对 9取余
def addDigits_(self, num: int) -> int:
	if num == 0: return 0
	return num % 9 if num % 9 != 0 else 9
```