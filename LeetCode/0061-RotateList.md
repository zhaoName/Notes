
# 61. Rotate List


## 题目

```
Given a linked list, rotate the list to the right by k places, where k is non-negative.

Example 1:
Input: 1->2->3->4->5->NULL, k = 2
Output: 4->5->1->2->3->NULL
Explanation:
rotate 1 steps to the right: 5->1->2->3->4->NULL
rotate 2 steps to the right: 4->5->1->2->3->NULL
```


<br>


## 解法


### Python

- 第一思路

```
## Time Complexity:O(n)   Space Complexity:O(1)
def rotateRight(self, head: ListNode, k: int) -> ListNode:
	if not head: return None

	listLen, tNd = 0, head
	while tNd:
		listLen += 1
		tNd = tNd.next
	## 整倍数 不需要移动
	if k % listLen == 0: return head
	k %= listLen

	## kNode 为链表后面 k 个节点
	## 遍历到第 listLen-k-1 个节点，将其next 置为空
	i, kNode, tempNd = 0, ListNode(0), head
	while i<listLen-k:
		if i == listLen-k-1: 
			kNode = tempNd.next
			tempNd.next = None
			break
		tempNd = tempNd.next
		i += 1

	resultNd = kNode;
	while kNode.next: 
		kNode = kNode.next
	kNode.next = head
	return resultNd
```