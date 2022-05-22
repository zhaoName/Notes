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
# nums[i] >= nums[2*i+1] && nums[i] >= nums[2*i+2]
# time complexity: O(n * logn)
def findKthLargest_largest_heap(self, nums: List[int], k: int) -> int:
    # 大顶堆
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

    def buildLargestHeap(ks: List[int], length: int):
        i = length // 2 - 1
        for i in range(i, -1, -1):
            heapify(ks, i, length)

    l = len(nums)
    buildLargestHeap(nums, l)
    for j in range(l-1, l-k, -1):
        nums[0], nums[j] = nums[j], nums[0]
        l -= 1
        heapify(nums, 0, l)
    return nums[0]
```


```python3
# 小顶堆 nums[i] <= nums[2*i+1] && nums[i] <= nums[2*i+2]
# time complexity: O(n * logk)
def findKthLargest_min_heap(self, nums: List[int], k: int) -> int:
    heaps = nums[:k]
    heapq.heapify(heaps)
    for i in range(k, len(nums)):
        heapq.heappushpop(heaps, nums[i])
    return heaps[0]
```

```python3
def partition(self, nums: List[int], left: int, right: int):
    i, j, pivot = left, right, nums[left]
    while i < j:
        # 先右边
        while i < j and nums[j] >= pivot:
            j -= 1
        nums[i], nums[j] = nums[j], nums[i]
        # 再左边
        while i < j and nums[i] <= pivot:
            i += 1
        nums[i], nums[j] = nums[j], nums[i]
    # 此时 i== j, 将 pivot 放到最终位置
    nums[i] = pivot
    # 返回分割数据下标
    return i

def topK_split(self, nums: List[int], k: int, left: int, right: int):
    index = self.partition(nums, left, right)
    if index == k:
        return
    elif index < k:
        self.topK_split(nums, k, index+1, right)
    else:
        self.topK_split(nums, k, left, index - 1)


# time complexity: O(n)
def findKthLargest(self, nums: List[int], k: int) -> int:
    # partition 是按从小到大划分的，如果让index左边为前n-k个小的数，则index右边为前k个大的数
    self.topK_split(nums, len(nums)-k, 0, len(nums)-1)
    return nums[len(nums)-k]
```

<br>