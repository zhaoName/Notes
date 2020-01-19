
# 189. Rotate Array

## 题目


```
Given an array, rotate the array to the right by k steps, where k is non-negative.

Note:
- Try to come up as many solutions as you can, there are at least 3 different ways to solve this problem.
- Could you do it in-place with O(1) extra space?
```

<br>


## 解法

### Python

- 第一思路

```
## Time Complexity:O(n)  Space:O(1)
def rotate_insert(self, nums, k: int) -> None:
	if k % len(nums): return
	i = 0
	while i < (k % len(nums)):
		nums.insert(0, nums.pop())
		i += 1
```


```
## Time Complexity:O(n)  Space:O(n-k)
def rotate_whole(self, nums, k) -> None:
	if k % len(nums) == 0: return

	k = k % len(nums)
	ro = nums[len(nums)-k:]
	nums[k:] = nums[0:len(nums)-k]
	nums[0:k] = ro
```

- 优化思路 反转

```
# Time Complexity:O(n)  Space:O(1)
def rotate_reverse(self, nums, k) -> None:
	if k % len(nums) == 0: return

	k %= len(nums)
	nums[:] = nums[::-1]
	nums[:k] = nums[:k][::-1]
	nums[k:] = nums[:k][::-1]
```

```
def rotate_(self,nums, k) -> None:
	k %= len(nums)
	nums[:] = nums[-k:] + nums[:-k]
```

<br>


### C


```
/**
 * 0189. Rotate Array
 *
 * Time Complexity:O(n)  Space Complexity:O(n)
 */
void rotate(int* nums, int numsSize, int k)
{
    if (k % numsSize == 0) return;
    
    int *a = malloc(sizeof(int) * numsSize);
    for (int i=0; i<numsSize; i++) {
        a[(i+k) % numsSize] = nums[i];
    }
    
    for (int i=0; i<numsSize; i++) {
        nums[i] = a[i];
    }
}
```

- 优化思路   使用环状替换

```
/**
 * 把数组当成环形的，把每个元素放到其后K个位置
 *
 * Time Complexity:O(n)  Space Complexity:O(1)
 */
void rotate_1(int *nums, int numsSize, int k)
{
    if (k % numsSize == 0) return;
    
    k %= numsSize;
    int count = 0;
    for (int start=0; count<numsSize; start++)
    {
        int cur = start;
        int prev = nums[start];
        do {
            int next = (cur + k) % numsSize;
            int temp = nums[next];
            nums[next] = prev;
            prev = temp;
            cur = next;
            count++;
        } while (start != cur);
    }
}
```