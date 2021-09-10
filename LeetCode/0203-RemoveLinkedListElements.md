
# 203. Remove Linked List Elements

## 题目

```
Given the head of a linked list and an integer val, remove all the nodes of the linked list that has Node.val == val, and return the new head.

Input: head = [7,7,7,7], val = 7
Output: []

Constraints:
The number of nodes in the list is in the range [0, 104].
1 <= Node.val <= 50
0 <= val <= 50
```

<br>


## 解法

### 第一思路

- 带头结点

```C
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
void zz_deleteNode(ZZHeadLinkedList *list, void *data)
{
    assert(list);
    Node *nd = list->head;
    while (nd->next)
    {
        if (nd->next->data == data) {
            Node *delNode = nd->next;
            nd->next = nd->next->next;
            free(delNode);
            list->count--;
        }
        else {
            nd = nd->next;
        }
    }
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
ZZLinkedList* zz_deleteNodeWithData(ZZLinkedList *list, void *data)
{
    assert(list);
    // you need to give priority ot determine if the list is empty
    // because the list may be empty after deleting a node.
    while (list && list->data == data) {
        // delete first node
        Node *nd = list;
        list = list->next;
        free(nd);
        count--;
    }
    // list == null indicates that all nodes in the linked list have been deleted
    // eg. list: 1->1->null   data: 1
    if (list == NULL) return NULL;
    // count limit range, no need to judge node->next is null
    // lookup the previous node that need to deleted
    Node *nd = list;
    while (nd->next) {
        if (nd->next->data == data) {
            Node *delNode = nd->next;
            nd->next = nd->next->next;
            free(delNode);
            count--;
        } else {
            nd = nd->next;
        }
    }
    return list;
}
```

<br>


### 参考别人

递归思路：从后往前删除节点。

```
/**
 * struct ListNode {
 *    int val;
 *    struct ListNode *next;
 * };
 */
struct ListNode* removeElements(struct ListNode* head, int val)
{
    if (head == NULL) return NULL;
    
    head->next = removeElements(head->next, val);
    if (head->val == val)
        return head->next;
    else
        return head;
}
```


### 二刷

```python3
def removeElements(self, head: ListNode, val: int) -> ListNode:
    preHead = ListNode(-1)
    preHead.next = head
    pre = preHead
    while pre.next:
        if pre.next.val == val:
            pre.next = pre.next.next
        else:
            pre = pre.next
    return preHead.next
```

