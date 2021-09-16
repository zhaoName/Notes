# 701. Insert into a Binary Search Tree


## 题目

```
You are given the root node of a binary search tree (BST) and a value to insert into the tree. Return the root node of the BST after the insertion.
It is guaranteed that the new value does not exist in the original BST.
Notice that there may exist multiple valid ways for the insertion, as long as the tree remains a BST after insertion. You can return any of them.

Input: root = [4,2,7,1,3,null,null,null,null,null,null], val = 5
Output: [4,2,7,1,3,5]
 
Constraints:
The number of nodes in the tree will be in the range [0, 104].
-108 <= Node.val <= 108
All the values Node.val are unique.
-108 <= val <= 108
It's guaranteed that val does not exist in the original BST.
```

<br>

## 解法


```python3
def insertIntoBST(self, root: TreeNode, val: int) -> TreeNode:
    insert_node = TreeNode(val)
    if not root: return insert_node

    head = root
    while head:
        if head.val > val:
            if head.left:
                head = head.left
            else:
                head.left = insert_node
                break
        else:
            if head.right:
                head = head.right
            else:
                head.right = insert_node
                break
    return root
```