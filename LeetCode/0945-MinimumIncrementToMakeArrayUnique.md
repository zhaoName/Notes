# 945. Minimum Increment to Make Array Unique


## 题目

```
Given an array of integers A, a move consists of choosing any A[i], and incrementing it by 1.
Return the least number of moves to make every value in A unique.

Input: [3,2,1,2,1,7]
Output: 6
Explanation:  After 6 moves, the array could be [3, 4, 1, 2, 5, 7].
It can be shown with 5 or less moves that it is impossible for the array to have all unique values.

Note:
0 <= A.length <= 40000
0 <= A[i] < 40000
```

<br>

## 解法


### Python

- 第一思路 排序后求解

```
## Time:O(nlogn)   Space:O(n)
def minIncrementForUnique(self, A: list) -> int:
	if not A: return 0
	ans, i, miss, s = 0, 0, 0, set(A)
	A.sort()
	while i < len(A) - 1:
		j = i + 1
		if A[j] == A[i]:
			if ans == 0:
				miss = A[i] + 1
			else:
				miss += 1
			while miss in s or miss < A[i]:
				miss += 1
			ans += miss - A[i]
			#print(A[i], miss, ans)
		i = j
	return ans
```


- 借鉴思路  虽然想到计数法 但受第一思路影响 需要优化

```
## Time:O(n)   Space:O(40001)
def minIncrementForUnique_(self, A: list) -> int:
	if len(A) < 2: return 0
	largest, nums, res = -1, [0] * 40001, 0
	for a in A:
		nums[a] += 1
		largest = max(largest, a)

	for i in range(largest):
		if nums[i] > 1:
			res += nums[i] - 1
			nums[i + 1] += nums[i] - 1
	## 设 max+1 = x，那么后面的d个数就是[x+1,x+2,x+3,...,x+d],
    ## 因此操作次数是[1,2,3,...,d],用求和公式求和。
	if nums[largest] > 1:
    	res += nums[largest] * (nums[largest] - 1) // 2
	return res
```