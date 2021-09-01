# 198. House Robber


## 题目

> You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed,
the only constraint stopping you from robbing each of them is that adjacent houses have security systems connected and
it will automatically contact the police if two adjacent houses were broken into on the same night.
Given an integer array nums representing the amount of money of each house, return the maximum amount of money you can rob tonight without alerting the police.

> Example 2:
Input: nums = [2,7,9,3,1]
Output: 12
Explanation: Rob house 1 (money = 2), rob house 3 (money = 9) and rob house 5 (money = 1).
Total amount you can rob = 2 + 9 + 1 = 12.

>Constraints:
1 <= nums.length <= 100
0 <= nums[i] <= 400

<br>

## 解法

```python3
class Solution:
    ## dp数组
    def rob_dp(self, nums: List[int]) -> int:
        n = len(nums)
        if n == 1: return nums[0]
        res = max(nums[0], nums[1])
        dp = [nums[0], res]
        for i in range(2, n):
            dpi = nums[i] + dp[i-2]
            if dpi > res:
                res = dpi
                dp.append(dpi)
            else:
                dp.append(res)
        return res


    ## 滚动存值
    def rob(self, nums: List[int]) -> int:
        n = len(nums)
        if n == 1: return nums[0]

        s1, s2 = nums[0], max(nums[0], nums[1])
        for i in range(2, n):
            s1, s2 = s2, max(s2, (nums[i]+s1))
        return s2
```