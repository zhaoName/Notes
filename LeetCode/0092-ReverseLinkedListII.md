# 92. Reverse Linked List II

## 题目

```
Given the head of a singly linked list and two integers left and right where left <= right,
reverse the nodes of the list from position left to position right, and return the reversed list.

Example 1:
Input: head = [1,2,3,4,5], left = 2, right = 4
Output: [1,4,3,2,5]

Example 2:
Input: head = [5], left = 1, right = 1
Output: [5]

Constraints:
The number of nodes in the list is n.
1 <= n <= 500
-500 <= Node.val <= 500
1 <= left <= right <= n

Follow up: Could you do it in one pass?
```

## 解法

```pyhon3
def reverseBetween(self, head: ListNode, left: int, right: int) -> ListNode:
    dummyNode = ListNode(-1)
    dummyNode.next = head
    pre = dummyNode

    for _ in range(left - 1):
        pre = pre.next

    cur = pre.next
    for _ in range(right - left):
        next = cur.next
        cur.next = next.next
        next.next = pre.next
        pre.next = next
    return dummyNode.next
```