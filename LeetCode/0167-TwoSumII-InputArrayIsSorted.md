# 167. Two Sum II - Input array is sorted


## 题目

```
Given an array of integers numbers that is already sorted in non-decreasing order, find two numbers such that they add up to a specific target number.
Return the indices of the two numbers (1-indexed) as an integer array answer of size 2, where 1 <= answer[0] < answer[1] <= numbers.length.
The tests are generated such that there is exactly one solution. You may not use the same element twice.

Example 1:
Input: numbers = [2,7,11,15], target = 9
Output: [1,2]
Explanation: The sum of 2 and 7 is 9. Therefore index1 = 1, index2 = 2.

Constraints:
2 <= numbers.length <= 3 * 104
-1000 <= numbers[i] <= 1000
numbers is sorted in non-decreasing order.
-1000 <= target <= 1000
The tests are generated such that there is exactly one solution.
```

<br>

## 解法

```python3
from typing import List

class Solution:
    def twoSum1(self, numbers: List[int], target: int) -> List[int]:
    	dict = {}
    	for i in range(len(numbers)):
    		if numbers[i] in dict:
    			return [dict[numbers[i]], i + 1]
    		else:
    			dict[target - numbers[i]] = i+1
    	return None
```