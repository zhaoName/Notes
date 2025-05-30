# 47.Permutations II


## 题目

```
Given a collection of numbers, nums, that might contain duplicates, return all possible unique permutations in any order.

Example 1:
Input: nums = [1,1,2]
Output:
[[1,1,2], [1,2,1], [2,1,1]]

Example 2:
Input: nums = [1,2,3]
Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

Constraints:
1 <= nums.length <= 8
-10 <= nums[i] <= 10
```

## 解法

```python3
def permuteUnique(self, nums: List[int]) -> List[List[int]]:
    if not nums: return []

    n, res = len(nums), []
    def backtracking(startIndex: int):
        if startIndex == n - 1:
            res.append(nums[:])
            return
        s = set()
        for i in range(startIndex, n):
            if nums[i] in s:
                continue
            s.add(nums[i])
            nums[i], nums[startIndex] = nums[startIndex], nums[i]
            backtracking(startIndex + 1)
            # 回溯
            nums[i], nums[startIndex] = nums[startIndex], nums[i]

    backtracking(0)
    return res
```