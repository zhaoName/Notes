
# 82. Remove Duplicates from Sorted List II


## 题目

```
Given a sorted linked list, delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list.

Example 1:
Input: 1->2->3->3->4->4->5
Output: 1->2->5

Example 2:
Input: 1->1->1->2->3
Output: 2->3
```


<br>


## 解法


### C


```
/**
 * 82. Remove Duplicates from Sorted List II
 *
 * 时间复杂度:O(n)  空间复杂度O(1)
 * 注意问题：
 * 1> 结尾出现重复节点 [1,2,3,3,3]  处理不好可能出现末尾重复节点没有删除
 * 2> 全部重复节点 [1,1,1] or [1,1,2,2,2], 特殊情况：就两个重复节点[1,1] 或 末尾有两个重复节点[1,1,1,2,2]
 * 3> 前 n 个节点是重复的 [1,1,1,2,2,3...] 注意删除首节点和非首节点的区别(带头节点链表不用考虑这个问题)
 */
struct ListNode* deleteDuplicates_II(struct ListNode* head)
{
   if (head == NULL) return NULL;
   
   struct ListNode *nd = head;
   struct ListNode *nextNd = head->next;
   if (nextNd == NULL) return head;
   
   int i = 0;
   while (nextNd->next)
   {
      if (nextNd->val == nextNd->next->val) {
         i++;
      }
      else {
         // 要删除第一个节点
         if (nd->val == nextNd->val) {
            head = nextNd->next;
            // head->next 可能为 NULL 如：[1,1,2]
            if (head->next == NULL) return head;
            nd = head;
            nextNd = head;
         }
         else {
            if (i>0) {
               // 有需要删除的节点
               nd->next = nextNd->next;
            }else{
               // 没有要删除的节点 直接挪动 nd 的位置
               nd = nextNd;
            }
         }
         i = 0;
      }
      nextNd = nextNd->next;
   }
   // 这里不用判断 i>0  因为你可能就两个节点 [1,1] or [1,1,1,2,2]
   if (head->val == nextNd->val) {
      // 整个链表都是重复节点
      head = NULL;
   }
   // 这里需要判断 i>0
   if (i>0 && nd->val != nextNd->val) {
      // 链表末尾有重复节点
      nd->next = NULL;
   }
   return head;
}
```