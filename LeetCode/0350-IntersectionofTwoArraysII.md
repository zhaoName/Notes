# 350. Intersection of Two Arrays II


## 题目

```
Given two arrays, write a function to compute their intersection.

Example 1:
Input: nums1 = [1,2,2,1], nums2 = [2,2]
Output: [2,2]

Example 2:
Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
Output: [4,9]

Note:
Each element in the result should appear as many times as it shows in both arrays.
The result can be in any order.

Follow up:
What if the given array is already sorted? How would you optimize your algorithm?
What if nums1's size is small compared to nums2's size? Which algorithm is better?
What if elements of nums2 are stored on disk, and the memory is limited such that you cannot load all elements into the memory at once?
```

<br>

## 解法


### Python

- 通用

```
## Time Complexity:O(n+m)   Space Complexity:O(len(nums1))
def intersect(self, nums1: list, nums2: list) -> list:
	d1, result = {}, []
	for i in range(len(nums1)):
		try:
			d1[nums1[i]] += 1
		except:
			d1[nums1[i]] = 1
	for a in nums2:
		if a in d1 and d1[a] > 0:
			result.append(a)
			d1[a] -= 1
	return result
```

- 两个列表已有序

```
## nums1 nums2 已排序
## Time Complexity:O(n) n 为两个列表长度较短
## Space Complexity:O(1)
def intersect_sorted(self, nums1: list, nums2: list) -> list:
	i, j = 0, 0
	while i<len(nums1) and j<len(nums2):
		if nums1[i] < nums2[j]:
			nums1.pop(i)
		elif nums1[i] > nums2[j]:
			j += 1
		else:
			i += 1
			j += 1
	return nums1
```