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

<br>


