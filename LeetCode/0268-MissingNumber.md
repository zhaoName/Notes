
# 268. Missing Number


## 题目

```
Given an array containing n distinct numbers taken from 0, 1, 2, ..., n, find the one that is missing from the array.

Example 2:
Input: [9,6,4,2,3,5,7,0,1]
Output: 8
```

<br>

## 解法

### Python

- 第一思路

```
## 时间复杂度 O(n) 空间复杂度O(1)
def missingNumber_math(self, nums) -> int:
	return (len(nums) * (len(nums) + 1)) // 2 - sum(nums);
```

- XOR

```
## 时间复杂度 O(n) 空间复杂度O(1)
## 数组中[0, 1, 2 ..., missing, ... n] 对应下标 0, 1, 2 ... n-1 (其中len(nums) == n)
## missing = 值 ^ 下标 ^ len(nums)
##         =  0^1^...^n ^ 0^1...^(n-1) ^ len(nums)
##         = (0^0) ^ (1^1) ^ .. (n^n) ^ missing
##         = missing
def missingNumber_xor(self, nums) -> int:
	missing = len(nums)
	## i 是下标, num 是值
	for i, num in enumerate(nums):
		missing ^= i ^ num
	return missing
```