
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