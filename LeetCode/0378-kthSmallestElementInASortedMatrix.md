# 378.kth Smallest Element in a Sorted Matrix.py

## 题目

```
Given an n x n matrix where each of the rows and columns is sorted in ascending order, return the kth smallest element in the matrix.
Note that it is the kth smallest element in the sorted order, not the kth distinct element.
You must find a solution with a memory complexity better than O(n^2).

Example 1:
Input: matrix = [[1,5,9],[10,11,13],[12,13,15]], k = 8
Output: 13
Explanation: The elements in the matrix are [1,5,9,10,11,12,13,13,15], and the 8th smallest number is 13

Example 2:
Input: matrix = [[-5]], k = 1
Output: -5

Constraints:
n == matrix.length == matrix[i].length
1 <= n <= 300
-10^9 <= matrix[i][j] <= 10^9
All the rows and columns of matrix are guaranteed to be sorted in non-decreasing order.
1 <= k <= n^2

Follow up:
Could you solve the problem with a constant memory (i.e., O(1) memory complexity)?
Could you solve the problem in O(n) time complexity?
The solution may be too advanced for an interview but you may find reading this paper fun.
```

## 解法

```
from typing import List
import heapq

class Solution:
    # time complexity:O(nlogn), Space complexity: O(n^2)
    def kthSmallest_1(self, matrix: List[List[int]], k: int) -> int:
        nums = sum(matrix, [])
        heapq.heapify(nums)
        while k > 1:
            heapq.heappop(nums)
            k -= 1
        return heapq.heappop(nums)

    # time complexity:O(klogn), Space complexity: O(n)
    def kthSmallest_2(self, matrix: List[List[int]], k: int) -> int:
        n = len(matrix)
        nums = [(matrix[i][0], i, 0) for i in range(n)]

        heapq.heapify(nums)
        while k > 1:
            k -= 1
            _, i, j = heapq.heappop(nums)
            if j < n - 1:
                heapq.heappush(nums, (matrix[i][j+1], i, j + 1))
        return heapq.heappop(nums)[0]

    # time complexity:O(n), Space complexity: O(1)
    def kthSmallest(self, matrix: List[List[int]], k: int) -> int:
        n = len(matrix)

        def check(mid: int) -> bool:
            num, i, j = 0, n - 1, 0
            while i >= 0 and j < n:
                if matrix[i][j] > mid:
                    i -= 1
                else:
                    # 记录不大于的个数
                    num += i + 1
                    j += 1
            return num >= k

        left, right = matrix[0][0], matrix[-1][-1]
        while left < right:
            mid = (left + right) // 2
            if check(mid):
                right = mid
            else:
                left = mid + 1
        return left
```