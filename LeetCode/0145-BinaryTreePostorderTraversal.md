# 145. Binary Tree Postorder Traversal

## 题目

```
Given the root of a binary tree, return the postorder traversal of its nodes' values.

Input: root = [1,null,2,3]
Output: [3,2,1]

Constraints:
The number of the nodes in the tree is in the range [0, 100].
-100 <= Node.val <= 100

Follow up: Recursive solution is trivial, could you do it iteratively?
```

<br>

## 解法

- 递归

```python3
def postorderTraversal(self, root: TreeNode) -> List[int]:
    ans = []
    def dfs(root: TreeNode):
        if not root: return
        dfs(root.left)
        dfs(root.right)
        ans.append(root.val)
    dfs(root)
    return ans
```

- 迭代

```python3
def postorderTraversal_iteratively(self, root: TreeNode) -> List[int]:
    ans = []
    if not root: return ans
    stack, pre = [], None
    while root or stack:
        while root:
            stack.append(root)
            root = root.left
        root = stack.pop()
        if not root.right or root.right == pre:
            ans.append(root.val)
            pre = root
            root = None
        else:
            stack.append(root)
            root = root.right
    return ans
```