
# 189. Rotate Array

## 题目


```
Given an array, rotate the array to the right by k steps, where k is non-negative.

Note:
- Try to come up as many solutions as you can, there are at least 3 different ways to solve this problem.
- Could you do it in-place with O(1) extra space?
```

<br>


## 解法

### Python

```
## Time Complexity:O(n)  Space:O(1)
def rotate_insert(self, nums, k: int) -> None:
	if k == 0: return
	i = 0
	while i < (k % len(nums)):
		nums.insert(0, nums.pop())
		i += 1
```