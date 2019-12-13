
# 27. Remove Element

## C

### 第一思路

```
int removeElement(int* nums, int numsSize, int val)
{
    int delNum = 0;
    for (int i=0; i<numsSize; i++)
    {
        if (nums[i] == val) {
            delNum++;
        }
        else {
            nums[i-delNum] = nums[i];
        }
    }
    return numsSize - delNum;
}
```

<br>

### 官网优化解法


当我们遇到`nums[i] = val` 时，我们可以将当前元素与最后一个元素进行交换，并释放最后一个元素。这实际上使数组的大小减少了 1。

这就可以解释为什么题目中会有数组中元素顺序可变的这一条提示了！！

```
int removeElement(int* nums, int numsSize, int val)
{
    int delNum = 0;
    while (delNum < numsSize)
    {
        if (nums[delNum] == val) {
            nums[delNum] = nums[numsSize-1];
            numsSize--;
        }
        else {
            delNum++;
        }
    }
    return numsSize;
}
```








<br>

写于 2019-12-13

<br>

