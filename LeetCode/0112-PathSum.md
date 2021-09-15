# 112. Path Sum

## 题目

```
Given the root of a binary tree and an integer targetSum, return true if the tree has a root-to-leaf path such that adding up all the values along the path equals targetSum.
A leaf is a node with no children.

Input: root = [5,4,8,11,null,13,4,7,2,null,null,null,1], targetSum = 22
Output: true

Constraints:
The number of nodes in the tree is in the range [0, 5000].
-1000 <= Node.val <= 1000
-1000 <= targetSum <= 1000
```

<br>

## 解法

```python3
def hasPathSum(self, root: Optional[TreeNode], targetSum: int) -> bool:
    if not root: return False

    que = [root]
    que_val = [root.val]
    while que:
        cur = que.pop()
        cur_val = que_val.pop()

        if not cur.left and not cur.right:
            if cur_val == targetSum:
                return True
            continue
        if cur.left:
            que.append(cur.left)
            que_val.append(cur_val + cur.left.val)
        if cur.right:
            que.append(cur.right)
            que_val.append(cur_val + cur.right.val)
    return False
```