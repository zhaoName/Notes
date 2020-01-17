
# 234. Palindrome Linked List


## 题目

```
Given a singly linked list, determine if it is a palindrome.

Follow up:
Could you do it in O(n) time and O(1) space?
```

<br>

## 解法

### python

- 第一思路

```
Space O(n/2)
def isPalindrome_linkList_list(self, head: ListNode) -> bool:
	if not head: return True

	n = 0
	while head:
		n += 1
		head = head.next

	l, i = [], 0
	while head:
		if i == n//2 and n % 2 != 0:
			None
		elif i < n//2:
			l.append(head.val)
		else:
			if l[-1] == head.val:
                l.pop()
            else:
                return False
		i += 1
		head = head.next
	return not l
```