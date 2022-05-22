# 912. Sort an Array

## 题目

```
Given an array of integers nums, sort the array in ascending order.

Input: nums = [5,1,1,2,0,0]
Output: [0,0,1,1,2,5] 

Constraints:
1 <= nums.length <= 50000
-50000 <= nums[i] <= 50000
```

<br>

## 解法

### Python

- 快排

```python3
def _sortArray_quick(self, nums, left, right) -> None:
	if left >= right: return
	key, i, j = nums[left], left, right
	while i < j:
		while i < j and nums[j] >= key:
			j -= 1
		nums[i] = nums[j]
		while i < j and nums[i] <= key:
			i += 1
		nums[j] = nums[i]
	nums[i] = key
	self._sortArray_quick(nums, left, i)
	self._sortArray_quick(nums, i+1, right)


def sortArray_quick(self, nums: list) -> list:
	if not nums: return list()
	self._sortArray_quick(nums, 0, len(nums)-1)
	return nums
```

- 堆排序

```python3
## 构建大顶堆
def heapify_max(self, nums:list, i, ln) -> None:
	largest = i
	l = 2 * i + 1
	r = 2 * i + 2
	if l < ln and nums[l] > nums[largest]:
		largest = l
	if r < ln and nums[r] > nums[largest]:
		largest = r

	if largest != i:
		nums[i], nums[largest] = nums[largest], nums[i]
		self.heapify_max(nums, largest, ln)


def heapify(self, nums:list, l) -> None:
	for i in range(l // 2 - 1, -1, -1):
		self.heapify_max(nums, i, l)


def sortArray_heap(self, nums: list) -> list:
	l = len(nums)
	self.heapify(nums, l)
	for i in range(len(nums)):
		nums[0], nums[l-1] = nums[l-1], nums[0]
		l -= 1
		self.heapify_max(nums, 0, l)
	print(nums)
	return nums```