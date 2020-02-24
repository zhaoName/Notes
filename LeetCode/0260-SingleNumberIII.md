
# 260. Single Number III

## 题目

```
Given an array of numbers nums, in which exactly two elements appear only once and all the other elements appear exactly twice. 
Find the two elements that appear only once.

Note:
1. The order of the result is not important. So in the above example, [5, 3] is also correct.
2. Your algorithm should run in linear runtime complexity. 
   Could you implement it using only constant space complexity?
```

<br>

## 解法

### Python

- 借鉴思路 掩码

```
## 两个掩码
## bitmask ^= x 得到的结果是出现奇数次的位，如
##  0101
## ^0100
## ------
##  0001
##
## x & (-x) 是保留位中最右边 1 ，且将其余的 1 设位 0 的方法.(-x = ~x + 1)
## 所以 bitmask & (-bitmask) 之后得到的最右边 1要么来自 x 要么来自 y
def singleNumber_bitmask(self, nums: List[int]) -> List[int]:
	bitmask = 0
	for num in nums:
		bitmask ^= num

	rightBit = bitmask & (-bitmask)
	x = 0
	for num in nums:
		if num & rightBit:
			x ^= num
	return [x, bitmask ^ x]
```

- 借鉴思路 最高非零位

```
## 此题解法取巧 适用于 python
def singleNumber_(self, nums: List[int]) -> List[int]:
	xor = 0
	for num in nums:
		xor ^= num

	## 最高非0位  bin(xor) = 0b1*****
	rightBit = len(bin(xor)) - 3
	x, y = 0, 0
	for num in nums:
		if num & rightBit:
			x ^= num
		else:
			y ^= num
	return [x, y]
```

