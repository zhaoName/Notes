# 108.Convert Sorted Array to Binary Search Tree

## 题目

```
Given an integer array nums where the elements are sorted in ascending order,
convert it to a height-balanced binary search tree.

Input: nums = [-10,-3,0,5,9]
Output: [0,-3,9,-10,null,5]
Explanation: [0,-10,5,null,-3,null,9] is also accepted:

Input: nums = [1,3]
Output: [3,1]
Explanation: [1,null,3] and [3,1] are both height-balanced BSTs.

Constraints:

1 <= nums.length <= 10^4
-10^4 <= nums[i] <= 10^4
nums is sorted in a strictly increasing order.
```

## 解

```python3
def sortedArrayToBST(self, nums: List[int]) -> TreeNode:
    if len(nums) == 1: return TreeNode(nums[0])

    n = len(nums)

    def binary(left: int, right: int) -> TreeNode:
        if left >= right: return None
        mid = (left + right) >> 1
        node = TreeNode(nums[mid])
        node.left = binary(left, mid)
        node.right = binary(mid + 1, right)
        return node

    return binary(0, n)
```