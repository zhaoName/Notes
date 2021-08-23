
# 计数排序

<br>

计数排序的核心在于将输入的数据值转化为键存储在额外开辟的数组空间中。作为一种线性时间复杂度的排序，计数排序要求输入的数据必须是有确定范围的整数。如高考时对全省考生的分数(0-100)排序。

### 算法原理

- 遍历待排序数据arr[n]，使用计数数组counting[max+1]，对每一个arr[n]中出现的元素进行计数;

- 扫描计数数组counting[max+1]，还原arr[n]，排序结束;


### 动画展示

![](../Images/Sort/countingSort.gif)


### OC 代码实现

```
- (void)countingSort:(NSMutableArray *)arr
{
    // 若已知最大值 这步可省略
    int max = [self getMaxValue:arr];
    NSMutableArray *countings = [NSMutableArray arrayWithCapacity:max + 1];
    for (int i=0; i<max+1; i++) {
        [countings addObject:@0];
    }
    
    for (int i=0; i<arr.count; i++) {
        int value = [arr[i] intValue];
        // countings 中的下标是 arr 值， countings 中的值是 arr 中元素出现的次数
        countings[value] = @([countings[value] intValue] + 1);
    }
    
    int index = 0;
    for (int i=0; i<countings.count; i++)
    {
        // 取出 countings 的值，也就是 arr 对应元素 i 出现的次数
        int valueCount = [countings[i] intValue];
        while (valueCount > 0) {
            arr[index++] = @(i);
            valueCount--;
        }
        [self printfArr:arr];
    }
}

/// 最大值
- (int)getMaxValue:(NSMutableArray *)arr
{
    int maxValue = 0;
    for (int i=0; i<arr.count-1; i++) {
        if (maxValue < [arr[i] intValue]) {
            maxValue = [arr[i] intValue];
        }
    }
    return maxValue;
}

// 打印结果
2019-08-24 18:46:17.532032+0800 Sort[24765:2745593] 2 3 8 7 1 2 2 2 7 3 9 8 2 1 4 6 7
2019-08-24 18:46:17.532275+0800 Sort[24765:2745593] 1 1 8 7 1 2 2 2 7 3 9 8 2 1 4 6 7
2019-08-24 18:46:17.532437+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 2 7 3 9 8 2 1 4 6 7
2019-08-24 18:46:17.532576+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 3 3 3 9 8 2 1 4 6 7
2019-08-24 18:46:17.532734+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 3 3 4 9 8 2 1 4 6 7
2019-08-24 18:46:17.532904+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 3 3 4 9 8 2 1 4 6 7
2019-08-24 18:46:17.533052+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 3 3 4 6 8 2 1 4 6 7
2019-08-24 18:46:17.533191+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 3 3 4 6 7 7 7 4 6 7
2019-08-24 18:46:17.533319+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 3 3 4 6 7 7 7 8 8 7
2019-08-24 18:46:17.533448+0800 Sort[24765:2745593] 1 1 2 2 2 2 2 3 3 4 6 7 7 7 8 8 9
```

### C 代码实现

```
void c_countingsSort(int arr[], int len)
{
    int *countings = malloc(10 * sizeof(int));
    // 初始化
    memset(countings, 0, 10 * sizeof(int));
    
    for (int i=0; i<len; i++) {
        countings[arr[i]]++;
    }
    
    int index = 0;
    for (int i=0; i<10; i++)
    {
        while (countings[i] > 0) {
            arr[index++] = i;
            countings[i]--;
        }
        c_printfArr(arr, len);
    }
}

// 打印结果
2 3 8 7 1 2 2 2 7 3 9 8 2 1 4 6 7 
1 1 8 7 1 2 2 2 7 3 9 8 2 1 4 6 7 
1 1 2 2 2 2 2 2 7 3 9 8 2 1 4 6 7 
1 1 2 2 2 2 2 3 3 3 9 8 2 1 4 6 7 
1 1 2 2 2 2 2 3 3 4 9 8 2 1 4 6 7 
1 1 2 2 2 2 2 3 3 4 9 8 2 1 4 6 7 
1 1 2 2 2 2 2 3 3 4 6 8 2 1 4 6 7 
1 1 2 2 2 2 2 3 3 4 6 7 7 7 4 6 7 
1 1 2 2 2 2 2 3 3 4 6 7 7 7 8 8 7 
1 1 2 2 2 2 2 3 3 4 6 7 7 7 8 8 9 
```


<br>

**参考：**  [Counting Sort](https://github.com/hustcc/JS-Sorting-Algorithm/blob/master/8.countingSort.md)


写于2019-08-24

<br>

