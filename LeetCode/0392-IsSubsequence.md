# 392. Is Subsequence

## 题目


```
Given two strings s and t, check if s is a subsequence of t.

A subsequence of a string is a new string that is formed from the original string by deleting some (can be none) of the characters without disturbing the relative positions of the remaining characters. (i.e., "ace" is a subsequence of "abcde" while "aec" is not).

Example 1:
Input: s = "abc", t = "ahbgdc"
Output: true

Constraints:
0 <= s.length <= 100
0 <= t.length <= 104
s and t consist only of lowercase English letters.

Follow up: If there are lots of incoming s, say s1, s2, ..., sk where k >= 109, and you want to check one by one to see if t has its subsequence. In this scenario, how would you change your code?
```

<br>

## 解法


### Python

```
def isSubsequence(s: str, t: str) -> bool:
        sl = len(s)
        tl = len(t)
        if sl <= 0: 
            return True
        if tl <= 0:
            return False
        
        for i in range(tl - 1, -1, -1):
            if t[i] == s[sl - 1]:
                sl -= 1
            if sl <= 0:
                return True
        return False
```
