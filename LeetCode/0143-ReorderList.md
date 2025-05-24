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

- 二刷

```python3
## Timer Complexity: O(n), Space Complexity: O(n)
def reorderList_02(self, head: ListNode) -> None:
    if not head: return
    stack, node = [], head
    while node:
        stack.append(node)
        node = node.next
    i, j = 0, len(stack) - 1
    while i < j:
        stack[i].next = stack[j]
        i += 1
        if i == j:
            break
        stack[j].next = stack[i]
        j -= 1
    stack[i].next = None


## Timer Complexity: O(n), Space Complexity: O(1) 
def reorderList(self, head: ListNode) -> None:
    if not head or not head.next: return

    slow = fast = head
    while fast and fast.next:
        slow = slow.next
        fast = fast.next.next

    dummyNode = ListNode()
    while slow:
        temp = slow.next
        slow.next = dummyNode.next
        dummyNode.next = slow
        slow = temp

    last, node = dummyNode.next, head
    while node and last:
        tempLast = last.next
        last.next = node.next
        node.next = last

        node = last.next
        last = tempLast
    node.next = None
```

<br>