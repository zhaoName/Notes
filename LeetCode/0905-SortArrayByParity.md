# 905. Sort Array By Parity

<br>

## 题目

```
Given an integer array nums, move all the even integers at the beginning of the array followed by all the odd integers.
Return any array that satisfies this condition.

Input: nums = [3,1,2,4]
Output: [2,4,3,1]
Explanation: The outputs [4,2,3,1], [2,4,1,3], and [4,2,1,3] would also be accepted.

Constraints:
1 <= nums.length <= 5000
0 <= nums[i] <= 5000
```

<br>


## 解法

```python3
def sortArrayByParity(self, nums: List[int]) -> List[int]:
    i , j = 0, len(nums) - 1
    while i <= j:
        while i < j and nums[i] % 2 == 0:
            # 左边是偶数
            i += 1
        while i < j and nums[j] % 2 != 0:
            # 右边是奇数
            j -= 1
        nums[i], nums[j] = nums[j], nums[i]
        i += 1
        j -= 1
    return nums
```


<br>