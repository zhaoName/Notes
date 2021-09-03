
# 617. Merge Two Binary Trees

## 题目

```
You are given two binary trees root1 and root2.

Imagine that when you put one of them to cover the other, some nodes of the two trees are overlapped while the others are not. You need to merge the two trees into a new binary tree.
The merge rule is that if two nodes overlap, then sum node values up as the new value of the merged node. Otherwise, the NOT null node will be used as the node of the new tree.
Return the merged tree.
Note: The merging process must start from the root nodes of both trees.

Constraints:
The number of nodes in both trees is in the range [0, 2000].
-104 <= Node.val <= 104
```

<br>

## 解法

```python3
class Solution:
    ## dfs
    def mergeTrees(self, root1: TreeNode, root2: TreeNode) -> TreeNode:
        if root1 == None: return root2
        if root2 == None: return root1

        mergeNode = TreeNode(root1.val + root2.val)

        mergeNode.left = self.mergeTrees(root1.left, root2.left)
        mergeNode.right = self.mergeTrees(root1.right, root2.right)
        return mergeNode
```