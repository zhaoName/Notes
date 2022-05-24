# 462. Minimum Moves to Equal Array Elements II


<br>

## 题目

```
Given an integer array nums of size n, return the minimum number of moves required to make all array elements equal.
In one move, you can increment or decrement an element of the array by 1.
Test cases are designed so that the answer will fit in a 32-bit integer.

Input: nums = [1,2,3]
Output: 2
Explanation:
Only two moves are needed (remember each move increments or decrements one element):
[1,2,3]  =>  [2,2,3]  =>  [2,2,2]

Constraints:
n == nums.length
1 <= nums.length <= 10^5
-10^9 <= nums[i] <= 10^9
```

<br>

## 解法

```python3
def minMoves2(self, nums: List[int]) -> int:
    ans = 0
    nums.sort()
    target = nums[len(nums) // 2]
    for j in range(len(nums)):
        ans += abs(nums[j] - target)
    return ans
```

```python3
# 借助快速选择排序(TopK问题)，查找第 K 大的数
def partition(self, nums: List[int], left: int, right: int) -> int:
    pivot = nums[left]
    i, j = left, right
    while i < j:
        while i < j and nums[j] >= pivot:
            j -= 1
        nums[i], nums[j] = nums[j], nums[i]

        while i < j and nums[i] <= pivot:
            i += 1
        nums[i], nums[j] = nums[j], nums[i]
    nums[i] = pivot
    return i

def k_split(self, nums: List[int], k: int, left: int, right: int):
    index = self.partition(nums, left, right)
    if index == k:
        return
    elif index > k:
        self.k_split(nums, k, left, index-1)
    else:
        self.k_split(nums, k, index+1, right)

def quickSelect(self, nums: List[int], k: int) -> int:
    self.k_split(nums, k, 0, len(nums)-1)
    return nums[k]

# time:O(n) space:O(logn)
def minMoves2(self, nums: List[int]) -> int:
    # 查找第 K 大的数
    target = self.quickSelect(nums, len(nums) // 2)
    return sum(abs(num - target) for num in nums)
```

<br>
