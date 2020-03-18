# 836. Rectangle Overlap


## 题目

```
A rectangle is represented as a list [x1, y1, x2, y2], where (x1, y1) are the coordinates of 
its bottom-left corner, and (x2, y2) are the coordinates of its top-right corner.

Two rectangles overlap if the area of their intersection is positive.  
To be clear, two rectangles that only touch at the corner or edges do not overlap.
Given two (axis-aligned) rectangles, return whether they overlap.

Input: rec1 = [0,0,2,2], rec2 = [1,1,3,3]
Output: true

Notes:
Both rectangles rec1 and rec2 are lists of 4 integers.
All coordinates in rectangles will be between -10^9 and 10^9.
```

<br>

## 解法

### Python

- 上下左右

```
def isRectangleOverlap_fan(self, rec1: list, rec2: list) -> bool:
	if not (rec2[0] >= rec1[2] or rec2[2] <= rec1[0] or rec2[1] >= rec1[3] or rec2[3] <= rec1[1]):
		return True
	return False
```

- 区域

```
def isRectangleOverlap_region(self, rec1: list, rec2: list) -> bool:
	return (min(rec2[2], rec1[2]) > max(rec1[0], rec2[0])) and (min(rec2[3], rec1[3]) > max(rec1[1], rec2[1]))
```