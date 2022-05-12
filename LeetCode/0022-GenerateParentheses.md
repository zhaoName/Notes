# 22. Generate Parentheses

<br>

## 题目

```
Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.

Input: n = 3
Output: ["((()))","(()())","(())()","()(())","()()()"]

Constraints:
1 <= n <= 8
```

<br>

## 解法

```python3
# 深度优先搜索
def generateParenthesis(self, n: int) -> List[str]:
	if n <= 0: return list()
	ans, pa = [], ""

	def dfs(pa, left, right):
		if left == 0 and right == 0:
			ans.append(pa)
		if right < left:
			# ')' 的数量一定要小于'('
			return
		if left > 0:
			dfs(pa + "(", left - 1, right)
		if right > 0:
			dfs(pa + ")", left, right - 1)

	dfs(pa, n, n)
	return ans
```

<br>