# 289. Game of Life


## 题目

```
ccording to the Wikipedia's article: "The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970."
Given a board with m by n cells, each cell has an initial state live (1) or dead (0). 
Each cell interacts with its eight neighbors (horizontal, vertical, diagonal) using the following four rules (taken from the above Wikipedia article):

- Any live cell with fewer than two live neighbors dies, as if caused by under-population.
- Any live cell with two or three live neighbors lives on to the next generation.
- Any live cell with more than three live neighbors dies, as if by over-population..
- Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

Write a function to compute the next state (after one update) of the board given its current state. 
The next state is created by applying the above rules simultaneously to every cell in the current state, where births and deaths occur simultaneously.

Input: 
[ [0,1,0],
  [0,0,1],
  [1,1,1],
  [0,0,0] ]
Output: 
[ [0,0,0],
  [1,0,1],
  [0,1,1],
  [0,1,0]]

Follow up:
- Could you solve it in-place? Remember that the board needs to be updated at the same time: You cannot update some cells first and then use their updated values to update other cells.
- In this question, we represent the board using a 2D array. In principle, the board is infinite, which would cause problems when the active area encroaches the border of the array. How would you address these problems?
```

<br>

## 解法

### Python 

- 借鉴思路

```
def gameOfLife(self, board: list) -> None:
    """
    Do not return anything, modify board in-place instead.
    """
    if not board: return
    m, n = len(board), len(board[0])
    board_copy = [[board[i][j] for j in range(n)] for i in range(m)]
    row, col = [-1, 0, 1], [-1, 0, 1]
    for i in range(m):
    	for j in range(n):
    		cnt = 0
    		for x in range(3):
    			for y in range(3):
    				a = i + row[x]
    				b = j + col[y]
    				if (a == i and b == j) or a < 0 or a >= m or b < 0 or b >= n:
    					continue
    				if board_copy[a][b] == 1:
    					cnt += 1
    		if board_copy[i][j] == 0 and cnt == 3:
    			board[i][j] = 1
    		if board_copy[i][j] == 1 and (cnt < 2 or cnt > 3):
    			board[i][j] = 0
    print(board)
```


- 借鉴思路

```
def gameOfLife(self, board: List[List[int]]) -> None:
    """
    Do not return anything, modify board in-place instead.
    """
    if not board: return
    m, n = len(board), len(board[0])
    row, col = [-1, 0, 1, -1, 1, -1, 0, 1], [-1, -1, -1, 0, 0, 1, 1, 1]
    for i in range(m):
        for j in range(n):
            cnt = 0
            for x in range(8):
                a = i + row[x]
                b = j + col[x]
                if a < 0 or a >= m or b < 0 or b >= n:
                    continue
                # 这里不能直接加board[x][y]，因为 board[x][y] 的倒数第二位是可能有值的
                cnt += board[a][b] & 1
            
            if board[i][j] & 1 > 0:
                if cnt == 2 or cnt == 3:
                    board[i][j] = int(0b11)
            elif cnt == 3:
                board[i][j] = int(0b10)

    for i in range(m):
        for j in range(n):
            board[i][j] >>= 1
```







