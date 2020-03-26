# 999. Available Captures for Rook


## 题目

```
On an 8 x 8 chessboard, there is one white rook.  
There also may be empty squares, white bishops, and black pawns.  
These are given as characters 'R', '.', 'B', and 'p' respectively. 
Uppercase characters represent white pieces, and lowercase characters represent black pieces.

The rook moves as in the rules of Chess: it chooses one of four cardinal directions (north, east, west, and south), 
then moves in that direction until it chooses to stop, reaches the edge of the board, 
or captures an opposite colored pawn by moving to the same square it occupies.  
Also, rooks cannot move into the same square as other friendly bishops.
Return the number of pawns the rook can capture in one move.

Input: [[".",".",".",".",".",".",".","."],[".","p","p","p","p","p",".","."],[".","p","p","B","p","p",".","."],[".","p","B","R","B","p",".","."],
		[".","p","p","B","p","p",".","."],[".","p","p","p","p","p",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]
Output: 0
Explanation: 
Bishops are blocking the rook to capture any pawn

Note:
- board.length == board[i].length == 8
- board[i][j] is either 'R', '.', 'B', or 'p'
- There is exactly one cell with board[i][j] == 'R'
```

<br>

## 解法

### Python

- 第一思路 


```
def numRookCaptures(self, board: list) -> int:
    i, j, res = 0, -1, 0
    for i in range(8):
        x, y = 0, 7
        while x < y:
            if board[i][x] == 'R':
                j = x
                break
            if board[i][y] == 'R':
                j = y
                break
            x += 1
            y -= 1
        if j != -1:
            break
    a, b = i - 1, i + 1
    while True:
        if a >= 0 and board[a][j] == 'p':
            res += 1
            a = -1
        elif a >= 0 and board[a][j] == 'B':
            a = -1
        else:
            a -= 1

        if b < 8 and board[b][j] == 'p':
            res += 1
            b = 8
        elif b < 8 and board[b][j] == 'B':
            b = 8
        else:
            b += 1
        if a < 0 and b > 8:
            break
    a, b = j - 1, j + 1
    while True:
        if a >= 0 and board[i][a] == 'p':
            res += 1
            a = -1
        elif a >= 0 and board[i][a] == 'B':
            a = -1
        else:
            a -= 1
        if b < 8 and board[i][b] == 'p':
            res += 1
            b = 8
        elif b < 8 and board[i][b] == 'B':
            b = 8
        else:
            b += 1
        if a < 0 and b > 8:
            break
    return res
```


- 借鉴思路  方向数组

```
def numRookCaptures_ori(self, board: list) -> int:
    st, ed, res = 0, 0, 0
    dx, dy = [0, 1, 0, -1], [1, 0, -1, 0]
    for i in range(8):
        for j in range(8):
            if board[i][j] == "R":
                st, ed = i, j
    for a in range(4):
        step = 0
        while True:
            si = st + step * dx[a]
            sj = ed + step * dy[a]
            if si < 0 or si >= 8 or sj < 0 or sj >= 8 or board[si][sj] == 'B':
                break
            if board[si][sj] == 'p':
                res += 1
                break
            step += 1
    return res
```