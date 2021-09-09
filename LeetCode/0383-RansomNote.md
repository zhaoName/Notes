# 383. Ransom Note

## 题目

```
Given two stings ransomNote and magazine, return true if ransomNote can be constructed from magazine and false otherwise.
Each letter in magazine can only be used once in ransomNote.

Input: ransomNote = "aa", magazine = "aab"
Output: true

Constraints:
1 <= ransomNote.length, magazine.length <= 105
ransomNote and magazine consist of lowercase English letters.
```

<br>

## 解法

```python3
def canConstruct(self, ransomNote: str, magazine: str) -> bool:
    d = dict()
    for ch in magazine:
        try:
            d[ch] += 1
        except:
            d[ch] = 1
    for ch in ransomNote:
        if ch not in d:
            return False
        else:
            d[ch] -= 1
            if d[ch] < 0: return False
    return True
```