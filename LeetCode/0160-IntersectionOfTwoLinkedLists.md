
# 160. Intersection of Two Linked Lists


## 题目

```
Write a program to find the node at which the intersection of two singly linked lists begins.

Notes:
- If the two linked lists have no intersection at all, return null.
- The linked lists must retain their original structure after the function returns.
- You may assume there are no cycles anywhere in the entire linked structure.
- Your code should preferably run in O(n) time and use only O(1) memory.
```


<br>

## 解法


### Python


- 双指针法

```
## 双指针法
## 创建两个指针 pA 和 pB，分别初始化为链表 A 和 B 的头结点。然后让它们向后逐结点遍历。
## 当 pA 到达链表的尾部时，将它重定位到链表 B 的头结点 (你没看错，就是链表 B); 类似的，当 pB 到达链表的尾部时，将它重定位到链表 A 的头结点
## Time Complexity:O(m+n)  Space Complexity:O(1)
def getIntersectionNode_twoPointer(self, headA: ListNode, headB: ListNode) -> ListNode:
	if not headA or not headB: return None
	tempA, tempB = headA, headB
	while tempA.val != tempB.val:
		tempA = tempA.next if tempA else headB
		tempB = tempB.next if tempB else headA
	return tempA
```