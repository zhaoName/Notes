
# 567. Permutation in String


## 题目

```
Given two strings s1 and s2, return true if s2 contains a permutation of s1, or false otherwise.
In other words, return true if one of s1's permutations is the substring of s2. 

Example 1:
Input: s1 = "ab", s2 = "eidbaooo"
Output: true
Explanation: s2 contains one permutation of s1 ("ba").

Constraints:
1 <= s1.length, s2.length <= 104
s1 and s2 consist of lowercase English letters.
```

<br>

## 解法

```python
def checkInclusion(self, s1: str, s2: str) -> bool:
    # 统计 s1 中每个字符出现的次数
    counter1 = collections.Counter(s1)
    N = len(s2)
    # 定义滑动窗口的范围是 [left, right]，闭区间，长度与s1相等
    left = 0
    right = len(s1) - 1
    # 统计窗口s2[left, right - 1]内的元素出现的次数
    counter2 = collections.Counter(s2[0:right])
    while right < N:
        # 把 right 位置的元素放到 counter2 中
        counter2[s2[right]] += 1
        # 如果滑动窗口内各个元素出现的次数跟 s1 的元素出现次数完全一致，返回 True
        if counter1 == counter2:
            return True
        # 窗口向右移动前，把当前 left 位置的元素出现次数 - 1
        counter2[s2[left]] -= 1
        # 如果当前 left 位置的元素出现次数为 0， 需要从字典中删除，否则这个出现次数为 0 的元素会影响两 counter 之间的比较
        if counter2[s2[left]] == 0:
            del counter2[s2[left]]
        # 窗口向右移动
        left += 1
        right += 1
    return False
```


