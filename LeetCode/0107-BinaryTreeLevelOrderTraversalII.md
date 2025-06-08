# 107.Binary Tree Level Order Traversal II

## 题目

```
Given the root of a binary tree, return the bottom-up level order traversal of its nodes' values.
(i.e., from left to right, level by level from leaf to root).

Input: root = [3,9,20,null,null,15,7]
Output: [[15,7],[9,20],[3]]

Input: root = []
Output: []

Constraints:
The number of nodes in the tree is in the range [0, 2000].
-1000 <= Node.val <= 1000
```

## 解法

```python3
def levelOrderBottom(self, root: TreeNode) -> List[List[int]]:
    if not root: return []

    deq = collections.deque([])
    que, temp, levelSize = [root], [], 1
    while que:
        node = que.pop(0)
        temp.append(node.val)
        levelSize -= 1

        if node.left:
            que.append(node.left)
        if node.right:
            que.append(node.right)

        if levelSize == 0:
            deq.appendleft(temp)
            levelSize = len(que)
            temp = []
    return list(deq)
```