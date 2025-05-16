# 148. Sort List

## 题目
```
Given the head of a linked list, return the list after sorting it in ascending order.

Input: head = [-1,5,3,4,0]
Output: [-1,0,3,4,5]

Constraints:
The number of nodes in the list is in the range [0, 5 * 104].
-10^5 <= Node.val <= 10^5

Follow up: Can you sort the linked list in O(nlogn) time and O(1) memory (i.e. constant space)?
```

## 解法

```python3
class Solution:
    def divide(self, head: ListNode, tail: ListNode) -> ListNode:
        if not head: return head
        if head.next == tail:
            ## 只有两个节点
            head.next = None
            return head

        slow = fast = head
        while fast != tail:
            slow = slow.next
            fast = fast.next
            if fast != tail:
                fast = fast.next
        mid = slow
        return self.merge(self.divide(head, mid), self.divide(mid, tail))

    def merge(self, head1: ListNode, head2: ListNode) -> ListNode:
        dummy = ListNode()
        temp, temp1, temp2 = dummy, head1, head2
        while temp1 or temp2:
            if temp1 == None or (temp2 and temp1.val > temp2.val):
                temp.next = temp2
                temp2 = temp2.next
            else:
                temp.next = temp1
                temp1 = temp1.next
            temp = temp.next
        return dummy.next


    def sortList(self, head: ListNode) -> ListNode:
        return self.divide(head, None)
```