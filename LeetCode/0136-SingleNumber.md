
# 136. Single Number


## 题目

```
Given a non-empty array of integers, every element appears twice except for one. Find that single one.

Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
```

<br>

## 解法


### Python

- 第一思路

```
# 时间复杂度 O(n)  空间复杂度 O(1)
# 最坏的时间复杂度肯定远远小于O(n^2)
# 提交到 leetcode 超时,但不明白 官方解答O(n^2)的时间复杂度为什么都可以
def singleNumber(self, nums)->int:
	if len(nums) == 1: return nums[0]
	a = 0
	i = 1
	while i < len(nums):
		if nums[a] == nums[i]:
			if a + 1 == i:
				a = i + 1
				i = i + 2
			else:
				nums[a+1], nums[i] = nums[i], nums[a+1]
				a += 2
				i = a + 1
		else:
			i += 1
	return nums[a]
```

- 字典

```
# 时间复杂度 O(n)  空间复杂度 O(n)
def singleNumber_hashTable(self, nums)->int:
	hash_tabel = {}
	for i in nums:
		try:
			hash_tabel.pop(i)
		except:
			hash_tabel[i] = 1
	return hash_tabel.popitem()[0]
```

- 数学方法

```
# 时间复杂度 O(n)  空间复杂度 O(n)
# 2*(a+b+c) - (a+a+b+b+c) = c
def singleNumber_math(self, nums)->int:
	return 2 * sum(set(nums)) - sum(nums)
```

- XOR

```
# 时间复杂度 O(n)  空间复杂度 O(1)
# a^a = 0  a^0 = a
# a ^ a ^ b = (a^a)^b = b
def singleNumber_xor(self, nums)->int:
	result = 0
	for i in nums:
		result ^= i
	return result
```