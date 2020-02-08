
# 125. Valid Palindrome


## 题目

```
Given a string, determine if it is a palindrome, 
considering only alphanumeric characters and ignoring cases.

Note: For the purpose of this problem, 
we define empty string as valid palindrome.

Example 1:
Input: "A man, a plan, a canal: Panama"
Output: true

Example 2:
Input: "race a car"
Output: false
```

<br>


## 解法

### Python


```
## 数字的ASCII范围[48, 57], 小写字母ASCII范围[97, 122]
## Time Complexity: O(n)  Space Complexity:O(n)
def isPalindrome_ascii(self, s: str) -> bool:
	if not s: return True

	temp = ""
	s = s.lower()
	for i in s:
		if 48<= ord(i) <=57 or 97<= ord(i) <=122:
			temp += i
	return temp == temp[::-1]
```

