
# 26. Remove Duplicates from Sorted Array

## 题目

```
Given a sorted array nums, remove the duplicates in-place such that each element appear only once and return the new length.

Do not allocate extra space for another array, you must do this by modifying the input array in-place with O(1) extra memory.
```

<br>

## 解法

### Python

- 第一思路  双指针

```
## Time Complexity:O(n)  Space Complexity:O(1)
def removeDuplicates_twoPointer(self, nums) -> int:
	i, j = 0, 1
	while j < len(nums):
		if nums[i] == nums[j]: 
			del nums[j]
		else:
			i = j
			j = j+ 1
	return len(nums)
```

- 第二思路 异或

```
## Time Complexity:O(n)  Space Complexity:O(1)
def removeDuplicates_XOR(self, nums) -> int:
	i = 1
	while i < len(nums):
		if nums[i-1]^nums[i] == 0:
			del nums[i]
		else:
			i += 1 
	return len(nums)
```

<br>


### C


```
/**
 * 26. Remove Duplicates from Sorted Array
 *
 * Time:O(n)  Space:O(1)
 */
int removeDuplicates(int* nums, int numsSize)
{
    if (numsSize == 0) return 0;
    
    int i = 0, j = 1;
    while(j < numsSize)
    {
        if (nums[i] != nums[j]) {
            if (j - i > 1) {
                nums[i+1] = nums[j];
            }
            i++;
        }
        j++;
    }
    return i+1;
}
```