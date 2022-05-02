# 1305. All Elements in Two Binary Search Trees

<br>

## 题目

```
Given two binary search trees root1 and root2, return a list containing all the integers from both trees sorted in ascending order.

Input: root1 = [2,1,4], root2 = [1,0,3]
Output: [0,1,1,2,3,4]

Constraints:
The number of nodes in each tree is in the range [0, 5000].
-105 <= Node.val <= 105
```

<br>

## 解法

```python3
# 二叉搜索树的中序遍历结果是升序的
def getAllElements(self, root1: TreeNode, root2: TreeNode) -> List[int]:
    def inorder(root: TreeNode, res: List[int]):
        stack = []
        while root or stack:
            while root:
                stack.append(root)
                root = root.left
            root = stack.pop()
            res.append(root.val)
            root = root.right

    nums1, nums2 = [], []
    inorder(root1, nums1)
    inorder(root2, nums2)

    merged = []
    p1, n1 = 0, len(nums1)
    p2, n2 = 0, len(nums2)
    while True:
        if p1 == n1:
            merged.extend(nums2[p2:])
            break
        if p2 == n2:
            merged.extend(nums1[p1:])
            break
        if nums1[p1] < nums2[p2]:
            merged.append(nums1[p1])
            p1 += 1
        else:
            merged.append(nums2[p2])
            p2 += 1
    return merged
```

<br>