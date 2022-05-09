# 942. DI String Match

<br>

## 题目

```
A permutation perm of n + 1 integers of all the integers in the range [0, n] can be represented as a string s of length n where:
s[i] == 'I' if perm[i] < perm[i + 1], and
s[i] == 'D' if perm[i] > perm[i + 1].
Given a string s, reconstruct the permutation perm and return it. If there are multiple valid permutations perm, return any of them.

Input: s = "IDID"
Output: [0,4,1,3,2]

Constraints:
1 <= s.length <= 105
s[i] is either 'I' or 'D'.
```

<br>

## 解法

```python3
def diStringMatch(self, s: str) -> List[int]:
    ans, i, j = [], 0, len(s)
    for m in range(len(s)):
        if s[m] == "I":
            ans.append(i)
            i += 1
        else:
            ans.append(j)
            j -= 1
    ans.append(i)
    return ans
```

<br>