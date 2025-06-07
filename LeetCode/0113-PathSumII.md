
# 113.Path Sum II

## 题目

```
Given the root of a binary tree and an integer targetSum,
return all root-to-leaf paths where the sum of the node values in the path equals targetSum.
Each path should be returned as a list of the node values, not node references.

A root-to-leaf path is a path starting from the root and ending at any leaf node. A leaf is a node with no children.

Input: root = [5,4,8,11,null,13,4,7,2,null,null,5,1], targetSum = 22
Output: [[5,4,11,2],[5,8,4,5]]
Explanation: There are two paths whose sum equals targetSum:
5 + 4 + 11 + 2 = 22
5 + 8 + 4 + 5 = 22

Constraints:
The number of nodes in the tree is in the range [0, 5000].
-1000 <= Node.val <= 1000
-1000 <= targetSum <= 1000
```

## 解法

```python3
def pathSum(self, root: TreeNode, targetSum: int) -> List[List[int]]:
    if not root: return []

    ans = []
    que = collections.deque([root])
    sumQue = collections.deque([(root.val, [root.val])])

    while que:
        node = que.popleft()
        sum = sumQue.popleft()

        if sum[0] == targetSum and not node.left and not node.right:
            ans.append(sum[1])

        if node.left:
            que.append(node.left)
            sumQue.append((sum[0] + node.left.val, sum[1] + [node.left.val]))
        if node.right:
            que.append(node.right)
            sumQue.append((sum[0] + node.right.val, sum[1] + [node.right.val]))
    return ans
```

```python3
def pathSum_dfs(self, root: TreeNode, targetSum: int) -> List[List[int]]:
    if not root: return []

    ans, path = [], []

    def dfs(node: TreeNode, target: int):
        if not node: return
        path.append(node.val)
        target -= node.val

        if target == 0 and not node.left and not node.right:
            ans.append(path[:])
        dfs(node.left, target)
        dfs(node.right, target)
        # 路径不对的 要回溯
        path.pop()

    dfs(root, targetSum)
    return ans
```