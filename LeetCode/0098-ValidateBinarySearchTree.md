# 98. Validate Binary Search Tree

## 题目

```
Given the root of a binary tree, determine if it is a valid binary search tree (BST).

A valid BST is defined as follows:
The left subtree of a node contains only nodes with keys less than the node's key.
The right subtree of a node contains only nodes with keys greater than the node's key.
Both the left and right subtrees must also be binary search trees.

Input: root = [5,1,4,null,null,3,6]
Output: false
Explanation: The root node's value is 5 but its right child's value is 4.

Constraints:
The number of nodes in the tree is in the range [1, 104].
-2^31 <= Node.val <= 2^31 - 1
```

<br>

## 解法

```python3
class Solution:
def __init__(self):
    self.pre = -2**32

# 二叉搜索树的当前节点的值 要大于其前驱节点的值
def isValidBST_recursive(self, root: TreeNode) -> bool:
    if not root: return True

    if not self.isValidBST(root.left):
        return False
    if root.val <= self.pre:
        return False
    self.pre = root.val
    return self.isValidBST(root.right)


def isValidBST_iteratively(self, root: TreeNode) -> bool:
    if not root: return True
    stack, pre = [], float('-inf')
    while stack or root:
        while root:
            stack.append(root)
            root = root.left
        root = stack.pop()
        if root.val <= pre:
            return False
        pre = root.val
        root = root.right
    return True
```