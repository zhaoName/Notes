
# 344. Reverse String

## 题目

```
Given a string and an integer k, you need to reverse the first k characters for every 2k characters counting from the start of the string. 
If there are less than k characters left, reverse all of them. 
If there are less than 2k but greater than or equal to k characters, 
then reverse the first k characters and left the other as original.

Example:
Input: s = "abcdefg", k = 2
Output: "bacdfeg"

Restrictions:
1.The string consists of lower English letters only.
2.Length of the given string and k will in the range [1, 10000]
```

<br>

## 解法

### Python

- 第一思路 转换成 list

```
## Time Complexity:O(n)   Space Complexity:O(n)
def reverseStr_list(self, s: str, k: int) -> str:
	if not s: return None
	lt = list(s)
	for i in range(0, len(s), 2*k):
		lt[i:i+k] = lt[i:i+k][::-1]
	return ''.join(lt)
```


- 第一思路 

```
## Time Complexity:O(n)   Space Complexity:O(1)
def reverseStr_str(self, s: str, k: int) -> str:
	if not s: return None
	for i in range(0, len(s), 2*k):
		s = s.replace(s[i:i+k], s[i:i+k][::-1])
	return s
```