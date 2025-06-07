
# 59.Spiral Matrix II


## 题目

```
Given a positive integer n, generate an n x n matrix filled with elements from 1 to n^2 in spiral order.

Input: n = 3
Output: [[1,2,3],[8,9,4],[7,6,5]]

Input: n = 1
Output: [[1]]


Constraints:
1 <= n <= 20
```

## 解法

```python3
# 区间划分
def generateMatrix(self, n: int) -> List[List[int]]:
    ans = [[0] * n for _ in range(n)]

    count = 1
    for x in range(n // 2):
        startI, endJ = x, n - x - 1
        # [0, n-1)
        for j in range(startI, endJ):
            ans[startI][j] = count
            count += 1
        # [0, n-1)
        for i in range(startI, endJ):
            ans[i][endJ] = count
            count += 1
        i += 1
        # [n-1, 0)
        for j in range(endJ, startI, -1):
            ans[i][j] = count
            count += 1
        j -= 1
        # [n-1, 0)
        for i in range(endJ, startI, -1):
            ans[i][j] = count
            count += 1

    if n % 2 != 0:
        index = n // 2
        ans[index][index] = count
    return ans
```

```python3
def generateMatrix(self, n: int) -> List[List[int]]:
    ans = [[0] * n for _ in range(n)]

    l, r, t, b = 0, n - 1, 0, n - 1
    count, maxCount = 1, n * n
    while count <= maxCount:
        for j in range(l, r + 1):
            ans[t][j] = count
            count += 1
        t += 1
        for i in range(t, b + 1):
            ans[i][r] = count
            count += 1
        r -= 1
        for j in range(r, l - 1, -1):
            ans[b][j] = count
            count += 1
        b -= 1
        for i in range(b, t - 1, -1):
            ans[i][l] = count
            count += 1
        l += 1
    return ans
```