
# 447. Number of Boomerangs

## 题目

```
You are given n points in the plane that are all distinct, where points[i] = [xi, yi]. A boomerang is a tuple of points (i, j, k)
such that the distance between i and j equals the distance between i and k (the order of the tuple matters).
Return the number of boomerangs.

Input: points = [[0,0],[1,0],[2,0]]
Output: 2
Explanation: The two boomerangs are [[1,0],[0,0],[2,0]] and [[1,0],[2,0],[0,0]].

Constraints:
n == points.length
1 <= n <= 500
points[i].length == 2
-104 <= xi, yi <= 104
All the points are unique.
```

<br>

## 解法

```python3
from typing import List
import collections

class Solution:
    def numberOfBoomerangs(self, points: List[List[int]]) -> int:
        ans = 0
        for p in points:
            cnt = collections.defaultdict(int)
            for q in points:
                ## 算出距离相等的两个点的个数
                dis = (p[0] - q[0]) * (p[0] - q[0]) + (p[1] - q[1]) * (p[1] - q[1])
                cnt[dis] += 1
            # 排列数问题：从m个距离相等的点中任意选两个点 m*(m-1)
            for m in cnt.values():
                ans += m * (m - 1)
        return ans
```