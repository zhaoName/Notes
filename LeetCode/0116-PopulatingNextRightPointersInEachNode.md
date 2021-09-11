
# 116. Populating Next Right Pointers in Each Node

## 题目

```
You are given a perfect binary tree where all leaves are on the same level, and every parent has two children. The binary tree has the following definition:
struct Node {
  int val;
  Node *left;
  Node *right;
  Node *next;
}
Populate each next pointer to point to its next right node. If there is no next right node, the next pointer should be set to NULL.
Initially, all next pointers are set to NULL.

Input: root = [1,2,3,4,5,6,7]
Output: [1,#,2,3,#,4,5,6,7,#]

Constraints:
The number of nodes in the tree is in the range [0, 2^12 - 1].
-1000 <= Node.val <= 1000
```

<br>

## 解法

```python3
def connect(self, root: 'Node') -> 'Node':
    if not root: return None

    queue, levelSize = [root], 1
    while queue:
        for i in range(levelSize):
            cur = queue.pop(0)
            if i + 1 < levelSize:
                cur.next = queue[0]
            if cur.left:
                queue.append(cur.left)
            if cur.right:
                queue.append(cur.right)

        levelSize = len(queue)
    return root
```