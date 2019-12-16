
# 35. Search Insert Position


## C  


### 第一思路

```
int searchInsert(int* nums, int numsSize, int target)
{
    if (numsSize <= 0) return 0;
    
    int i = 0;
    while (i < numsSize && nums[i] < target){
        i++;
    }
    return i;
}
```


### 优化思路

二分查找 时间复杂度 O(lgn)

```
int searchInsert(int* nums, int numsSize, int target)
{
    int left = 0, middle = 0;
    int right = numsSize - 1;
    
    while (left <= right)
    {
        middle = left + (right - left) / 2;
        if (nums[middle] == target) return middle;
        else if (nums[middle] < target) {
            left = middle + 1;
        }
        else {
            right = middle - 1;
        }
    }
    // 返回 left: nums =[1, 2, 4, 6] target = 7
    return left;
}
```