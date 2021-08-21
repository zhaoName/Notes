# 209. Minimum Size Subarray Sum

## 题目

```
Given an array of positive integers nums and a positive integer target, 
return the minimal length of a contiguous subarray [numsl, numsl+1, ..., numsr-1, numsr] of which the sum is greater than or equal to target. 
If there is no such subarray, return 0 instead.

Example 1:
Input: target = 7, nums = [2,3,1,2,4,3]
Output: 2
Explanation: The subarray [4,3] has the minimal length under the problem constraint.

Constraints:
1 <= target <= 10^9
1 <= nums.length <= 10^5
1 <= nums[i] <= 10^5

Follow up: If you have figured out the O(n) solution, try coding another solution of which the time complexity is O(n log(n)).
```

<br>


## 解法

```
from typing import List
import sys

class Solution:
    def minSubArrayLen(self, target: int, nums: List[int]) -> int:
    	if sum(nums) < target: return 0

    	sumSub, left, right, n = 0, 0, 0, len(nums)
    	result = n + 1
    	
    	while right < n:
    		sumSub += nums[right]
    		while sumSub >= target:
    			result = min(result, right-left+1)
    			sumSub -= nums[left]
    			left += 1
    		right += 1
    	return result
```