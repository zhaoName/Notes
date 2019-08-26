
# 基数排序

<br>

基数排序 (Radix Sort) 是一种非比较型整数排序算法，其原理是将整数按位数切割成不同的数字，然后按每个位数分别比较。由于整数也可以表达字符串（比如名字或日期）和特定格式的浮点数，所以基数排序也不是只能使用于整数。基数排序的发明可以追溯到1887年赫尔曼·何乐礼在打孔卡片制表机（Tabulation Machine）上的贡献。实际复杂度为`O(n*k)`


### 算法原理

- 找出所给数据的最大值，最大值的位数就是外层循环的次数即 k

- 从最低位开始，遍历所有数据，将所有最低位相同的数据放入到一个桶中，再按[0, 9]放回原数组中

- 从次低位遍历数据，重复步骤二。知道最高位排序完成后，数据就变成一个有序序列。

### 动画展示


![](https://images.gitee.com/uploads/images/2019/0826/163124_dddfa886_1355277.gif "radixSort.gif")


### OC 代码实现

```
- (void)radixSort:(NSMutableArray *)arr
{
    if (arr.count < 2) return;
    
    // 找出最大值
    int max = [arr[0] intValue];
    for (int i=0; i<arr.count; i++) {
        if (max < [arr[i] intValue]) {
            max = [arr[i] intValue];
        }
    }
    
    // 最大数的最高位
    int maxDigit = 1;
    int temp = 10;
    while (floor(max / temp)) {
        temp *= 10;
        maxDigit++;
    }
    
    int mod = 1;
    for (int i=0; i<maxDigit; i++)
    {
        // 创建桶并初始化 数字都在 [0, 9] 之间
        NSMutableArray *buckets = [NSMutableArray arrayWithCapacity:10];
        for (int i=0; i<10; i++) {
            [buckets addObject:[NSMutableArray array]];
        }
        // 按位分隔 将数据放入对应的桶中
        for (int j=0; j<arr.count; j++) {
            int index = [arr[j] intValue] / mod % 10;
            [buckets[index] addObject:arr[j]];
        }
        // 将桶中的数据放回数组中
        [arr removeAllObjects];
        for (int a=0; a<buckets.count; a++) {
            [arr addObjectsFromArray:buckets[a]];
        }
        
        mod *= 10;
        [self printfArr:arr];
    }
}

// 打印结果
2019-08-26 16:00:48.128501+0800 Sort[34158:3049529] 0 10 100 51 141 121 181 63 13 194 156 156 157 47 157 117 67 28 189 139 109
2019-08-26 16:00:48.128785+0800 Sort[34158:3049529] 0 100 109 10 13 117 121 28 139 141 47 51 156 156 157 157 63 67 181 189 194
2019-08-26 16:00:48.128979+0800 Sort[34158:3049529] 0 10 13 28 47 51 63 67 100 109 117 121 139 141 156 156 157 157 181 189 194
```

### C 代码实现

```
void c_radixSort(int arr[], int len)
{
    int i, b[len], m = arr[0], exp = 1;
    
    for (i = 1; i < len; i++) {
        if (arr[i] > m) {
            m = arr[i];
        }
    }
    
    while (m / exp > 0) {
        int bucket[10] = { 0 };
        
        for (i = 0; i < len; i++) {
            bucket[(arr[i] / exp) % 10]++;
        }
        
        for (i = 1; i < 10; i++) {
            bucket[i] += bucket[i - 1];
        }
        
        for (i = len - 1; i >= 0; i--) {
            b[--bucket[(arr[i] / exp) % 10]] = arr[i];
        }
        
        for (i = 0; i < len; i++) {
            arr[i] = b[i];
        }
        
        exp *= 10;
        c_printfArr(arr, len);
    }
}

// 打印结果
10 1 12 22 5 15 6 8 9 19 
1 5 6 8 9 10 12 15 19 22 
```


<br>

**参考：**

- [基数排序](https://zh.wikipedia.org/wiki/%E5%9F%BA%E6%95%B0%E6%8E%92%E5%BA%8F)

- [Radix Sort](https://github.com/hustcc/JS-Sorting-Algorithm/blob/master/10.radixSort.md)

写于2019-08-26

<br>