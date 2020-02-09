
# 28. Implement strStr()


## 题目

```
Implement strStr().

Return the index of the first occurrence of needle in haystack, 
or -1 if needle is not part of haystack.

Example 1:
Input: haystack = "hello", needle = "ll"
Output: 2

Example 2:
Input: haystack = "aaaaa", needle = "bba"
Output: -1

Clarification:
What should we return when needle is an empty string? 
This is a great question to ask during an interview.
For the purpose of this problem, we will return 0 when needle is an empty string.
This is consistent to C's strstr() and Java's indexOf().
```

<br>

## 解法


### Python

- 暴力求解 - 双指针

```
## 暴力求解 Time Complexity:O(n*m), Space Complecity:O(1)
def strStr_simple(haystack: str, needle: str) -> int:
	if not needle: return 0
	if not haystack: return -1
	for i in range(0, len(haystack)-len(needle)+1):
		for j in range(0, len(needle)):
			if haystack[i+j] != needle[j]:
				break
			if j == len(needle)-1:
				return i
	return -1
```


- sunday 算法

```
'''
sunday 算法:
每次匹配都会从 haystack 提取 待匹配字符串 与 needle 进行匹配
- 若匹配，则返回当前 idx
- 不匹配，则查看 待匹配字符串 的后一位字符 c：
	- 若c存在于needle中，则 idx = idx + 偏移表[c]
	- 否则，idx = idx + len(needle)
Repeat Loop 直到 idx + len(needle) > len(haystack)

最坏时间复杂度O(n*m)  平均时间复杂度:O(n)
'''
def strStr_sunday(haystack: str, needle: str) -> int:
	if not needle: return 0
	if len(needle) > len(haystack): return -1

	offsetD = sunday_offset_dict(needle)
	idx = 0
	while idx + len(needle) <= len(haystack):
		## 待匹配字符串
		str_cut = haystack[idx:idx+len(needle)]
		if str_cut == needle:
			return idx
		else:
			## 注意这里有个坑
			if idx + len(needle) >= len(haystack):
				return -1
			## 待匹配字符串的后一个字符 c
			after = haystack[idx + len(needle)]
			if offsetD.get(after):
				## 如 C 出现在模式串中
				idx = idx + offsetD[after]
			else:
				## 如 C 未出现在模式串中
				idx = idx + len(needle) + 1
	return -1 if idx + len(needle) > len(haystack) else idx


## 计算偏移表 偏移表的作用是存储每一个在「模式串」中出现的字符，在「模式串」中出现的最右位置到尾部的距离 +1
def sunday_offset_dict(needle: str) -> dict:
	offsetDict = {}
	for i in range(len(needle)-1, -1, -1):
		if not offsetDict.get(needle[i]):
			offsetDict[needle[i]] = len(needle) - i
	return offsetDict
```




