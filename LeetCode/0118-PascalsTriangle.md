# 118. Pascal's Triangle

## 题目

```
Given an integer numRows, return the first numRows of Pascal's triangle.
In Pascal's triangle, each number is the sum of the two numbers directly above it as shown:

Example 1:
Input: numRows = 5
Output: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]

Constraints:
1 <= numRows <= 30
```

<br>

## 解法

```python3
from typing import List
class Solution:
    def generate(self, numRows: int) -> List[List[int]]:
        ans = [[1] * i for i in range(1,numRows + 1)]
        print(ans)
        for i in range(1, numRows):
            for j  in range(1, i):
                ans[i][j] = ans[i-1][j] + ans[i-1][j-1]
        return ans
```