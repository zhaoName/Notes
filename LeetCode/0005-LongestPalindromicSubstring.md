# 5. Longest Palindromic Substring

## 题目

```
Given a string s, return the longest palindromic substring in s.

Example 1:
Input: s = "babad"
Output: "bab"
Note: "aba" is also a valid answer.

Constraints:
1 <= s.length <= 1000
s consist of only digits and English letters.
```

解法

- 动态规划

```python3
# dp[i][j] = dp[i+1][j-1] && s[i] == s[j]
# 临界条件： j-1 - (i+1) + 1 < 2 ==> j-i < 3
def longestPalindrome_dp(self, s: str) -> str:
    ln, maxLen, start = len(s), 1, 0
    # 构建 dp 数组
    dp = [[False] * ln for _ in range(ln)]
    # 对角线上 长度为1 必是回文串
    for i in range(ln):
        dp[i][i] = True

    # 先左下角
    for j in range(1, ln):
        for i in range(j):
            if s[i] != s[j]:
                dp[i][j] = False
            else:
                if j - i < 3:
                    dp[i][j] = True
                else:
                    dp[i][j] = dp[i + 1][j - 1]
            if dp[i][j] and j - i + 1 > maxLen:
                maxLen = j - i + 1
                start = i
    return s[start:start + maxLen]
```

- 中心扩展

```python3
# 中心扩展
def longestPalindrome_center(self, s: str) -> str:
    ln, left, right = len(s), 0, 0
    maxLen, tempLen, start = 0, 1, 0

    for i in range(ln):
        left = i - 1
        right = i + 1
        while left >= 0 and s[left] == s[i]:
            left -= 1
            tempLen += 1
        while right < ln and s[right] == s[i]:
            right += 1
            tempLen += 1
        while left >= 0 and right < ln and s[left] == s[right]:
            left -= 1
            right += 1
            tempLen += 2
        if tempLen > maxLen:
            maxLen = tempLen
            start = left + 1
        tempLen = 1
    return s[start:start + maxLen]
```