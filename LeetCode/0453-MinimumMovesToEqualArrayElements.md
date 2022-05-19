# 453. Minimum Moves to Equal Array Elements


<br>

## 题目

```
Given an integer array nums of size n, return the minimum number of moves required to make all array elements equal.
In one move, you can increment n - 1 elements of the array by 1.

Input: nums = [1,2,3]
Output: 3
Explanation: Only three moves are needed (remember each move increments two elements):
[1,2,3]  =>  [2,3,3]  =>  [3,4,3]  =>  [4,4,4]

Constraints:
n == nums.length
1 <= nums.length <= 10^5
-10^9 <= nums[i] <= 10^9
The answer is guaranteed to fit in a 32-bit integer.
```

<br>

## 解法

```python3
def minMoves(self, nums: List[int]) -> int:
    min_num = min(nums)
    res = 0
    for num in nums:
        res += num - min_num
    return res
```

<br>