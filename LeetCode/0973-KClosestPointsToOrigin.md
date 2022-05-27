# 973. K Closest Points to Origin


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
def kClosest_heap(self, points: List[List[int]], k: int) -> List[List[int]]:
    q = [(- x ** 2 - y ** 2, i) for (i, (x, y)) in enumerate(points[:k])]
    heapq.heapify(q)

    for i in range(k, len(points)):
        distance = points[i][0] ** 2 + points[i][1] ** 2
        heapq.heappushpop(q, (distance, i))

    return [points[j] for (_, j) in q]
```

```python
def partition(self, nums, left, right):
    pivot, i, j = nums[left], left, right
    while i < j:
        while i < j and nums[j][0] >= pivot[0]:
            j -= 1
        nums[i], nums[j] = nums[j], nums[i]
        while i < j and nums[i][0] <= pivot[0]:
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


def kClosest(self, points: List[List[int]], k: int) -> List[List[int]]:
    distances = []
    for i in range(len(points)):
        distances.append((points[i][0] ** 2 + points[i][1] ** 2, i))

    self.topK_Split(distances, 0, len(distances) - 1, k)
    return [points[distance[1]] for distance in distances[:k]]
```

<br>


