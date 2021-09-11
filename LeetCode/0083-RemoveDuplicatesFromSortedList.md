
# 83. Remove Duplicates from Sorted List


## 题目

```
Given the head of a sorted linked list, delete all duplicates such that each element appears only once. Return the linked list sorted as well.

Input: head = [1,1,2,3,3]
Output: [1,2,3]

Constraints:
The number of nodes in the list is in the range [0, 300].
-100 <= Node.val <= 100
The list is guaranteed to be sorted in ascending order.
```


<br>


## 解法


### C 

```C
/**
 * 83. Remove Duplicates from Sorted List
 *
 * 时间复杂度:O(n)  空间复杂度O(1)
 */
struct ListNode* deleteDuplicates(struct ListNode* head)
{
   if (head == NULL) return NULL;
   
   struct ListNode *nd = head;
   while(nd->next)
   {
      if (nd->val == nd->next->val) {
         struct ListNode *delNd = nd->next;
         nd->next = nd->next->next;
         free(delNd);
      }
      else {
          nd = nd->next;
      }
   }
   return head;
}
```


### 二刷

```python3
def deleteDuplicates(self, head: ListNode) -> ListNode:
    if not head: return None
    preHead = head
    slow, fast = head, head.next
    while fast:
        if slow.val == fast.val:
            slow.next = fast.next
        else:
            slow = slow.next
        fast = fast.next
    return preHead
```