# 704. Binary Search

<br>

## 题目

```
Given an array of integers nums which is sorted in ascending order, and an integer target, write a function to search target in nums.
If target exists, then return its index. Otherwise, return -1.
You must write an algorithm with O(log n) runtime complexity.

Example 1:
Input: nums = [-1,0,3,5,9,12], target = 9
Output: 4
Explanation: 9 exists in nums and its index is 4

Constraints:
1 <= nums.length <= 104
-104 < nums[i], target < 104
All the integers in nums are unique.
nums is sorted in ascending order.
```

<br>

## 解法

```python3
from typing import List

class Solution:
    def search1(self, nums: List[int], target: int) -> int:
        return self.binary(0, len(nums)-1, nums, target)


    def binary(self, left: int, right: int, nums: List[int], target: int) -> int:
        if left > right: return -1
        middle = left + (right - left) // 2

        if nums[middle] == target:
            return middle
        elif nums[middle] > target:
            return self.binary(left, middle-1, nums, target)
        else:
            return self.binary(middle+1, right, nums, target)



    def search(self, nums: List[int], target: int) -> int:
        left, right = 0, len(nums) - 1
        while left < right:
            middle = left + (right - left) // 2
            if nums[middle] == target:
                return middle
            elif nums[middle] > target:
                right = middle - 1
            else:
                left = middle + 1
        return -1

```