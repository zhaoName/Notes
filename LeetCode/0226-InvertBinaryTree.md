# 226. Invert Binary Tree


## 题目

```
Given the root of a binary tree, invert the tree, and return its root.

Input: root = [4,2,7,1,3,6,9]
Output: [4,7,2,9,6,3,1]

Constraints:
The number of nodes in the tree is in the range [0, 100].
-100 <= Node.val <= 100
```

<br>

## 解法

```python3
## 后续遍历
def invertTree_postOrder(self, root: TreeNode) -> TreeNode:
    if not root: return None

    self.invertTree(root.left)
    self.invertTree(root.right)
    (root.left, root.right) = (root.right, root.left)
    return root

## 中序遍历
def invertTree_inOrder(self, root: TreeNode) -> TreeNode:
    if not root: return None

    self.invertTree(root.left)
    (root.left, root.right) = (root.right, root.left)
    # 注意这里左右节点已经交换过，下面不能再传 root.right
    self.invertTree(root.left)
    return root


## 前序遍历
def invertTree_preOrder(self, root: TreeNode) -> TreeNode:
    if not root: return None

    (root.left, root.right) = (root.right, root.left)
    self.invertTree(root.left)
    self.invertTree(root.right)
    return root


## 层序遍历
def invertTree_level(self, root: TreeNode) -> TreeNode:
    if not root: return None

    que = [root]
    while que:
        cur = que.pop()
        cur.left, cur.right = cur.right, cur.left
        if cur.left:
            que.append(cur.left)
        if cur.right:
            que.append(cur.right)
    return root
```