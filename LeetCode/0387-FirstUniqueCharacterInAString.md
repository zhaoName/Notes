# 387. First Unique Character in a String

## 题目

```
Given a string s, find the first non-repeating character in it and return its index. If it does not exist, return -1.

Example 1:
Input: s = "leetcode"
Output: 0

Constraints:
1 <= s.length <= 105
s consists of only lowercase English letters
``` 


<br>

## 解法

```python3
import collections
class Solution:
    def firstUniqChar(self, s: str) -> int:
        frequency = collections.Counter(s)
        for i, ch in enumerate(s):
            if frequency[ch] == 1:
                return i
        return -1


    # 队列中有一些字符出现了超过一次，但它只要不位于队首，那么就不会对答案造成影响
    def firstUniqChar_queue(self, s: str) -> int:
        queue, d = collections.deque(), dict()
        for index, ch in enumerate(s):
            if ch not in d:
                d[ch] = index
                queue.append((s[index], index))
            else:
                d[ch] = -1
                while queue and d[queue[0][0]] == -1:
                    queue.popleft()
        return -1 if not queue else queue[0][1]
```