
# 36. Valid Sudoku

## 题目

```
Determine if a 9 x 9 Sudoku board is valid. Only the filled cells need to be validated according to the following rules:

Each row must contain the digits 1-9 without repetition.
Each column must contain the digits 1-9 without repetition.
Each of the nine 3 x 3 sub-boxes of the grid must contain the digits 1-9 without repetition.

Note:
A Sudoku board (partially filled) could be valid but is not necessarily solvable.
Only the filled cells need to be validated according to the mentioned rules.

Input: board =
[["5","3",".",".","7",".",".",".","."]
,["6",".",".","1","9","5",".",".","."]
,[".","9","8",".",".",".",".","6","."]
,["8",".",".",".","6",".",".",".","3"]
,["4",".",".","8",".","3",".",".","1"]
,["7",".",".",".","2",".",".",".","6"]
,[".","6",".",".",".",".","2","8","."]
,[".",".",".","4","1","9",".",".","5"]
,[".",".",".",".","8",".",".","7","9"]]
Output: true
```

<br>

## 解法

```python3
def isValidSudoku(self, board: List[List[str]]) -> bool:
    rows = [{} for i in range(9)]
    columns = [{} for i in range(9)]
    boxes = [{} for i in range(9)]

    for row in range(9):
        for col in range(9):
            ch = board[row][col]
            if ch == '.': continue

            index = (row // 3) * 3 + col // 3
            print(row, col, index)
            boxes[index][ch] = boxes[index].get(ch, 0) + 1
            rows[row][ch] = rows[row].get(ch, 0) + 1
            columns[col][ch] = columns[col].get(ch, 0) + 1

            if rows[row][ch] > 1 or columns[col][ch] > 1 or boxes[index][ch] > 1:
                return False
    return True
```