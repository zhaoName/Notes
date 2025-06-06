
# 54.Spiral Matrix

## 题目

```
Given an m x n matrix, return all elements of the matrix in spiral order.

Input: matrix = [[1,2,3],[4,5,6],[7,8,9]]
Output: [1,2,3,6,9,8,7,4,5]

Input: matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
Output: [1,2,3,4,8,12,11,10,9,5,6,7]

Constraints:
m == matrix.length
n == matrix[i].length
1 <= m, n <= 10
-100 <= matrix[i][j] <= 100
```

## 解法

```python3
def spiralOrder(self, matrix: List[List[int]]) -> List[int]:
	m, n = len(matrix), len(matrix[0])
	ans, count, maxLen = [], 1, m * n

	l, r, t, b = 0, n - 1, 0, m - 1
	while True:
		for j in range(l, r + 1):
			ans.append(matrix[t][j])
		t += 1
		if t > b: break
		for i in range(t, b + 1):
			ans.append(matrix[i][r])
		r -= 1
		if l > r: break
		for j in range(r, l - 1, -1):
			ans.append(matrix[b][j])
		b -= 1
		if t > b: break
		for i in range(b, t - 1, -1):
			ans.append(matrix[i][l])
		l += 1
		if l > r: break
	return ans
```