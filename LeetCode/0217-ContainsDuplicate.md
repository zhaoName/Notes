
# 217. Contains Duplicate

## 题目

```
Given an array of integers, find if the array contains any duplicates.

Your function should return true if any value appears at least twice in the array, and it should return false if every element is distinct.

Example 1:
Input: [1,2,3,1]
Output: true

Example 2:
Input: [1,2,3,4]
Output: false
```


<br>

## 解法

### Python

- 第一思路 hash

```
## Time Complexity:O(n)  Space Complexity:O(n)
def containsDuplicate_hash(self, nums) -> bool:
	hash = {}
	for a in nums:
		if not hash.get(a) and hash.get(a) != 0:
			hash[a] = a
		else:
			return True
	return False
```

- 借鉴思路 set

```
def containsDuplicate_set(self, nums) -> bool:
	return len(nums) != len(set(nums))
```





