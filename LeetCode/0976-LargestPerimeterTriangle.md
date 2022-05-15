# 976. Largest Perimeter Triangle


<br>

## 题目

```
Given an integer array nums, return the largest perimeter of a triangle with a non-zero area, formed from three of these lengths.
If it is impossible to form any triangle of a non-zero area, return 0.

Input: nums = [2,1,2]
Output: 5

Constraints:
3 <= nums.length <= 104
1 <= nums[i] <= 106
```

<br>

## 解法

```python3
def largestPerimeter(self, nums: List[int]) -> int:
    nums.sort(reverse=True)

    for i in range(len(nums)-2):
        if nums[i] < nums[i+1] + nums[i+2]:
            return nums[i] + nums[i+1] + nums[i+2]
    return 0
```

<br>