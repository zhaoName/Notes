
# 303. Range Sum Query - Immutable


## 题目

```
Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.

Example:
Given nums = [-2, 0, 3, -5, 2, -1]
sumRange(0, 2) -> 1
sumRange(2, 5) -> -1
sumRange(0, 5) -> -3

Note:
You may assume that the array does not change.
There are many calls to sumRange function.
```

注意`sumRange `可能会调用很多次，这就意味着需要缓存来降低运算时间。否则这题出的就没任何意义。

## 解法

### Python


- 第一思路 没看清题目,直接切片求和

```
def __init__(self, nums: List[int]):
    self.nums = nums
    
def sumRange(self, i: int, j: int) -> int:
    	return sum(self.nums[i:j+1])
```



- 借鉴思路 缓存求解

```
def __init__(self, nums: List[int]):
    self.nums = nums


## sum[i:j] = sum[0:j] - sum[0:i]
def sumRange_(self, i: int, j: int) -> int:
	return self.sums[j+1] - self.sums[i]
```

