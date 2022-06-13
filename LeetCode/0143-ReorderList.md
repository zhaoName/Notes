# 143. Reorder List

## 题目

```
You are given the head of a singly linked-list. The list can be represented as:
L0 → L1 → … → Ln - 1 → Ln
Reorder the list to be on the following form:
L0 → Ln → L1 → Ln - 1 → L2 → Ln - 2 → …
You may not modify the values in the list's nodes. Only nodes themselves may be changed.

Input: head = [1,2,3,4,5]
Output: [1,5,2,4,3]

Constraints:
The number of nodes in the list is in the range [1, 5 * 104].
1 <= Node.val <= 1000
```

<br>

## 解法

```python3
# Time: O(n), Space:O(n)
def reorderList(self, head: ListNode) -> None:
    if not head: return

    stackHead, stack, len = head, [], 0
    while stackHead:
        stack.append(stackHead)
        stackHead = stackHead.next
        len += 1

    stackHead = head
    for i in range(len // 2):
        temp = stackHead.next
        popNode = stack.pop()
        if i == len // 2 and len % 2 == 0:
            popNode.next = None
        else:
            popNode.next = stackHead.next

        stackHead.next = popNode
        stackHead = temp
    stackHead.next = None
```

<br>