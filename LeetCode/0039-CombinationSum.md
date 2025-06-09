# 39.Combination Sum

## 题

```
Given an array of distinct integers candidates and a target integer target,
return a list of all unique combinations of candidates where the chosen numbers sum to target. You may return the combinations in any order.

The same number may be chosen from candidates an unlimited number of times.
Two combinations are unique if the frequency of at least one of the chosen numbers is different.
The test cases are generated such that the number of unique combinations
that sum up to target is less than 150 combinations for the given input.

Example 1:
Input: candidates = [2,3,6,7], target = 7
Output: [[2,2,3],[7]]
Explanation:
2 and 3 are candidates, and 2 + 2 + 3 = 7. Note that 2 can be used multiple times.
7 is a candidate, and 7 = 7.
These are the only two combinations.

Example 2:
Input: candidates = [2,3,5], target = 8
Output: [[2,2,2,2],[2,3,3],[3,5]]

Example 3:
Input: candidates = [2], target = 1
Output: []

Constraints:
1 <= candidates.length <= 30
2 <= candidates[i] <= 40
All elements of candidates are distinct.
1 <= target <= 40
```

## 解

```python3
def combinationSum(self, candidates: List[int], target: int) -> List[List[int]]:
    ans, sub = [], []
    candidates.sort()

    # 剪枝，从 start 开始计算，防止出现重复子集
    def backtracking(start: int, tar: int):
        ## 结束条件
        if tar == 0:
            ans.append(sub[:])
            return
        for i in range(start, len(candidates)):
            # 出现负数 剪枝
            if tar - candidates[i] < 0:
                break
            sub.append(candidates[i])
            backtracking(i, tar - candidates[i])
            sub.pop()

    backtracking(0, target)
    return ans
```