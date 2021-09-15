# 162. Find Peak Element

## 题目

```
A peak element is an element that is strictly greater than its neighbors.
Given an integer array nums, find a peak element, and return its index. If the array contains multiple peaks, return the index to any of the peaks.
You may imagine that nums[-1] = nums[n] = -∞.
You must write an algorithm that runs in O(log n) time.

Input: nums = [1,2,1,3,5,6,4]
Output: 5
Explanation: Your function can return either index number 1 where the peak element is 2, or index number 5 where the peak element is 6.

Constraints:
1 <= nums.length <= 1000
-231 <= nums[i] <= 231 - 1
nums[i] != nums[i + 1] for all valid i.
```

<br>

## 解法

```python3
## 二分法 迭代爬坡
def findPeakElement(self, nums: List[int]) -> int:
    n = len(nums)
    ## 方便处理越界问题
    def get(i: int) -> int:
        if i == -1 or i == n:
            return float('-inf')
        return nums[i]

    left, right, ans = 0, n - 1, -1
    while left <= right:
        mid = (right + left) // 2
        if get(mid-1) < get(mid) > get(mid+1):
            ans = mid
            break
        if get(mid) < get(mid+1):
            left = mid + 1
        else:
            right = mid - 1
    return ans
```