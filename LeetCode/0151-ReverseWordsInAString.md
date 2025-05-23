
# 151. Reverse Words in a String


## 题目

```
Given an input string, reverse the string word by word.

Input: "a good   example"
Output: "example good a"
Explanation: You need to reduce multiple spaces between two words to a single space in the reversed string.

Note:
A word is defined as a sequence of non-space characters. 
Input string may contain leading or trailing spaces. However, your reversed string should not contain leading or trailing spaces.
You need to reduce multiple spaces between two words to a single space in the reversed string.

Follow up:
For C programmers, try to solve it in-place in O(1) extra space.
```

<br>

## 解法

### Python

- 第一思路

```python3
def reverseWords_sys(self, s: str) -> str:
	return ' '.join(s.split()[::-1])
```

- 反转

```python3
def reverseWords(self, s: str) -> str:
	hasSpace, cur, listS = True, 0, list(s)
	for fast in range(len(listS)):
		if listS[fast] != ' ':
			listS[cur] = listS[fast]
			cur += 1
			hasSpace = False
		elif not hasSpace:
			# fast是空格 && 前一个字符不是空格
			listS[cur] = listS[fast]
			cur += 1
			hasSpace = True
	lenS = cur - 1 if listS[-1] == ' ' else cur

	# 反转
	def swap(left: int, right: int):
		while left < right:
			listS[left], listS[right] = listS[right], listS[left]
			left += 1
			right -= 1

	# 先整体反转
	swap(0, lenS - 1)
	# 再局部反转
	i = 0
	for j in range(lenS):
		if listS[j] == ' ':
			swap(i, j - 1)
			i = j + 1
	# 再反转最后一个单词
	swap(i, lenS - 1)
	return ''.join(listS[:lenS])
```
