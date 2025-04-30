# 23.Merge k Sorted Lists

## 题目
```
You are given an array of k linked-lists lists, each linked-list is sorted in ascending order.
Merge all the linked-lists into one sorted linked-list and return it.

Example 1:
Input: lists = [[1,4,5],[1,3,4],[2,6]]
Output: [1,1,2,3,4,4,5,6]
Explanation: The linked-lists are:
[
  1->4->5,
  1->3->4,
  2->6
]
merging them into one sorted list:
1->1->2->3->4->4->5->6

Example 2:
Input: lists = []
Output: []

Example 3:
Input: lists = [[]]
Output: []

Constraints:
k == lists.length
0 <= k <= 10^4
0 <= lists[i].length <= 500
-10^4 <= lists[i][j] <= 10^4
lists[i] is sorted in ascending order.
The sum of lists[i].length will not exceed 10^4.
```

## 解法

```python3
class Solution:
    ListNode.__lt__ = lambda a, b: a.val < b.val

    def mergeKLists(self, lists: List[Optional[ListNode]]) -> Optional[ListNode]:
        heap = []
        pivot = dummy = ListNode()

        for head in lists:
            if head:
                heapq.heappush(heap, head)

        while heap:
            curLink = heapq.heappop(heap)
            if curLink.next:
                heapq.heappush(heap, curLink.next)
            pivot.next = curLink
            pivot = curLink
        return dummy.next
```