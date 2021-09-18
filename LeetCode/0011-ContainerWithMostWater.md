# 11. Container With Most Water

## 题目

```
Given n non-negative integers a1, a2, ..., an , where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of the line i is at (i, ai) and (i, 0).
Find two lines, which, together with the x-axis forms a container, such that the container contains the most water.
Notice that you may not slant the container.

Input: height = [1,8,6,2,5,4,8,3,7]
Output: 49
Explanation: The above vertical lines are represented by array [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the container can contain is 49.

Constraints:
n == height.length
2 <= n <= 105
0 <= height[i] <= 104
```

<br>

## 解法 


```python3
def maxArea(self, height: List[int]) -> int:
    left, right, ans = 0, len(height) - 1, 0
    while left < right:
        if height[left] <= height[right]:
            ans = max(height[left] * (right - left), ans)
            left += 1
        else:
            ans = max(height[right] * (right - left), ans)
            right -= 1
    return ans
```