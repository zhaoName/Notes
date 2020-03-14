# 300. Longest Increasing Subsequence

## 题目

```
Given an unsorted array of integers, find the length of longest increasing subsequence.

Input: [10,9,2,5,3,7,101,18]
Output: 4 
Explanation: The longest increasing subsequence is [2,3,7,101], therefore the length is 4. 

Note:
There may be more than one LIS combination, it is only necessary for you to return the length.
Your algorithm should run in O(n2) complexity.
Follow up: Could you improve it to O(n log n) time complexity?
```

<br>

## 解法

### Python

- 第一思路 暴力求解

```
## Time Complexity:O(n^2)    Space Complexity:O(n)
def lengthOfLIS(self, nums: list) -> int:
	if not nums: return 0
	maxLen = 0
	for i in range(len(nums)):
		res = [nums[i]]
		for j in range(i+1, len(nums)):
			if res[-1] < nums[j]:
				res.append(nums[j])
			else:
				if len(res) == 1:
					res[-1] = nums[j]
				elif len(res) >= 2 and res[-2] < nums[j]:
					res[-1] = nums[j]
		print(len(res))
		if maxLen < len(res):
			maxLen = len(res)
	return maxLen
```