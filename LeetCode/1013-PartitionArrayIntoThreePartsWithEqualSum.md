
# 1013. Partition Array Into Three Parts With Equal Sum


## 题目 

```
Given an array A of integers, return true if and only if we can partition the array into three non-empty parts with equal sums.
Formally, we can partition the array if we can find indexes i+1 < j 
with (A[0] + A[1] + ... + A[i] == A[i+1] + A[i+2] + ... + A[j-1] == A[j] + A[j-1] + ... + A[A.length - 1])

Input: A = [0,2,1,-6,6,-7,9,1,2,0,1]
Output: true
Explanation: 0 + 2 + 1 = -6 + 6 - 7 + 9 + 1 = 2 + 0 + 1
```

<br>

## 解法 

### Python

- 第一思路 超时

```
## 超时
def canThreePartsEqualSum(self, A: list) -> bool:
	if len(A) < 3: return False
	i, j, sumA, sumB = 0, len(A) - 1, 0, 0
	dpA, dpB = {}, {}
	while i < len(A) - 2 and j >= 2:
		sumA += A[i]
		sumB += A[j]
		if sumA in dpB:
			if i + 1 < dpB[sumA] and sumA == sum(A[i+1:dpB[sumA]]):
				return True
		dpA[sumA] = i
		i += 1
		if sumB in dpA:
			if dpA[sumB] + 1 < j and sumB == sum(A[dpA[sumB]+1:j]):
				return True
		dpB[sumB] = j
		j -= 1
	return False
```

- 借鉴思路  双指针

```
## 双指针
def canThreePartsEqualSum_twoPointer(self, A: list) -> bool:
	s = sum(A)
	if s % 3 != 0: return False
	i, j, target = 0, len(A) - 1, s // 3

	sumA, sumB = A[i], A[j]
	while i + 1 < j:
		if sumA == target and sumB == target:
			return True
		if sumA != target:
			i += 1
			sumA += A[i]
		if sumB != target:
			j -= 1
			sumB += A[j]
	return False
```

- 借鉴思路  遍历

```
## 遍历查找
def canThreePartsEqualSum_sum(self, A: list) -> bool:
	s = sum(A)
	if s % 3 != 0: return False

	i, sumleft, target = 0, 0, s // 3
	while i < len(A) - 2:
		sumleft += A[i]
		i += 1
		if sumleft == target:
			break
	if sumleft != target:
		return False
	j, sumleft = i, 0
	while j < len(A) - 1:
		sumleft += A[j]
		j += 1
		if target == sumleft:
			return True
	return False
```