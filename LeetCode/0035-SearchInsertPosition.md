
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
    if(numsSize==0) return 0;
    int low=0,high=numsSize-1,index=numsSize/2;
    while(low<high)
    {
        if(target==nums[index]) return index;
        if(target>nums[index]) low=index+1;
        else high=index-1;
        index=(high+low)/2;
    }
    if(target>nums[index]) return index+1;
    return index;
}
```