# 417. Pacific Atlantic Water Flow

<br>

## 题目

```
There is an m x n rectangular island that borders both the Pacific Ocean and Atlantic Ocean.
The Pacific Ocean touches the island's left and top edges, and the Atlantic Ocean touches the island's right and bottom edges.
The island is partitioned into a grid of square cells. You are given an m x n integer matrix heights where heights[r][c] represents the height above sea level of the cell at coordinate (r, c).
The island receives a lot of rain, and the rain water can flow to neighboring cells directly north, south, east, and west if the neighboring cell's height is less than or equal to the current cell's height. Water can flow from any cell adjacent to an ocean into the ocean.
Return a 2D list of grid coordinates result where result[i] = [ri, ci] denotes that rain water can flow from cell (ri, ci) to both the Pacific and Atlantic oceans.

Input: heights = [[1,2,2,3,5],[3,2,3,4,4],[2,4,5,3,1],[6,7,1,4,5],[5,1,1,2,4]]
Output: [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]

Constraints:
m == heights.length
n == heights[r].length
1 <= m, n <= 200
0 <= heights[r][c] <= 105
```

<br>


## 解法

```python3
class Solution:
    def __init__(self):
        self.result_all = None
        # 分别表示上右下左
        self.directs = [(-1, 0), (0, 1), (1, 0), (0, -1)]
        self.m = 0
        self.n = 0
        # 表示能流到太平洋
        self.po = None
        # 表示能流到大西洋
        self.ao = None
        self.visited = None

    def pacificAtlantic(self, matrix):
        # 初始化一些东西
        self.result_all = []
        self.m = len(matrix)
        if self.m == 0:
            return self.result_all
        self.n = len(matrix[0])
        self.ao = [[0] * self.n for _ in range(self.m)]
        self.po = [[0] * self.n for _ in range(self.m)]
        self.visited = [[0] * self.n for _ in range(self.m)]

        # 本题顺着流不太好做，我们用逆流的方式来思考
        # 从上面的太平洋逆流
        for i in range(0, 1):
            for j in range(self.n):
                self.dfs(matrix, i, j, True)
        # 从左边的太平洋逆流
        self.visited = [[0] * self.n for _ in range(self.m)]
        for i in range(self.m):
            for j in range(0, 1):
                self.dfs(matrix, i, j, True)
        # 下面的大西洋
        self.visited = [[0] * self.n for _ in range(self.m)]
        for i in range(self.m - 1, self.m):
            for j in range(self.n):
                self.dfs(matrix, i, j, False)
        # 右边的大西洋
        self.visited = [[0] * self.n for _ in range(self.m)]
        for i in range(self.m):
            for j in range(self.n - 1, self.n):
                self.dfs(matrix, i, j, False)

        for i in range(self.m):
            for j in range(self.n):
                if self.po[i][j] == 1 and self.ao[i][j] == 1:
                    self.result_all.append((i, j))
        return self.result_all

    def dfs(self, matrix, x, y, flag):
        if self.visited[x][y] == 1:
            return
        self.visited[x][y] = 1
        if flag:
            # 表示是太平洋
            self.po[x][y] = 1
        else:
            # 表示是大西洋
            self.ao[x][y] = 1

        for i in range(4):
            new_x = x + self.directs[i][0]
            new_y = y + self.directs[i][1]
            if not self.in_area(new_x, new_y):
                continue
            if matrix[x][y] > matrix[new_x][new_y]:
                continue
            self.dfs(matrix, new_x, new_y, flag)
        return

    def in_area(self, x, y):
        return 0 <= x < self.m and 0 <= y < self.n
```


<br>