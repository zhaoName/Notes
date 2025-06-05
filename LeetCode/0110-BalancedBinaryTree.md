# 110.BalancedBinaryTree

## 题目

```
给定一个二叉树，判断它是否是 平衡二叉树
平衡二叉树是指该树所有节点的左右子树的高度相差不超过 1。

Constraints:
The number of nodes in the tree is in the range [0, 5000].
-10^4 <= Node.val <= 10^4
```

## 解法

```python3
# 自下而上
def isBalanced(self, root: TreeNode) -> bool:
	def dfs(node: TreeNode) -> int:
		if not node: return 0
		left = dfs(node.left)
		# -1 表示不是平衡二叉树 剪枝
		if left == -1: return -1
		right = dfs(node.right)
		if right == -1: return -1
		return max(left, right) + 1 if abs(left - right) <= 1 else -1

	return dfs(root) != -1
```

```python3
# 先序遍历 从顶至下
def isBalanced_pre(self, root: TreeNode) -> bool:
	if not root: return True

	def depth(node: TreeNode) -> int:
		if not node: return 0
		return max(depth(node.left), depth(node.right)) + 1

	return abs(depth(root.left) - depth(root.right)) <= 1 and self.isBalanced_pre(root.left) and self.isBalanced_pre(
		root.right)
```
