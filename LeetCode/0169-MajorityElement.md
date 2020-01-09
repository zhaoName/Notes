# 169. Majority Element


## 题目

```
Given an array of size n, find the majority element. The majority element is the element that appears more than ⌊ n/2 ⌋ times.

You may assume that the array is non-empty and the majority element always exist in the array.
```


## 解法


### Python

```
# 时间复杂度 O(n)  空间复杂度 最坏 O(n/2) 最好O(1)
# if i in dict 的时间复杂度也是O(n) 若用其 则整体时间复杂度为O(n^2)
def majorityElement(self, nums) -> int:
	dict = {}
	for i in nums:
		#if i in dict:
		try:
			dict[i] += 1
		except:
			dict[i] = 1

	#dict = sorted(dict.items(), key=lambda x:x[1], reverse=True) 
	#return dict[0][0]
	max = 0; result = 0
	for key, value in dict.items():
		if (value > max):
			max = value
			result = key
	print(result)
	return result
```