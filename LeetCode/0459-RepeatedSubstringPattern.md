
# 459. Repeated Substring Pattern

## 题目

```
Given a non-empty string check if it can be constructed by taking a substring of it and appending multiple copies of the substring together. 
You may assume the given string consists of lowercase English letters only and its length will not exceed 10000.
```

<br>

## 解法

### Python

- 切片

```
def repeatedSubstringPattern(s: str) -> bool:
	n = len(s)
	for i in range(0, n//2 + 1):
		if n % i == 0 and s[:i] * n//i == s:
			return True
	return False
```

- find函数

```
## 从 s + s 下标为1的元素中查找 s，若find返回的值小于len(s) 则 s 存在重复的子串
def repeatedSubstringPattern_find(s: str) -> bool:
	return (s + s).find(s, 1) != len(s)
```