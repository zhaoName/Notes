
# 349. Intersection of Two Arrays


## 题目

```
Given two arrays, write a function to compute their intersection.

Example 1:
Input: nums1 = [1,2,2,1], nums2 = [2,2]
Output: [2]

Example 2:
Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
Output: [9,4]

Note:
Each element in the result must be unique.
The result can be in any order.
```


## 解法


### Python

- 第一思路

```
def intersection_dict(self, nums1: list, nums2: list) -> list:
	d1, result = {}, []
	for i in range(len(nums1)):
		d1[nums1[i]] = i
	for a in nums2:
		if a in d1:
			result.append(a)
	return set(result)
```


- 借鉴思路  原来 Python 中 & 还可以这么用

```
def intersection_set(self, nums1: list, nums2: list) -> list:
	return list(set(nums1) & set(nums2))
```