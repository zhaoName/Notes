# 977. Squares of a Sorted Array


## 题目

```
Given an integer array nums sorted in non-decreasing order, return an array of the squares of each number sorted in non-decreasing order.

Example 1:
Input: nums = [-4,-1,0,3,10]
Output: [0,1,9,16,100]
Explanation: After squaring, the array becomes [16,1,0,9,100].
After sorting, it becomes [0,1,9,16,100].

Constraints:
1 <= nums.length <= 104
-104 <= nums[i] <= 104
nums is sorted in non-decreasing order.

Follow up: Squaring each element and sorting the new array is very trivial, could you find an O(n) solution using a different approach?
```

<br>

## 解法

```python3
# Time:O(n*k) k为负数的个数
# Space: O(1)
def sortedSquares1(self, nums: List[int]) -> List[int]:
    	i, j = 0, len(nums) - 1

    	while j - 1 >= 0 and nums[0] < 0:
    		t = math.fabs(nums[0])
    		print(t, nums[j])
    		if t < nums[j]:
    			nums[j] = pow(nums[j], 2)
    		else:
    			nums.remove(nums[0])
    			nums.insert(j, pow(int(t), 2))
    		j -= 1
    	while i <= j:
    		nums[i] = pow(nums[i], 2)
    		i += 1
    	return nums
```


```
# Time: O(n) 
# Space: O(n)
def sortedSquares(self, nums: List[int]) -> List[int]:
    	i, j, pos = 0, len(nums) - 1, len(nums) - 1
    	ans = [0] * len(nums)
    	while i <= j:
    		if nums[i] * nums[i] < nums[j] * nums[j]:
    			ans[pos] = nums[j] * nums[j]
    			j -= 1
    		else:
    			ans[pos] = nums[i] * nums[i]
    			i += 1
    		pos -= 1
    	return ans
```

