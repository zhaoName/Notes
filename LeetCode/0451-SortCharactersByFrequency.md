# 451.Sort Characters by Frequency


## 题目

```
Given a string s, sort it in decreasing order based on the frequency of the characters.
The frequency of a character is the number of times it appears in the string.
Return the sorted string. If there are multiple answers, return any of them.

Example 1:
Input: s = "tree"
Output: "eert"
Explanation: 'e' appears twice while 'r' and 't' both appear once.
So 'e' must appear before both 'r' and 't'. Therefore "eetr" is also a valid answer.

Example 3:
Input: s = "Aabb"
Output: "bbAa"
Explanation: "bbaA" is also a valid answer, but "Aabb" is incorrect.
Note that 'A' and 'a' are treated as two different characters.


Constraints:
1 <= s.length <= 5 * 10^5
s consists of uppercase and lowercase English letters and digits.
```


## 解法

```python3
import heapq

class Solution:
    def frequencySort(self, s: str) -> str:
        dict = {}
        for c in s:
            dict[c] = dict.get(c, 0) + 1

        heap = []
        for c, count in dict.items():
            heapq.heappush(heap, (-count, c))

        ans = ''
        while heap:
            count, c = heapq.heappop(heap)
            ans += c*(-count)
        return ans
```
