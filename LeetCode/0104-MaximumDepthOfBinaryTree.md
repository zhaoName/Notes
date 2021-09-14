# 104. Maximum Depth of Binary Tree

## 题目

```
Given the root of a binary tree, return its maximum depth.
A binary tree's maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.

Input: root = [3,9,20,null,null,15,7]
Output: 3

Constraints:
The number of nodes in the tree is in the range [0, 104].
-100 <= Node.val <= 100
```

<br>

## 解法

```python3
def maxDepth(self, root: TreeNode) -> int:
    if not root: return 0

    que, depth, levelSize = [root], 0, 1
    while que:
        cur = que.pop(0)
        levelSize -= 1
        if cur.left:
            que.append(cur.left)
        if cur.right:
            que.append(cur.right)

        if levelSize == 0:
            depth += 1
            levelSize = len(que)
    return depth


def maxDepth_dfs(self, root: TreeNode) -> int:
    if not root: return 0

    max_right = self.maxDepth_dfs(root.right)
    max_left = self.maxDepth_dfs(root.left)
    return max(max_left, max_right) + 1
```