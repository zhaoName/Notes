# 347. Top K Frequent Elements


<br>

## 题目

```
Given an integer array nums and an integer k, return the k most frequent elements. You may return the answer in any order.

Input: nums = [1,1,1,2,2,3], k = 2
Output: [1,2]

Constraints:
1 <= nums.length <= 10^5
k is in the range [1, the number of unique elements in the array].
It is guaranteed that the answer is unique.

Follow up: Your algorithm's time complexity must be better than O(n log n), where n is the array's size.
```

<br>

## 解法

```python
# Time: O(n), space: O(n)
def topKFrequent(self, nums: List[int], k: int) -> List[int]:
    bucketD, n = {}, len(nums)
    for i in range(n):
        try:
            bucketD[nums[i]] += 1
        except:
            bucketD[nums[i]] = 1

    buckets = [None] * (n + 1)
    for num in bucketD.keys():
        index = bucketD[num]
        if buckets[index] == None:
            buckets[index] = list()
        buckets[index].append(num)

    ans, j = [], len(buckets) - 1
    while j >= 0 and len(ans) < k:
        if buckets[j] == None:
            j -= 1
            continue
        ans += buckets[j]
        j -= 1
    return ans
```

```python
def topKFrequent_quick_select(self, nums: List[int], k: int) -> List[int]:
    if not nums: return list()
    bucket_d = {}
    for i in range(len(nums)):
        try:
            bucket_d[nums[i]] = (bucket_d[nums[i]][0] + 1, nums[i])
        except:
            bucket_d[nums[i]] = (1, nums[i])

    ans = list(bucket_d.values())
    self.k_split(ans, 0, len(ans) - 1, len(ans) - k)
    return [value[1] for value in ans[len(ans)-k:]]



def partition(self, arr, left, right) -> int:
    pivot, i, j = arr[left], left, right
    while i < j:
        while i < j and arr[j][0] >= pivot[0]:
            j -= 1
        while i < j and arr[i][0] <= pivot[0]:
            i += 1
        arr[i], arr[j] = arr[j], arr[i]
    arr[i], arr[left] = arr[left], arr[i]
    return i

def k_split(self, arr, left, right, k):
    if left < right:
        index = self.partition(arr, left, right)
        if index == k:
            return
        elif index > k:
            self.k_split(arr, left, index - 1, k)
        else:
            self.k_split(arr, index + 1, right, k
```

<br>




