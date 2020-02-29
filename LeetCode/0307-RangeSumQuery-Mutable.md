# 307. Range Sum Query - Mutable


## 题目

```
Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.
The update(i, val) function modifies nums by updating the element at index i to val.

Example:
Given nums = [1, 3, 5]
sumRange(0, 2) -> 9
update(1, 2)
sumRange(0, 2) -> 8

Note:
The array is only modifiable by the update function.
You may assume the number of calls to update and sumRange function is distributed evenly.
```

<br>

## 解法


### Python


- 第一思路  延续 题目303 

```
class NumArray:

    def __init__(self, nums):
    	self.nums = nums
    	self.sums = [0] * (len(nums) + 1)
    	for i in range(len(nums)):
    	 	self.sums[i + 1] = self.sums[i] + nums[i]
        

    def update(self, i: int, val: int) -> None:
    	div = val - self.nums[i]
    	self.nums[i] = val
    	for j in range(i+1, len(self.sums)):
    		self.sums[j] += div


    def sumRange(self, i: int, j: int) -> int:
    	return self.sums[j+1] - self.sums[i]
```


- 借鉴思路  线段树


```
class NumArray:

    def __init__(self, nums):
    	self.nums = nums
    	## 构建线段树  将 nums 放到 tree 的后面
    	## Time Complexity: O(n)   Space Complexity:O(n)
    	self.tree = nums * 2
    	## 构建非叶子节点 
    	for i in range(len(nums)-1, 0, -1):
    		self.tree[i] = self.tree[i * 2] + self.tree[i*2 + 1]


    ## Time Complexity: O(logn)   Space Complexity:O(1)
    def update_tree(self, i: int, val: int) -> None:
    	i += len(self.nums)
    	self.tree[i] = val

    	while i > 0:
    		left = i
    		right = i
    		if i % 2 == 0:
    			right = i + 1
    		else:
    			left = i - 1
    		self.tree[i // 2] = self.tree[left] + self.tree[right]
    		i //= 2


    ## Time Complexity: O(logn)   Space Complexity:O(1)
    def sumRange_tree(self, i: int, j: int) -> int:
    	i += len(self.nums)
    	j += len(self.nums)
    	sum = 0
    	while i <= j:
    		## i 不是2的倍数 需要单独加上此数，
    		## 因为 i 和 i-1 相加够成的节点不在此次计算范围内，只能单独计算i
    		if i % 2 == 1:
    			sum += self.tree[i]
    			i += 1
    		if j % 2 == 0:
    			sum += self.tree[j]
    			j -= 1
    		i //= 2
    		j //= 2
    	return sum
```