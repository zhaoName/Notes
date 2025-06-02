# 119.Pascal's Triangle II

## 题目

```
给定一个非负索引 rowIndex，返回「杨辉三角」的第 rowIndex 行。
在「杨辉三角」中，每个数是它左上方和右上方的数的和。

示例 1:
输入: rowIndex = 3
输出: [1,3,3,1]

示例 2:
输入: rowIndex = 0
输出: [1]

示例 3:
输入: rowIndex = 1
输出: [1,1]

提示:
0 <= rowIndex <= 33

进阶：
你可以优化你的算法到 O(rowIndex) 空间复杂度吗？
```

## 解法

```python3
# 两个数组
def getRow(self, rowIndex: int) -> List[int]:
    n = rowIndex + 1
    ans = [1] * n
    for i in range(1, n):
        temp = ans.copy()
        for j in range(1, i):
            ans[j] = temp[j - 1] + temp[j]
    return ans
```


```python3
# 一个数组，从后往前计算
def getRow_02(self, rowIndex: int) -> List[int]:
    n = rowIndex + 1
    ans = [1] * n
    for i in range(1, n):
        for j in range(i - 1, 0, -1):
            ans[j] += ans[j - 1]
    return ans
```