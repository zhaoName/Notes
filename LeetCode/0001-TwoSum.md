# 1.Two Sum

## 题目

```
Given an array of integers, return indices of the two numbers such that they add up to a specific target.
You may assume that each input would have exactly one solution, and you may not use the same element twice.

Example:
Given nums = [2, 7, 11, 15], target = 9,
Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
```

<br>

## 解法


### Python

```python3
def twoSum(self, nums: list, target: int) -> list:
	numsDict = {}
    for i in range(len(nums)):
        if nums[i] in numsDict:
            return [numsDict[nums[i]], i]
        numsDict[target - nums[i]] = i
    return None
```