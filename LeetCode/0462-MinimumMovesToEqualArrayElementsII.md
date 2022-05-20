# 462. Minimum Moves to Equal Array Elements II


<br>

## 题目

```
Given an integer array nums of size n, return the minimum number of moves required to make all array elements equal.
In one move, you can increment or decrement an element of the array by 1.
Test cases are designed so that the answer will fit in a 32-bit integer.

Input: nums = [1,2,3]
Output: 2
Explanation:
Only two moves are needed (remember each move increments or decrements one element):
[1,2,3]  =>  [2,2,3]  =>  [2,2,2]

Constraints:
n == nums.length
1 <= nums.length <= 10^5
-10^9 <= nums[i] <= 10^9
```

<br>

## 解法

```python3
def minMoves2(self, nums: List[int]) -> int:
    ans = 0
    nums.sort()
    target = nums[len(nums) // 2]
    for j in range(len(nums)):
        ans += abs(nums[j] - target)
    return ans
```

<br>