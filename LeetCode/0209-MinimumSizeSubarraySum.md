# 209. Minimum Size Subarray Sum

## 题目

```
Given an array of positive integers nums and a positive integer target, 
return the minimal length of a contiguous subarray [numsl, numsl+1, ..., numsr-1, numsr] of which the sum is greater than or equal to target. 
If there is no such subarray, return 0 instead.

Example 1:
Input: target = 7, nums = [2,3,1,2,4,3]
Output: 2
Explanation: The subarray [4,3] has the minimal length under the problem constraint.

Constraints:
1 <= target <= 10^9
1 <= nums.length <= 10^5
1 <= nums[i] <= 10^5

Follow up: If you have figured out the O(n) solution, try coding another solution of which the time complexity is O(n log(n)).
```

<br>


## 解法

- 队列

```python3
from typing import List
import sys


class Solution:
    def minSubArrayLen(self, target: int, nums: List[int]) -> int:
        if sum(nums) < target: return 0

        sumSub, left, right, n = 0, 0, 0, len(nums)
        result = n + 1

        while right < n:
            sumSub += nums[right]
            while sumSub >= target:
                result = min(result, right - left + 1)
                sumSub -= nums[left]
                left += 1
            right += 1
        return result
```

- 二分查找

```python3
# 查找 target 在数组中的 待插入位置下标
# 已存在相同元素，插在最左侧
def binarySearch_left(self, target: int, nums: List[int]) -> int:
    left, right = 0, len(nums)

    while left < right:
        mid = (left + right) >> 1
        if nums[mid] >= target:
            right = mid
        elif nums[mid] < target:
            left = mid + 1
    return left


def minSubArrayLen(self, target: int, nums: List[int]) -> int:
    ln, sums = len(nums), [0]
    ans = ln + 1
    for i in range(ln):
        sums.append(sums[-1] + nums[i])

    for i in range(1, len(sums)):
        s = target + sums[i - 1]
        # bound = bisect.bisect_left(sums, s)
        bound = self.binarySearch_left(s, sums)
        if bound != len(sums):
            ans = min(ans, bound - i + 1)
    return 0 if ans == ln + 1 else ans
```