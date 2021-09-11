# 102. Binary Tree Level Order Traversal

## 题目

```
Given the root of a binary tree, return the level order traversal of its nodes' values. (i.e., from left to right, level by level).

Input: root = [3,9,20,null,null,15,7]
Output: [[3],[9,20],[15,7]]

Constraints:
The number of nodes in the tree is in the range [0, 2000].
-1000 <= Node.val <= 1000
```


<br>

## 解法

```python3
from Other import TreeNode
from typing import List
import collections

class Solution:
    # 借助队列 迭代法
    def levelOrder(self, root: TreeNode) -> List[List[int]]:
        if not root: return []

        ans, queue = [], collections.deque()
        queue.appendleft(root)
        levelSize, depth = 1, 0

        while queue:
            cur = queue.pop()
            levelSize -= 1
            try:
                ans[depth].append(cur.val)
            except:
                ans.append([cur.val])

            if cur.left:
                queue.appendleft(cur.left)
            if cur.right:
                queue.appendleft(cur.right)
            if levelSize == 0:
                levelSize = len(queue)
                depth += 1
        return ans

    # 深度优先搜索 递归
    def levelOrder_dfs(self, root: TreeNode) -> List[List[int]]:
        if not root: return []

        res = []
        def dfs(root: TreeNode, index: int) -> List[List[int]]:
            if index > len(res):
                res.append([])
            res[index-1].append(root.val)
            if root.left:
                dfs(root.left, index+1)
            if root.right:
                dfs(root.right, index+1)
        dfs(root, 1)
        return res
```

