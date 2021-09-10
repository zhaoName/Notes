
# 141. Linked List Cycle



## 题目

```
Given a linked list, determine if it has a cycle in it.

To represent a cycle in the given linked list, 
we use an integer pos which represents the position (0-indexed) in the linked list where tail connects to. 
If pos is -1, then there is no cycle in the linked list.

Follow up:
Can you solve it using O(1) (i.e. constant) memory?
```

<br>

## 解法


### C


- 快慢指针法(类似 202-HappyNumber)

```C
/**
 * 141. Linked List Cycle
 * time complexily:O(n)  space complexily:O(1)
 *
 * 双指针法：快指针走两步 慢指针走一步
 */
bool hasCycle(struct ListNode *head)
{
   if (head == NULL) return false;
   struct ListNode *fast = head->next;
   if (fast == NULL) return false;
   fast = fast->next;
   
   while (head && fast)
   {
      if (head == fast) {
         return true;
      }
      head = head->next;
      if (fast->next) {
         fast = fast->next->next;
      }
      else return false;
   }
   return false;
}
```

<br>


### Python

- 集合 哈希表

```python3
## Time complexity:O(n)  Space complexity:O(n)
def hasCycle(self, head) -> bool:
	s = set()
	while head:
		if head in s:
			return True
		s.add(head)
		head = head.next
	return False
```


### 二刷

```python3
# 快慢指针
def hasCycle(self, head: ListNode) -> bool:
    if not head: return False
    slow = fast = head
    while fast and fast.next:
        slow = slow.next
        fast = fast.next.next
        if fast == slow:
            return True
    return False
```