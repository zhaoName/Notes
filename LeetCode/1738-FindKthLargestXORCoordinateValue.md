# 1738. Find Kth Largest XOR Coordinate Value


<br>

## 题目

```
Given an array of points where points[i] = [xi, yi] represents a point on the X-Y plane and an integer k, return the k closest points to the origin (0, 0).
The distance between two points on the X-Y plane is the Euclidean distance (i.e., √(x1 - x2)2 + (y1 - y2)2).
You may return the answer in any order. The answer is guaranteed to be unique (except for the order that it is in).

Input: points = [[1,3],[-2,2]], k = 1
Output: [[-2,2]]
Explanation:
The distance between (1, 3) and the origin is sqrt(10).
The distance between (-2, 2) and the origin is sqrt(8).
Since sqrt(8) < sqrt(10), (-2, 2) is closer to the origin.
We only want the closest k = 1 points from the origin, so the answer is just [[-2,2]].

Constraints:
1 <= k <= points.length <= 10^4
-10^4 < xi, yi < 10^4
```

<br>

## 解法

```python3
def kthLargestValue_heap(self, matrix: List[List[int]], k: int) -> int:
    m, n = len(matrix), len(matrix[0])
    values, temps = [], [0] * n

    for i in range(m):
        temp = 0
        for j in range(n):
            # 同行
            temp ^= matrix[i][j]
            # 同列
            temps[j] ^= temp
            values.append(temps[j])
    # 小顶堆
    heap = values[:k]
    heapq.heapify(heap)
    for i in range(k, len(values)):
        heapq.heappushpop(heap, values[i])
    return heap[0]
```

```python
def partition(self, nums, left, right):
    pivot, i, j = nums[left], left, right
    while i < j:
        while i < j and nums[j] >= pivot:
            j -= 1
        nums[i], nums[j] = nums[j], nums[i]
        while i < j and nums[i] <= pivot:
            i += 1
        nums[i], nums[j] = nums[j], nums[i]
    nums[i] = pivot
    return i

def topK_Split(self, nums, left, right, k):
    if left < right:
        index = self.partition(nums, left, right)
        if index == k:
            return
        elif index > k:
            self.topK_Split(nums, left, index - 1, k)
        else:
            self.topK_Split(nums, index + 1, right, k)


def kthLargestValue(self, matrix: List[List[int]], k: int) -> int:
    m, n = len(matrix), len(matrix[0])
    values, temps = [], [0] * n

    for i in range(m):
        temp = 0
        for j in range(n):
            temp ^= matrix[i][j]
            temps[j] ^= temp
            values.append(temps[j])

    self.topK_Split(values, 0, len(values)-1, len(values)-k)
    print(values)
    return values[len(values) - k]
```

<br>


