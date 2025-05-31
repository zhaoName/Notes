# 95. 最长公共子序


## 题目

```
给定两个字符串 text1 和 text2，返回这两个字符串的最长 公共子序列 的长度。如果不存在 公共子序列 ，返回 0 。
一个字符串的 子序列 是指这样一个新的字符串：它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。

例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是 "abcde" 的子序列。
两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列。

示例 1：
输入：text1 = "abcde", text2 = "ace"
输出：3
解释：最长公共子序列是 "ace" ，它的长度为 3 。

示例 2：
输入：text1 = "abc", text2 = "abc"
输出：3
解释：最长公共子序列是 "abc" ，它的长度为 3 。

示例 3：
输入：text1 = "abc", text2 = "def"
输出：0
解释：两个字符串没有公共子序列，返回 0 。


提示：
1 <= text1.length, text2.length <= 1000
text1 和 text2 仅由小写英文字符组成。
```


## 解法

- 二维数组

```python3
# dp[i][j]: 表示长度为[0, i-1] text1 和 长度为[0, j-1] text2 最长公共子序
# dp[i][j] = dp[i-1][j-1] + 1, text1[i-1] == text2[j-1]
# dp[i][j] = max(dp[i-1][j], dp[i][j-1]), text1[i-1] != text2[j-1]
# Time Complexity: O(nm)， Space Complexity:O(nm)
def longestCommonSubsequence(self, text1: str, text2: str) -> int:
    n, m = len(text1) + 1, len(text2) + 1
    dp = [[0] * m for _ in range(n)]

    for i in range(1, n):
        for j in range(1, m):

            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])
    return dp[n - 1][m - 1]
```

- 一维数组

```python3
# 滚动数组：两个一位数组
# dp[i][j] 与三个元素有关，dp[i-][j-1]， dp[i-1][j]、dp[i][j-1]
# Time Complexity: O(nm)， Space Complexity:O(n)
def longestCommonSubsequence_02(self, text1: str, text2: str) -> int:
    m, n = len(text1) + 1, len(text2) + 1
    dp1 = [0] * n

    for i in range(1, m):
        dp2 = [0] * n
        for j in range(1, n):
            if text1[i - 1] == text2[j - 1]:
                dp2[j] = dp1[j - 1] + 1
            else:
                dp2[j] = max(dp2[j - 1], dp1[j])
        dp1 = dp2
    return dp2[n - 1]


# 上述滚动数组 只用到 dp1[j-1] 和 dp1[j] 两个元素，可优化
def longestCommonSubsequence(self, text1: str, text2: str) -> int:
    m, n = len(text1) + 1, len(text2) + 1

    dp = [0] * n
    for i in range(1, m):
        pre = 0  # 相当于dp[i-1][j-1]
        for j in range(1, n):
            cur = dp[j]  # 相当于dp[i-1][j]
            if text1[i - 1] == text2[j - 1]:
                dp[j] = pre + 1
            else:
                dp[j] = max(dp[j - 1], dp[j])
            pre = cur  # 重置dp[i-1][j-1]
    return dp[n - 1]
```