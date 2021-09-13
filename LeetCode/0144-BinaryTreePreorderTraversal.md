# 144. Binary Tree Preorder Traversal

## 题目

```
Given the root of a binary tree, return the preorder traversal of its nodes' values.

Input: root = [1,null,2,3]
Output: [1,2,3]

Constraints:
The number of nodes in the tree is in the range [0, 100].
-100 <= Node.val <= 100

Follow up: Recursive solution is trivial, could you do it iteratively?
```

<br>

## 解法

- 递归

```python3
# Time Complexity: O(n), Space Complexity: O(n)
def preorderTraversal(self, root: TreeNode) -> List[int]:
    ans = []
    def dfs(node: TreeNode, ans: List[int]):
        if node == None: return
        ans.append(node.val)
        dfs(node.left, ans)
        dfs(node.right, ans)

    dfs(root, ans)
    return ans
```

- 迭代 - 栈

```python3
# Time Complexity: O(n), Space Complexity: O(logn) (最坏O(n) 树成连表)
def preorderTraversal_iteratively(self, root: TreeNode) -> List[int]:
    ans = []
    if not root: return ans

    stack, node = [], root
    while stack or node:
        while node:
            ans.append(node.val)
            stack.append(node)
            node = node.left
        node = stack.pop()
        node = node.right
    return ans


def preorderTraversal_stack(self, root: TreeNode) -> List[int]:
    ans = []
    if not root: return ans
    stack = [root]
    while stack:
        node = stack.pop()
        ans.append(node.val)
        if node.right:
            stack.append(node.right)
        if node.left:
            stack.append(node.left)
    return ans
```

- Morris

```python3
# Time Complexity: O(n), Space Complexity: O(1)
def preorderTraversal_morris(self, root: TreeNode) -> List[int]:
    ans = []
    if not root: return ans

    # 当前开始遍历的节点
    cur = root
    # 记录当前节点的左子树
    curLeft = None
    while cur:
        curLeft = cur.left
        if curLeft:
            # 找到当前左子树的最右侧节点，且这个节点应该在指向根结点之前，否则整个节点又回到了根结点。
            while curLeft.right and curLeft.right != cur:
                curLeft = curLeft.right
            if curLeft.right == None:
                # 若最右侧这个节点的右指针没有指向根结点，创建连接然后往下一个左子树的根结点进行连接操作。
                ans.append(cur.val)
                curLeft.right = cur
                cur = cur.left
                continue
            else:
                # 当左子树的最右侧节点有指向根结点，此时说明我们已经回到了根结点并重复了之前的操作，同时在回到根结点的时候我们应该已经处理完左子树的最右侧节点了，把路断开
                curLeft.right = None
        else:
            ans.append(cur.val)
        cur = cur.right
    return ans
```

<br>

<br>