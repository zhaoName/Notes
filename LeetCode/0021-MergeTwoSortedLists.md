# 21. Merge Two Sorted Lists


## C


### 第一思路

```
/**
 * struct ListNode {
 *    int val;
 *    struct ListNode *next;
 * };
 */
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2){
    
    if (l1 == NULL && l2 == NULL) return NULL;
    if (l1 == NULL && l2 != NULL) return l2;
    if (l1 != NULL && l2 == NULL) return l1;
    
    struct ListNode *curNode = l1;
    // 保留l1
    if (curNode->val > l2->val) {
        struct ListNode *nd = l2;
        l2 = l2->next;
        
        nd->next = l1;
        l1 = nd;
        // 有可能出现 l2 的第二个 第三个节点的值也比 l1 的第一个节点值小。eg:[5] [1,2,4]
        curNode = l1;
    }
    while (curNode->next && l2)
    {
        if (curNode->next->val > l2->val) {
            struct ListNode *nd = l2;
            l2 = l2->next;
            nd->next = curNode->next;
            curNode->next = nd;
        }
        else {
            curNode = curNode->next;
        }
    }
    // l1 已比较完 但 l2 还有节点未参与比较 eg: [1, 4]  [2, 3]
    if (curNode->next == NULL && l2 != NULL) {
        curNode->next = l2;
    }
    return l1;
}
```

<br>

### 官网递归解法


递归解法和第一思路的差别在于**第一思路是把 l2 往 l1 上合并，而递归是返回最小元素所在的链表。**

递归思路：

- 先判断返回那个链表(返回第一个节点值较小的链表)

- 依次遍历两个链表的剩余节点，调整节点位置

- 直到某个链表为空(另一个链表不管是否为null 都直接放到合并好的链表末尾)，递归结束

```
/**
 * struct ListNode {
 *    int val;
 *    struct ListNode *next;
 * };
 */
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) 
{
    if(l1==NULL)
        return l2;
    if(l2==NULL)
        return l1;
    if(l1->val < l2->val){
        l1->next = mergeTwoLists(l1->next,l2);
        return l1;
    }else{
        l2->next = mergeTwoLists(l1,l2->next);
        return l2;
    }
}
```