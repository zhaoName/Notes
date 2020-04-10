
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

```
def reverseWords_sys(self, s: str) -> str:
	return ' '.join(s.split()[::-1])
```