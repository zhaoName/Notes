# 199. Binary Tree Right Side View


## 题目

```
Given the root of a binary tree, imagine yourself standing on the right side of it, return the values of the nodes you can see ordered from top to bottom.

Input: root = [1,2,3,null,5,null,4]
Output: [1,3,4]

Constraints:
The number of nodes in the tree is in the range [0, 100].
-100 <= Node.val <= 100
```

<br>

## 解法

```python3
from Other import TreeNode
from typing import List
import collections

class Solution:
    def rightSideView(self, root: TreeNode) -> List[int]:
        if not root: return []
        ans, que = [], collections.deque()
        levelSize = 1
        que.appendleft(root)

        while que:
            cur = que.pop()
            levelSize -= 1
            if cur.right:
                que.appendleft(cur.right)
            if cur.left:
                que.appendleft(cur.left)
            if levelSize == 0:
                levelSize = len(que)
                ans.append(cur.val)
        return ans

    def rightSideView_dfs(self, root: TreeNode) -> List[int]:
        if not root: return []
        ans, nodes = [], []
        def dfs(root: TreeNode, index):
            if index > len(nodes):
                nodes.append([])
                ans.append(root.val)
            nodes[index-1].append(root.val)
            if root.right:
                dfs(root.right, index+1)
            if root.left:
                dfs(root.left, index+1)
        return ans
```