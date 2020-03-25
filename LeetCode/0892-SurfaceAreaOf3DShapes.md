
# 892. Surface Area of 3D Shapes


## 题目

```
Each value v = grid[i][j] represents a tower of v cubes placed on top of grid cell (i, j).
Return the total surface area of the resulting shapes.

Input: [[2]]
Output: 10

Input: [[1,2],[3,4]]
Output: 34

Note:
1 <= N <= 50
0 <= grid[i][j] <= 50
```

<br>

## 解法


### Python 

- 第一思路

```
def surfaceArea(self, grid: list) -> int:
	res, coincide = 0, 0
	for i in range(len(grid)):
		for j in range(len(grid[i])):
			if grid[i][j] == 0:
				continue
			res += 6 * grid[i][j] - 2 * (grid[i][j] - 1)
			if i == 0 and j == 0:
				continue
			elif i == 0 and j > 0:
				coincide += min(grid[i][j], grid[i][j-1]) * 2
			elif i > 0 and j == 0:
				coincide += min(grid[i][j], grid[i-1][j]) * 2
			else:
				coincide += min(grid[i][j], grid[i][j-1]) * 2
				coincide += min(grid[i][j], grid[i-1][j]) * 2
	return res - coincide
```