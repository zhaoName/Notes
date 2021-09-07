# 1221. Split a String in Balanced Strings

## 题目

```
Balanced strings are those that have an equal quantity of 'L' and 'R' characters.
Given a balanced string s, split it in the maximum amount of balanced strings.
Return the maximum amount of split balanced strings.

Example 1:
Input: s = "RLRRLLRLRL"
Output: 4
Explanation: s can be split into "RL", "RRLL", "RL", "RL", each substring contains same number of 'L' and 'R'.

Constraints:
1 <= s.length <= 1000
s[i] is either 'L' or 'R'.
s is a balanced string.
```

<br>

## 解法

```python3
class Solution:
    def balancedStringSplit(self, s: str) -> int:
        num, ans = 0, 0
        for char in s:
            if char == 'L':
                num += 1
            elif char == 'R':
                num -= 1
            if num == 0:
                ans += 1
        return ans
```

