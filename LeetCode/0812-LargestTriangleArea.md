# 812. Largest Triangle Area


<br>

## 题目

```
Given an array of points on the X-Y plane points where points[i] = [xi, yi], return the area of the largest triangle
that can be formed by any three different points. Answers within 10-5 of the actual answer will be accepted.

Input: points = [[0,0],[0,1],[1,0],[0,2],[2,0]]
Output: 2.00000
Explanation: The five points are shown in the above figure. The red triangle is the largest.

Constraints:
3 <= points.length <= 50
-50 <= xi, yi <= 50
All the given points are unique.
```

<br>

## 解法

```python3
from typing import List
from itertools import combinations

class Solution:
    def largestTriangleArea(self, points: List[List[int]]) -> float:
        def triangleArea(x1: int, y1: int, x2: int, y2: int, x3: int, y3: int) -> float:
            return abs(x1 * y2 + x2 * y3 + x3 * y1 - x1 * y3 - x2 * y1 - x3 * y2) / 2
        return max(triangleArea(x1, y1, x2, y2, x3, y3) for (x1, y1), (x2, y2), (x3, y3) in combinations(points, 3))
```

<br>