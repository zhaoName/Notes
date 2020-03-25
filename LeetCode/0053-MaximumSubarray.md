# 53. Maximum Subarray


## 题目

```
Given an integer array nums, find the contiguous subarray (containing at least one number) which has the largest sum and return its sum.

Input: [-2,1,-3,4,-1,2,1,-5,4],
Output: 6
Explanation: [4,-1,2,1] has the largest sum = 6.

Follow up:
If you have figured out the O(n) solution, try coding another solution using the divide and conquer approach, which is more subtle.
```

<br>

## 解法

### Python

- 第一思路 动态规划 dp

```
def maxSubArray(self, nums) -> int:
	if not nums: return 0
	largest, dp = nums[0], [0] * len(nums)
	dp[0] = nums[0]
	for i in range(1, len(nums)):
		dp[i] = max(dp[i-1] + nums[i], nums[i])
		if largest < dp[i]:
			largest = dp[i]
	return largest
```