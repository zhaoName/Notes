# 73. Set Matrix Zeroes

## 题目

```
Given an m x n integer matrix matrix, if an element is 0, set its entire row and column to 0's, and return the matrix.
You must do it in place.

Input: matrix = [[1,1,1],[1,0,1],[1,1,1]]
Output: [[1,0,1],[0,0,0],[1,0,1]]

Constraints:
m == matrix.length
n == matrix[0].length
1 <= m, n <= 200
-231 <= matrix[i][j] <= 231 - 1

Follow up:
A straightforward solution using O(mn) space is probably a bad idea.
A simple improvement uses O(m + n) space, but still not the best solution.
Could you devise a constant space solution?
```

<br>

## 解法

```python3
def setZeroes(self, matrix: List[List[int]]) -> None:
    m, n = len(matrix), len(matrix[0])
    rows, cols = [False] * m, [False] * n

    for i in range(m):
        for j in range(n):
            if matrix[i][j] == 0:
                rows[i] = cols[j] = True
    for i in range(m):
        for j in range(n):
            if rows[i] or cols[j]:
                matrix[i][j] = 0

## 两个标记变量，拿 matrix 第一行、第一列当上述的 rows 和 cols
def setZeroes_two(self, matrix: List[List[int]]) -> None:
    m, n = len(matrix), len(matrix[0])
    # 记录第一行、第一列中是否包含 0
    flag_row0 = any(matrix[0][j] == 0 for j in range(n))
    flag_col0 = any(matrix[i][0] == 0 for i in range(m))

    for i in range(1, m):
        for j in range(1, n):
            if matrix[i][j] == 0:
                matrix[0][j] = matrix[i][0] = 0
    for i in range(1, m):
        for j in range(1, n):
            if matrix[0][j] == 0 or matrix[i][0] == 0:
                matrix[i][j] = 0
    if flag_row0:
        for j in range(n):
            matrix[0][j] = 0
    if flag_col0:
        for i in range(m):
            matrix[i][0] = 0
```