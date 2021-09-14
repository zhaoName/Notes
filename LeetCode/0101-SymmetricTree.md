# 101. Symmetric Tree


## 题目

```
Given the root of a binary tree, check whether it is a mirror of itself (i.e., symmetric around its center).

Input: root = [1,2,2,3,4,4,3]
Output: true

Constraints:
The number of nodes in the tree is in the range [1, 1000].
-100 <= Node.val <= 100

Follow up: Could you solve it both recursively and iteratively?
```

<br>


## 解法

```python3
# 递归
def isSymmetric_recursive(self, root: TreeNode) -> bool:
    if not root: return True
    return self.check(root, root)

def check(self, p: TreeNode, q: TreeNode) -> bool:
    if not p and not q: return True
    if not p or not q: return False
    return p.val == q.val and self.check(p.left, q.right) and self.check(p.right, q.left)
```