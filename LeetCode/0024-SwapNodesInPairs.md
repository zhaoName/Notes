# 24.Swap Nodes In Pairs


## 题目

```
Given a linked list, swap every two adjacent nodes and return its head.
You must solve the problem without modifying the values in the list's nodes (i.e., only nodes themselves may be changed.)

Example 1:
Input: head = [1,2,3,4]
Output: [2,1,4,3]

Example 2:
Input: head = []
Output: []

Example 3:
Input: head = [1]
Output: [1]

Example 4:
Input: head = [1,2,3]
Output: [2,1,3]

Constraints:
The number of nodes in the list is in the range [0, 100].
0 <= Node.val <= 100
```


## 解法

```python3
# 第一思路
def swapPairs_(self, head: ListNode) -> ListNode:
    if not head or not head.next: return head

    res, pre = ListNode(), None
    while head:
        slow, fast = head, head.next
        if fast:
            temp = fast.next
            fast.next = slow
            slow.next = temp
            if pre:
                pre.next = fast
            if not res.next:
                res = fast

        head = slow.next
        pre = slow
    return res

# 官方解法
def swapPairs(self, head: ListNode) -> ListNode:
    dummyHead = ListNode()
    dummyHead.next = head
    temp = dummyHead

    while temp.next and temp.next.next:
        node1 = temp.next
        node2 = temp.next.next
        # 交换
        temp.next = node2
        node1.next = node2.next
        node2.next = node1
        temp = node1
    return dummyHead.next
```