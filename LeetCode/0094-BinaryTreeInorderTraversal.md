
# 94. Binary Tree Inorder Traversal

## 题目

```
Given the root of a binary tree, return the inorder traversal of its nodes' values.

Input: root = [1,null,2,3]
Output: [1,3,2]

Constraints:
The number of nodes in the tree is in the range [0, 100].
-100 <= Node.val <= 100

Follow up: Recursive solution is trivial, could you do it iteratively?
```

<br>

## 解法

- 递归

```python3
def inorderTraversal_recursive(self, root: TreeNode) -> List[int]:
    ans = []
    def dfs(root: TreeNode):
        if not root: return
        dfs(root.left)
        ans.append(root.val)
        dfs(root.right)
    dfs(root)
    return ans
```

- 迭代

```python3
def inorderTraversal_iteratively(self, root: TreeNode) -> List[int]:
    ans = []
    if not root: return ans

    stack = []
    while root or stack:
        while root:
            stack.append(root)
            root = root.left
        root = stack.pop()
        ans.append(root.val)
        root = root.right
    return ans
```