# 215. Kth Largest Element in an Array


<br>

## 题目

```
Given an integer array nums and an integer k, return the kth largest element in the array.
Note that it is the kth largest element in the sorted order, not the kth distinct element.

Input: nums = [3,2,3,1,2,4,5,5,6], k = 4
Output: 4

Constraints:
1 <= k <= nums.length <= 10^4
-10^4 <= nums[i] <= 10^4
```

<br>

## 解法

```python3
# nums[i] <= nums[2*i+1] && nums[i] <= nums[2*i+2]
def findKthLargest(self, nums: List[int], k: int) -> int:
    def heapify(ks: List[int], index: int, length: int):
        largest = index
        left = 2 * index + 1
        right = 2 * index + 2

        if left < length and ks[left] > ks[largest]:
            largest = left
        if right < length and ks[right] > ks[largest]:
            largest = right

        if largest != index:
            ks[index], ks[largest] = ks[largest], ks[index]
            heapify(ks, largest, length)

    def fetchMin(ks: List[int], length: int) -> int:
        i = length // 2 - 1
        for i in range(i, -1, -1):
            heapify(ks, i, length)

    l = len(nums)
    fetchMin(nums, l)
    for j in range(l-1, l-k, -1):
        nums[0], nums[j] = nums[j], nums[0]
        l -= 1
        heapify(nums, 0, l)
    return nums[0]
```

<br>