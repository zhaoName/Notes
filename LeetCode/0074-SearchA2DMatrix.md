# 74. Search a 2D Matrix


<br>

## 题目

```
Write an efficient algorithm that searches for a value target in an m x n integer matrix matrix. This matrix has the following properties:
Integers in each row are sorted from left to right.
The first integer of each row is greater than the last integer of the previous row.

Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13
Output: false

Constraints:
m == matrix.length
n == matrix[i].length
1 <= m, n <= 100
-104 <= matrix[i][j], target <= 104
```

<br>

## 解法

```python3
def binarySearch(self, nums: List[int], target: int) -> bool:
    left, right = 0, len(nums) - 1
    while left <= right:
        middle = (right + left) // 2
        print(left, right, middle)
        if nums[middle] == target:
            return True
        elif nums[middle] < target:
            left = middle + 1
        else:
            right = middle - 1
    return False

def searchMatrix(self, matrix: List[List[int]], target: int) -> bool:
    if not matrix: return False

    m = len(matrix)
    n = len(matrix[0])
    for i in range(m):
        if matrix[i][0] == target or matrix[i][n - 1] == target:
            return True
        elif matrix[i][0] < target and matrix[i][n - 1] > target:
            return self.binarySearch(matrix[i], target)
    return False
```


```python3
## 转化成一位数组也是升序，再用二分查找
def searchMatrix(self, matrix: List[List[int]], target: int) -> bool:
    m = len(matrix)
    n = len(matrix[0])

    left, right = 0, m * n -1
    while left <= right:
        middle = (right - left) // 2 + left
        cur = matrix[middle // n][middle % n]
        if cur == target:
            return True
        elif cur < target:
            left = middle + 1
        else:
            right = middle - 1
    return False
```

<br>