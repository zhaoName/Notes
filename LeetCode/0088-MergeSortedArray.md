
# 88. Merge Sorted Array


## 题目

```
Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as one sorted array.

Note:
The number of elements initialized in nums1 and nums2 are m and n respectively.
You may assume that nums1 has enough space (size that is greater or equal to m + n) to hold additional elements from nums2.
```

<br>


## 解法


### Python

- 第一思路（不知道为啥不行）

```python3
# O(m+n)  O(1)
def merge(nums1, m, nums2, n):
    i = 0
    j = 0
    while i < (m + j) and j < n:
        if nums1[i] > nums2[j]:
            nums1.insert(i, nums2[j])
            j += 1
        else:
            i += 1

    if j < n:
        nums1 = nums1[0:(j + m)] + nums2[j:n]
    else:
        nums1 = nums1[0:(j + m)]
```

- 为解决第一思路

```python3
## 双指针 从前往后
## 时间复杂度：O(m+n)  空间复杂度：O(m)
def merge_1(nums1, m, nums2, n):
    i = 0;
    j = 0
    nums1_copy = nums1[:m]
    nums1[:] = []
    while i < m and j < n:
        if nums1_copy[i] > nums2[j]:
            nums1.append(nums2[j])
            j += 1
        else:
            nums1.append(nums1_copy[i])
            i += 1

    if j < n:
        nums1[(j + m):] = nums2[j:n]
    if i < m:
        nums1[i + n:] = nums1_copy[i:m]
```

- 网上优化思路

```python3
## 双指针 从后往前
## 时间复杂度：O(m+n)  空间复杂度：O(1)
def merge_2(nums1, m, nums2, n):
    i = m - 1;
    j = n - 1
    index = m + n - 1
    # 题目上都假设 nums1 的空间足够大，为啥还报越界错误！！！！
    if m + n > len(nums1):
        nums1[len(nums1):] = [0] * (m + n - len(nums1))
    while i >= 0 and j >= 0:
        if nums1[i] < nums2[j]:
            nums1[index] = nums2[j]
            j -= 1
        else:
            nums1[index] = nums1[i]
            i -= 1
        index -= 1

    if j >= 0:
        nums1[:j + 1] = nums2[:j + 1]
```


<br>



### C


```C
/**
 * 88. Merge Sorted Array
 * 双指针 从后往前
 * 时间复杂度：O(m+n) 空间复杂度：O(1)
 */
void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n)
{
    int i = m-1, j = n-1;
   int index = m + n -1;
   
   while (i>=0 && j>=0)
   {
      if (nums1[i] < nums2[j]) {
         nums1[index] = nums2[j];
         j--;
      }
      else {
         nums1[index] = nums1[i];
         i--;
      }
      index--;
   }
   
   if (j>=0) {
      memmove(nums1, nums2, (j+1)*sizeof(int));
   }
}
```