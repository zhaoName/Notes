# 100. Same Tree


<br>

## 题目

```
Given the roots of two binary trees p and q, write a function to check if they are the same or not.
Two binary trees are considered the same if they are structurally identical, and the nodes have the same value.

Constraints:
The number of nodes in both trees is in the range [0, 100].
-104 <= Node.val <= 104
```

<br>

## 解法

```python3
def isSameTree(self, p: TreeNode, q: TreeNode) -> bool:
    if not p and not q: return True
    if not p or not q: return False

    pQueue, qQueue = [p], [q]
    while pQueue and qQueue:
        curP = pQueue.pop(0)
        curQ = qQueue.pop(0)
        if curP.val != curQ.val:
            return False

        if (not curP.left) ^ (not curQ.left):
            return False
        if (not curP.right) ^ (not curQ.right):
            return False

        if curP.left:
            pQueue.append(curP.left)
        if curP.right:
            pQueue.append(curP.right)
        if curQ.left:
            qQueue.append(curQ.left)
        if curQ.right:
            qQueue.append(curQ.right)
    return False if pQueue or qQueue else True
```

<br>