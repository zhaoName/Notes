# 120. Triangle


## 题目

```
Given a triangle array, return the minimum path sum from top to bottom.
For each step, you may move to an adjacent number of the row below. More formally,
if you are on index i on the current row, you may move to either index i or index i + 1 on the next row.

Example 1:
Input: triangle = [[2],[3,4],[6,5,7],[4,1,8,3]]
Output: 11
Explanation: The triangle looks like:
   2
  3 4
 6 5 7
4 1 8 3
The minimum path sum from top to bottom is 2 + 3 + 5 + 1 = 11 (underlined above).

Constraints:
1 <= triangle.length <= 200
triangle[0].length == 1
triangle[i].length == triangle[i - 1].length + 1
-104 <= triangle[i][j] <= 104

Follow up: Could you do this using only O(n) extra space, where n is the total number of rows in the triangle?
```

<br>

## 解法

```python3
class Solution:
    def minimumTotal(self, triangle: List[List[int]]) -> int:
        n = len(triangle)
        if n == 1: return triangle[0][0]

        dp = [0] * len(triangle[n-1])
        dp[0] = triangle[0][0]
        for i in range(1, n):
            temp = dp.copy()
            for j in range(0, len(triangle[i])):
                if j-1 < 0:
                    ## 第一个元素
                    dp[j] = temp[j] + triangle[i][j]
                elif j == len(triangle[i]) - 1:
                    ## 最后一个元素
                    dp[j] = temp[j-1] + triangle[i][j]
                else:
                    ## 中间元素有两个 取最小值
                    dp[j] = min(temp[j-1], temp[j]) + triangle[i][j]
        return min(dp)
```


