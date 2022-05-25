# 324. Wiggle Sort II


<br>

## 题目

```
Given an integer array nums, reorder it such that nums[0] < nums[1] > nums[2] < nums[3]....
You may assume the input array always has a valid answer.

Input: nums = [1,5,1,1,6,4]
Output: [1,6,1,5,1,4]
Explanation: [1,4,1,5,1,6] is also accepted.

Constraints:
1 <= nums.length <= 5 * 10^4
0 <= nums[i] <= 5000
It is guaranteed that there will be an answer for the given input nums.

Follow Up: Can you do it in O(n) time and/or in-place with O(1) extra space?
```

<br>

## 解法

```python3
class Solution:
    def partition(self, nums, left, right):
        pivot = nums[left]
        i, j = left, right
        while i < j:
            while i < j and nums[j] >= pivot:
                j -= 1
            nums[i], nums[j] = nums[j], nums[i]
            while i < j and nums[i] <= pivot:
                i += 1
            nums[i], nums[j] = nums[j], nums[i]
        nums[i] = pivot
        return i

    def split(self, nums, left, right, k):
        if left < right:
            index = self.partition(nums, left, right)
            if index == k:
                return
            elif index > k:
                self.split(nums, left, index - 1, k)
            else:
                self.split(nums, index+1, right, k)

    # Time: O(n), space: O(n)
    def wiggleSort(self, nums: List[int]) -> None:
        midIndex = len(nums) // 2
        self.split(nums, 0, len(nums)-1, midIndex)
        mid = nums[midIndex]

        # 3-way-partition
        i, j, k = 0, 0, len(nums)-1
        while j < k:
            if nums[j] > mid:
                nums[j], nums[k] = nums[k], nums[j]
                k -= 1
            elif nums[j] < mid:
                nums[i], nums[j] = nums[j], nums[i]
                i += 1
                j += 1
            else:
                j += 1
        # nums 为奇数，前面数组元素较多
        if len(nums) % 2:
            midIndex += 1
        # 倒序穿插
        temp1 = nums[:midIndex]
        temp2 = nums[midIndex:]
        for m in range(len(temp1)):
            nums[2 * m] = temp1[len(temp1) - 1 - m]
        for n in range(len(temp2)):
            nums[2 * n + 1] = temp2[len(temp2) - 1 - n]
        print(nums)
```

```python
# Time: O(n), space: O(1)
def wiggleSort_vi(self, nums: List[int]) -> None:
    midIndex = len(nums) // 2
    self.split(nums, 0, len(nums)-1, midIndex)
    mid = nums[midIndex]

    # i是虚拟地址，(1+2*(i)) % (n|1)是实际地址。
    # 其中n为数组长度，如果n为偶数，(n|1)为n+1，如果n为奇数，(n|1)仍为n。
    def actIndex(i):
        n = len(nums)
        return (1 + 2 * i) % (n | 1)

    # 3-way-partition
    # 大数排在左边，小数排在右边
    i, j, k = 0, 0, len(nums)-1
    while j <= k:
        if nums[actIndex(j)] < mid:
            nums[actIndex(j)], nums[actIndex(k)] = nums[actIndex(k)], nums[actIndex(j)]
            k -= 1
        elif nums[actIndex(j)] > mid:
            nums[actIndex(j)], nums[actIndex(i)] = nums[actIndex(i)], nums[actIndex(j)]
            i += 1
            j += 1
        else:
            j += 1
        print(nums, mid)
```

<br>


