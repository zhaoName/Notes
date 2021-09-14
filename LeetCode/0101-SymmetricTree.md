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

- 递归

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

- 迭代

```python3
def isSymmetric_iteratively(self, root: TreeNode) -> bool:
    # 只有根节点 也是对称二叉树
    if not root or (not root.left and not root.right): return True

    que = [root.left, root.right]
    while que:
        leftNode = que.pop(0)
        rightNode = que.pop(0)
        # 同时为空 继续往下遍历
        if not leftNode and not rightNode: continue
        # 有一个为空 或者值不等 返回 False
        if (not leftNode or not rightNode) or leftNode.val != rightNode.val: return False

        que.append(leftNode.left)
        que.append(rightNode.right)
        que.append(leftNode.right)
        que.append(rightNode.left)
    return True
```





