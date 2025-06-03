# 128.Longest Consecutive Sequence

## 题目

```
Given an unsorted array of integers nums, return the length of the longest consecutive elements sequence.
You must write an algorithm that runs in O(n) time.

Example 1:
Input: nums = [100,4,200,1,3,2]
Output: 4
Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.

Example 2:
Input: nums = [0,3,7,2,5,8,4,6,0,1]
Output: 9

Example 3:
Input: nums = [1,0,1,2]
Output: 3

Constraints:
0 <= nums.length <= 10^5
-10^9 <= nums[i] <= 10^9
```

## 解法

```python3
def longestConsecutive(self, nums: List[int]) -> int:
    if len(nums) == 0: return 0

    numSet, ans = set(nums), 0
    for num in numSet:
        # 减1 去重
        if (num - 1) not in numSet:
            temp = 1
            while (num + 1) in numSet:
                temp += 1
                num += 1
            ans = max(ans, temp)
    return ans
```