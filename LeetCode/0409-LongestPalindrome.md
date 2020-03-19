# 409. Longest Palindrome


## 题目

```
Given a string which consists of lowercase or uppercase letters, 
find the length of the longest palindromes that can be built with those letters.
This is case sensitive, for example "Aa" is not considered a palindrome here.
Note:
Assume the length of given string will not exceed 1,010.

Input: "abccccdd"
Output: 7
Explanation:
One longest palindrome that can be built is "dccaccd", whose length is 7.
```

<br>

## 解法

### Python

- 第一思路

```
def longestPalindrome(self, s: str) -> int:
	if not s: return 0
	ans, numD, temp = 0, {}, False
	for a in s:
		try:
			numD[a] += 1
		except:
			numD[a] = 1
	for value in numD.values():
		if value % 2 == 0:
			ans += value
		elif value > 2 and value % 2 != 0:
			temp = True
			ans += value - 1
		else:
			temp = True
	return ans+1 if temp else ans
```

- 位运算 快速去偶数

```
def longestPalindrome(self, s: str) -> int:
    if not s: return 0
    ans, numD = 0, {}
    for a in s:
        try:
            numD[a] += 1
        except:
            numD[a] = 1
    for value in numD.values():
        ans += value - (value & 1)
    return ans + 1 if ans < len(s) else ans
```







