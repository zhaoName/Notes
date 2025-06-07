# 111. Minimum Depth of Binary Tree

## 题目

```
Given a binary tree, find its minimum depth.
The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
Note: A leaf is a node with no children.

Input: root = [3,9,20,null,null,15,7]
Output: 2

Input: root = [2,null,3,null,4,null,5,null,6]
Output: 5

Constraints:
The number of nodes in the tree is in the range [0, 10^5].
-1000 <= Node.val <= 1000
```


## 解法

```python3
# 击败 100%
def minDepth(self, root: TreeNode) -> int:
    if not root: return 0

    ans, level, que = 1, 1, [root]
    while len(que) != 0:
        node = que.pop(0)
        level -= 1

        if not node.left and not node.right:
            return ans

        if node.left:
            if not node.left.left and not node.left.right:
                return ans
            que.append(node.left)
        if node.right:
            if not node.right.left and not node.right.right:
                return ans
            que.append(node.right)

        if level == 0:
            ans += 1
            level = len(que)
    return ans
```

- 官方解法 

```
# 击败 74%
def minDepth(self, root: TreeNode) -> int:
    if not root: return 0

    ans, level, que = 1, 1, [root]
    while len(que) != 0:
        node = que.pop(0)
        level -= 1

        if not node.left and not node.right:
            return ans
        if node.left:
            que.append(node.left)
        if node.right:
            que.append(node.right)

        if level == 0:
            ans += 1
            level = len(que)
    return ans
```
