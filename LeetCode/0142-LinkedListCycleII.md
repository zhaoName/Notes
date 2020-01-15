# 142. Linked List Cycle II


## 题目

```
Given a linked list, return the node where the cycle begins. 
If there is no cycle, return null.

To represent a cycle in the given linked list,
we use an integer pos which represents the position (0-indexed) in the linked list where tail connects to. 
If pos is -1, then there is no cycle in the linked list.

Note: Do not modify the linked list.
Follow-up: Can you solve it without using extra space?
```

<br>


## 解法

### Python

- 哈希表

```
## time complexity:O(n)  Space Complexity:O(n)
def detectCycle_set(self, head) -> ListNode:
	if head == None: return None
	s = set()
	while head:
		if head in s:
			return head
		s.add(head)
		head = head.next
	return None
```


- Floyd 算法

```
## time complexity:O(n)  Space Complexity:O(1)
def detectCycle_Floyd(self, head) -> ListNode:
	fast, slow = head, head
	## 判断是否有环
    while True:
        if not (fast and fast.next): return None
        fast, slow = fast.next.next, slow.next
        if fast == slow: break

    ## 查找环的入口点
    fast = head
    while fast != slow:
        fast, slow = fast.next, slow.next
    return fast
```