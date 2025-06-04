# 103.Binary Tree Zigzag Level Order Traversal

## 题目

```
Given the root of a binary tree, return the zigzag level order traversal of its nodes' values.
(i.e., from left to right, then right to left for the next level and alternate between).

Input: root = [3,9,20,null,null,15,7]
Output: [[3],[20,9],[15,7]]

Input: root = [1]
Output: [[1]]

Input: root = []
Output: []

Constraints:
The number of nodes in the tree is in the range [0, 2000].
-100 <= Node.val <= 100
```


## 解法

```python3
def zigzagLevelOrder(self, root: TreeNode) -> List[List[int]]:
    if not root: return []

    eles = deque()
    queue, ans = [root], []
    level, flag = 1, False
    while len(queue) != 0:
        node = queue.pop(0)
        if flag:
            eles.appendleft(node.val)
        else:
            eles.append(node.val)
        level -= 1

        if node.left:
            queue.append(node.left)
        if node.right:
            queue.append(node.right)

        if level == 0:
            flag = not flag
            ans.append(list(eles))
            eles = deque()
            level = len(queue)
    return ans
```

