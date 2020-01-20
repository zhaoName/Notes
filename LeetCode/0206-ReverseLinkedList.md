
# 206.Reverse Linked List



## 题目

```
Reverse a singly linked list.

Example:
Input: 1->2->3->4->5->NULL
Output: 5->4->3->2->1->NULL

Follow up:
A linked list can be reversed either iteratively or recursively. Could you implement both?
```

<br>


## 解法

### C 

- 带头结点

```
/**
 * typedef struct ZZNode{
 *    void *data;
 *    struct ZZNode *next;
 * }Node;
 *
 * typedef struct ZZHeadLinkedList{
 *    unsigned int count;
 *    Node *head;
 * }ZZHeadLinkedList;
*/
ZZHeadLinkedList* zz_reverse_headLinkedList(ZZHeadLinkedList *list)
{
    assert(list);
    ZZHeadLinkedList *reverseList = zz_init_headLinkedList();
    Node *nd = list->head->next;
    while (nd) {
        Node *nextNode = nd->next;
        nd->next = reverseList->head->next;;
        reverseList->head->next = nd;
        nd = nextNode;
    }
    return reverseList;
}
```


- 不带头结点

```
/**
 * typedef struct ZZNode{
 *    void *data;
 *    struct ZZNode *next;
 * }Node;
 * 
 * typedef struct ZZNode ZZLinkedList;
 */
ZZLinkedList* zz_reverse_linkedList(ZZLinkedList *list)
{
    if (list == NULL) return NULL;
    
    ZZLinkedList *reverseList = zz_init_linkedList();
    Node *nd = list;
    int i = 0;
    while (nd) {
        Node *nextNode = nd->next;
        if (i == 0) {
            reverseList->data = nd->data;
            reverseList->next = NULL;
        }
        else{
            nd->next = reverseList;
            reverseList = nd;
        }
        i++;
        nd = nextNode;
    }
    return reverseList;
}
```

<br>

### Python

```
def reverseList(self, head: ListNode) -> ListNode:
	if not head: return None
	
	reNode = ListNode(0)
	temp, i = head, 0
	while temp:
		nextNd = temp.next
		if i == 0:
			reNode.val = temp.val
		else:
			temp.next = reNode
			reNode = temp
		temp = nextNd
		i += 1
	return reNode
```