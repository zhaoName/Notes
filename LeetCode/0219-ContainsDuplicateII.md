# 219. Contains Duplicate 


## 题目

```
Given an array of integers and an integer k, 
find out whether there are two distinct indices i and j in the array such that nums[i] = nums[j] 
and the absolute difference between i and j is at most k.

Example 1:
Input: nums = [1,2,3,1], k = 3
Output: true

Example 3:
Input: nums = [1,2,3,1,2,3], k = 2
Output: false
```


<br>

## 解法

### Python

- 第一思路 hash

```
def containsNearbyDuplicate(self, nums, k: int) -> bool:
	hash = {}
	for i in range(0, len(nums)):
		try:
			if i - hash[nums[i]] <= k:
				return True
			else:
				hash[nums[i]] = i
		except:
			hash[nums[i]] = i
	return False
```
