
# 220. Contains Duplicate III

## 题目

```
Given an array of integers, find out whether there are two distinct indices i and j in the array 
such that the absolute difference between nums[i] and nums[j] is at most t and the absolute difference between i and j is at most k.

Example 2:
Input: nums = [1,0,1,1], k = 1, t = 2
Output: true

Example 3:
Input: nums = [1,5,9,1,5,9], k = 2, t = 3
Output: false
```


<br>

## 解法

### Python

- 借鉴思路 bucket

```
def containsNearbyAlmostDuplicate_(self, nums, k: int, t: int) -> bool:
	if k < 0 or t < 0: return False

	bucket = {}
	bucket_size = t + 1
	for i in range(len(nums)):
		bucket_num = nums[i] // (t + 1)
		if bucket_num in bucket: return True
		bucket[bucket_num] = nums[i]

		if (bucket_num - 1) in bucket and abs(bucket[bucket_num] - bucket[bucket_num-1]) <= t: return True
		if (bucket_num + 1) in bucket and abs(bucket[bucket_num] - bucket[bucket_num+1]) <= t: return True

		if i >= k:
			bucket.pop(nums[i-k] // (t+1)) 

	return False
```
