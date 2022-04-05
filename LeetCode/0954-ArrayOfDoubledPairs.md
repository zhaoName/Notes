# 954. Array of Doubled Pairs

<br>

## 题目

```
Given an integer array of even length arr, return true if it is possible to reorder arr
such that arr[2 * i + 1] = 2 * arr[2 * i] for every 0 <= i < len(arr) / 2, or false otherwise.

Input: arr = [4,-2,2,-4]
Output: true
Explanation: We can take two groups, [-2,-4] and [2,4] to form [-2,-4,2,4] or [2,4,-2,-4].

Constraints:
2 <= arr.length <= 3 * 104
arr.length is even.
-105 <= arr[i] <= 105
```


<br>


## 解法

```python
class Solution:
    def canReorderDoubled(self, arr: List[int]) -> bool:
        cnt = collections.Counter(arr)
        # 数组中的 0 须是偶数个
        if cnt[0] % 2:
            return False
        for x in sorted(cnt, key=abs):
            if cnt[2 * x] < cnt[x]:  # 无法找到足够的 2x 与 x 配对
                return False
            cnt[2 * x] -= cnt[x]
        return True
```

