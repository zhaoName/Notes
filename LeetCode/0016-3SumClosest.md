# 16. 3Sum Closest


## 题目

```
Given an array nums of n integers and an integer target, find three integers in nums such that the sum is closest to target. 
Return the sum of the three integers. You may assume that each input would have exactly one solution.

Given array nums = [-1, 2, 1, -4], and target = 1.
The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
```

<br>

## 解法


### Python

```
def threeSumClosest(self, nums: list, target: int) -> int:
	nums.sort(reverse=True)
	res, i = nums[0] + nums[1] + nums[2], 0
	if res <= target:
		return res
	if nums[-1] + nums[-2] + nums[-3] >= target:
		return nums[-1] + nums[-2] + nums[-3]
	while i < len(nums):
		left, right = i + 1, len(nums) - 1
		while left < right:
			temp = nums[i] + nums[left] + nums[right]
			if abs(temp - target) < abs(res - target):
				res = temp
			if temp < target:
				right -= 1
			elif temp > target:
				left += 1
			else:
				return res
		i += 1
	return res
```