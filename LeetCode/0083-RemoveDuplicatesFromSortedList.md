
# 83. Remove Duplicates from Sorted List


## 题目

```
Given a sorted linked list, delete all duplicates such that each element appear only once.

Example
Input: 1->1->2->3->3
Output: 1->2->3
```


<br>


## 解法


### C 

```
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