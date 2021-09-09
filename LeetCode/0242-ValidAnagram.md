# 242. Valid Anagram

## 题目

```
Given two strings s and t, return true if t is an anagram of s, and false otherwise.

Example 1:
Input: s = "anagram", t = "nagaram"
Output: true

Constraints:
1 <= s.length, t.length <= 5 * 104
s and t consist of lowercase English letters.

Follow up: What if the inputs contain Unicode characters? How would you adapt your solution to such a case?
```

<br>


## 解法

```python3
def isAnagram(self, s: str, t: str) -> bool:
    if len(s) != len(t): return False

    record = [0] * 26
    for ch in s:
        record[ord(ch) - 97] += 1
    for ch in t:
        record[ord(ch) - 97] -= 1
    return not any(r != 0 for r in record)
```