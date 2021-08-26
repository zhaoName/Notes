# 283. Move Zeroes

## 题目

```
Given an integer array nums, move all 0's to the end of it while maintaining the relative order of the non-zero elements.
Note that you must do this in-place without making a copy of the array.

Example 1:
Input: nums = [0,1,0,3,12]
Output: [1,3,12,0,0]

Constraints:
1 <= nums.length <= 104
-231 <= nums[i] <= 231 - 1

Follow up: Could you minimize the total number of operations done?
```

<br>


## 解法

```python3
from typing import List

class Solution:
	# Do not return anything, modify nums in-place instead.
    def moveZeroes(self, nums: List[int]) -> None:
    	j = 0
    	# 第一次遍历的时候，j指针记录非0的个数，只要是非0的统统都赋给nums[j
    	for i in range(len(nums)):
    		if nums[i]:
    			nums[j] = nums[i]
    			j += 1
    	# 非0元素统计完了，剩下的都是0了
		# 所以第二次遍历把末尾的元素都赋为0即可
    	for i in range(j,len(nums)):
    		nums[i] = 0
```