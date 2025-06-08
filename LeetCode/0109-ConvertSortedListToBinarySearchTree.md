# 109.Convert Sorted List to Binary Search Tree


## 题

```
给定一个单链表的头节点  head ，其中的元素 按升序排序 ，将其转换为 平衡 二叉搜索树。
平衡二叉树 是指该树所有节点的左右子树的高度相差不超过 1。

输入: head = [-10,-3,0,5,9]
输出: [0,-3,9,-10,null,5]
解释: 一个可能的答案是[0，-3,9，-10,null,5]，它表示所示的高度平衡的二叉搜索树。

输入: head = []
输出: []


提示:
head 中的节点数在[0, 2 * 10^4] 范围内
-105 <= Node.val <= 10^5
```

## 解

```python3
def sortedListToBST(self, head: ListNode) -> TreeNode:
    def getMidNode(left: ListNode, right: ListNode) -> ListNode:
        fast = slow = left
        while fast != right and fast.next != right:
            slow = slow.next
            fast = fast.next.next
        return slow

    def binarySearch(left: ListNode, right: ListNode) -> TreeNode:
        if left == right: return None

        mid = getMidNode(left, right)
        root = TreeNode(mid.val)
        root.left = binarySearch(left, mid)
        root.right = binarySearch(mid.next, right)
        return root

    return binarySearch(head, None)
```


```python3
def sortedListToBST_inorder(self, head: ListNode) -> TreeNode:
    def getLen(head: ListNode) -> int:
        ln = 0
        while head:
            head = head.next
            ln += 1
        return ln

    def buildTree(left: int, right: int) -> TreeNode:
        if left >= right: return None

        mid = (left + right) >> 1
        root = TreeNode()
        root.left = buildTree(left, mid)
        nonlocal head
        root.val = head.val
        head = head.next
        root.right = buildTree(mid + 1, right)
        return root

    return buildTree(0, getLen(head))
```
