# 9. Palindrome Number


## 题目

```
Determine whether an integer is a palindrome. 
An integer is a palindrome when it reads the same backward as forward.

Follow up:
Coud you solve it without converting the integer to a string?
```

<br>


## 解法

### Python

- 第一思路  数字转字符串 反转一半

```
## Time Complexity:O(n)  Space Complexity:O(n) n为x的长度
def isPalindrome_str(self, x) -> bool:
	if x < 0: return False
	if x >=0 and x <10: return True

	s = str(x)
	mid = len(s) // 2
	left, rifht = '', ''
	if len(s) % 2 == 0:
		left = s[:mid]
		right = s[mid:]
	else:
		left = s[:mid]
		right = s[mid+1:]

	return left == right[::-1]
```


- 优化 不转字符串 算后一半余数和前一半比较

```
## Time Complexity:O(n)  Space Complexity:O(1) n为x的长度
def isPalindrome_half(self, x) -> bool:
	if x < 0 or (x % 10 == 0 and x != 0): return False

	revertedNum = 0
	while x > revertedNum:
		revertedNum = revertedNum * 10 + x % 10
		x = x // 10
	## x 可能为奇数 这时 revertedNum 可能多一位
	return x == revertedNum or x == revertedNum // 10
```


- 优化  拿整数两端的数字依次比较

```
Space Complexity:O(1) n为x的长度
def isPalindrom_max(self, x) -> bool:
	if x < 0: return False
	max = 1
	while x // max >= 10:
		max *= 10

	while x > 0:
		left = x // max
		right = x % 10
		if left != right: return False
		x = x - left*max - right
		max //= 100
	return True
```





