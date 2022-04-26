# 883. Projection Area of 3D Shapes

<br>

## 题目

```
You are given an n x n grid where we place some 1 x 1 x 1 cubes that are axis-aligned with the x, y, and z axes.
Each value v = grid[i][j] represents a tower of v cubes placed on top of the cell (i, j).
We view the projection of these cubes onto the xy, yz, and zx planes.
A projection is like a shadow, that maps our 3-dimensional figure to a 2-dimensional plane. We are viewing the "shadow" when looking at the cubes from the top, the front, and the side.
Return the total area of all three projections.

Constraints:
n == grid.length == grid[i].length
1 <= n <= 50
0 <= grid[i][j] <= 50
```

<br>


## 解法

```python3
def projectionArea_space(self, grid: List[List[int]]) -> int:
    xy, yz, zx = 0, 0, 0
    for i in range(len(grid)):
        row_max, column_max = 0, 0
        for j in range(len(grid[i])):
            if grid[i][j] != 0:
                xy += 1
            # n*n 的矩阵 [i][j] 和 [j][i] 特殊性
            row_max = max(row_max, grid[i][j])
            column_max = max(column_max, grid[j][i])
        yz += row_max
        zx += column_max
    return xy + yz + zx
```


<br>