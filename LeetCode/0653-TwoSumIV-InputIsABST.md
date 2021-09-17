# 653. Two Sum IV - Input is a BST

## 题目

```
Given the root of a Binary Search Tree and a target number k, return true if there exist two elements in the BST such that their sum is equal to the given target.

Input: root = [2,1,3], k = 3
Output: true

Constraints:
The number of nodes in the tree is in the range [1, 104].
-104 <= Node.val <= 104
root is guaranteed to be a valid binary search tree.
-105 <= k <= 105
```

## 解法

```python3
def findTarget(self, root: TreeNode, k: int) -> bool:
    if not root: return False

    que, d = [root], {(k-root.val): 1}
    while que:
        cur = que.pop()
        if cur.left:
            if cur.left.val in d:
                return True
            else: d[k - cur.left.val] = 1
            que.append(cur.left)
        if cur.right:
            if cur.right.val in d:
                return True
            else: d[k - cur.right.val] = 1
            que.append(cur.right)
    return False
```