# 114. Flatten Binary Tree to Linked List


<br>

## 题目

```
Given the root of a binary tree, flatten the tree into a "linked list":

The "linked list" should use the same TreeNode class where the right child pointer points to the next node in the list and the left child pointer is always null.
The "linked list" should be in the same order as a pre-order traversal of the binary tree.

Input: root = [1,2,5,3,4,null,6]
Output: [1,null,2,null,3,null,4,null,5,null,6]

Constraints:
The number of nodes in the tree is in the range [0, 2000].
-100 <= Node.val <= 100

Follow up: Can you flatten the tree in-place (with O(1) extra space)?
```

<br>

## 解法

```python3
# time: O(n), space: O(n)
def flatten(self, root: TreeNode) -> None:
    if not root: return

    stack, ans = [root], None
    while stack:
        cur = stack.pop()
        if ans:
            ans.left = None
            ans.right = cur
        if cur.right:
            stack.append(cur.right)
        if cur.left:
            stack.append(cur.left)
        ans = cur
```

```python3
# time: O(n), space: O(1)
# 问题转化成寻找当前节点的前驱节点
def flatten_(self, root: TreeNode) -> None:
    if not root: return

    cur = root
    while cur:
        if cur.left:
            predecessor = next = cur.left
            # 查找当前节点左子树 最右边的节点，也就是当前节点的右子节点的前驱结点
            while predecessor.right:
                predecessor = predecessor.right
            # 找到前驱结点后，将当前节点的右节点 赋值给前驱结点的右子节点
            predecessor.right = cur.right
            # 清空当前节点的左子节点
            cur.left = None
            # 将记录的当前节点的左子节点 赋值给当前节点的右子节点
            cur.right = next
        # 若当前节点的左子节点为空，则不需要展开查找前驱结点
        cur = cur.right
```

<br>