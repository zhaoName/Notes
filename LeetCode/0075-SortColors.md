# 75. Sort Colors


<br>

## 题目

```
Given an array nums with n objects colored red, white, or blue, sort them in-place so that objects of the same color are adjacent, with the colors in the order red, white, and blue.
We will use the integers 0, 1, and 2 to represent the color red, white, and blue, respectively.
You must solve this problem without using the library's sort function.

Input: nums = [2,0,1]
Output: [0,1,2]

Constraints:
n == nums.length
1 <= n <= 300
nums[i] is either 0, 1, or 2.

Follow up: Could you come up with a one-pass algorithm using only constant extra space?
```

<br>

## 解法

```python3
# 双指针 一前一后
def sortColors(self, nums: List[int]) -> None:
    """
    Do not return anything, modify nums in-place instead.
    """
    i, left, right = 0, 0, len(nums) - 1
    while i <= right:
        if nums[i] == 2:
            nums[i], nums[right] = nums[right], nums[i]
            right -= 1
        elif nums[i] == 0:
            nums[i], nums[left] = nums[left], nums[i]
            left += 1
        i += 1
```


```python3
# 双指针 两个都在前
def sortColors_(self, nums: List[int]) -> None:
    """
    Do not return anything, modify nums in-place instead.
    """
    i, left, right = 0, 0, 0
    while i < len(nums):
        if nums[i] == 1:
            nums[i], nums[right] = nums[right], nums[i]
            right += 1
        elif nums[i] == 0:
            nums[i], nums[left] = nums[left], nums[i]
            # 若 left<right, 则left对应的元素为1，上句代码会将1调换到数组的第i个元素
            # 所以需要交换 i 和 right 的位置，将 1 放在应该放的位置
            if left < right:
                nums[i], nums[right] = nums[right], nums[i]
            # 放0的位置，不会再放1，所以right也要加1
            left += 1
            right += 1
        i += 1
```

<br>