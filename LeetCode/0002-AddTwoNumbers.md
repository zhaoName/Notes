
# 2. Add Two Numbers


## 题目

```
You are given two non-empty linked lists representing two non-negative integers. 
The digits are stored in reverse order and each of their nodes contain a single digit. 
Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example:
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
```

<br>

## 解法


### Python

- 第一思路  保留l1

```python3
## Time Complexity: O(max(m ,n))  Space Complexity:O(1)
def addTwoNumbers(self, l1: ListNode, l2: ListNode) -> ListNode:
	## lastNd 记录 l1 的最后一个节点，否则当 l1 较短，或者l1 l2一样长且有进位时，链表会断掉
	carry, save, lastNd = 0, l1, ListNode(0)
    while l1 and l2:
        temp = (l1.val + l2.val + carry) % 10
        carry = (l1.val + l2.val + carry) // 10
        l1.val = temp
        if l1.next == None:
            lastNd = l1
        l1 = l1.next 
        l2 = l2.next
    while l2:
        temp = (l2.val + carry) % 10
        carry = (l2.val + carry) // 10
        l2.val = temp
        lastNd.next = l2
        lastNd = lastNd.next
        l2 = l2.next
    while l1:
        temp = (l1.val + carry) % 10
        carry = (l1.val + carry) // 10
        l1.val = temp
        if l1.next == None:
            lastNd = l1
        l1 = l1.next
    if carry == 1:
        newNode = ListNode(1)
        lastNd.next = newNode
    return save
```


<br>

## 二刷 

2021-08-20

```python3
class Solution:
	def addTwoNumbers(self, l1: ListNode, l2: ListNode) -> ListNode:
		carry, head, tail = 0, None, None
		while l1 or l2:
			n1 = l1.val if l1 else 0
			n2 = l2.val if l2 else 0 
			sum = n1 + n2 + carry

			if head == None:
				head = ListNode(sum % 10)
				tail = head
			else:
				tail.next = ListNode(sum % 10)
				tail = tail.next

			carry = int(sum / 10)
			if l1:
				l1 = l1.next
			if l2:
				l2 = l2.next

		if carry == 1:
			tail.next = ListNode(1)
		return head
```
