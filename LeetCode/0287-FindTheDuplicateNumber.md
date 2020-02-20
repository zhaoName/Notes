
# 287. Find the Duplicate Number


##  题目

```
Given an array nums containing n + 1 integers where each integer is between 1 and n (inclusive),
prove that at least one duplicate number must exist. 
Assume that there is only one duplicate number, find the duplicate one.

Note:
1.You must not modify the array (assume the array is read only).
2.You must use only constant, O(1) extra space.
3.Your runtime complexity should be less than O(n^2).
4.There is only one duplicate number in the array, but it could be repeated more than once.
```


<br>

## 解法

### Python

- 第一思路  不符合题意

```
## time complexity:O(n)  Space Complexity:O(n)
def findDuplicate_set(self, nums) -> int:
	s = set(nums)
	return (sum(nums) - sum(s)) // (len(nums) - len(s))
```


- 借鉴思路  转化成查找链表环的入口

```
## 把nums看成是顺序存储的链表，nums中每个元素的值是下一个链表节点的地址
## 那么如果nums有重复值，说明链表存在环，本问题就转化为了找链表中环的入口节点，因此可以用快慢指针解决
## time complexity:O(n)  Space Complexity:O(1)
def findDuplicate_(self, nums) -> int:
	slow, fast = 0, 0
	## 判断是否又环
	while True:
		slow, fast = nums[slow], nums[nums[fast]]
		if slow == fast: break
	## 找环入口 也就是重复数字
	find = 0
	while slow != find:
		slow, find = nums[slow], nums[find]
	return slow
```