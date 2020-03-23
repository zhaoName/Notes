# 876. Middle of the Linked List


## 题目

```
Given a non-empty, singly linked list with head node head, return a middle node of linked list.
If there are two middle nodes, return the second middle node.

Input: [1,2,3,4,5,6]
Output: Node 4 from this list (Serialization: [4,5,6])
Since the list has two middle nodes with values 3 and 4, we return the second one.

Note:
The number of nodes in the given list will be between 1 and 100
```

<br>

## 解法

### Python 

- 第一思路

```
## Time Complxity:O(n)    Space Complxity:O(1)
def middleNode(self, head: ListNode) -> ListNode:
	if not head: return None
    l, i, temp = 0, 0, head
    while temp:
        l += 1
        temp = temp.next
    while i < l // 2:
        head = head.next
        i += 1
    return head
```

- 借鉴思路 快慢指针 

```
## Time Complxity:O(n)    Space Complxity:O(1)
def middleNode_slow_fast(self, head: ListNode) -> ListNode:
	if not head: return None
    slow, fast = head, head
    while fast and fast.next:
        slow = slow.next
        fast = fast.next.next
    return slow
```