
# 插入排序

插入排序是一种最简单直观的排序算法，它的工作原理是通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入。时间复杂度为 O(n²) 。

插入排序和冒泡排序一样，也有一种优化算法，叫做拆半插入。

### 算法原理

- 将待排序的序列的第一个元素看成有序序列，将第二个元素到最后一个元素当成未排序序列。

- 从头到尾依次扫描未排序序列，将扫描到的每个元素插入有序序列的适当位置(注意结束条件和最小值)。


### 动画展示

![](https://images.gitee.com/uploads/images/2019/0816/175428_f719872a_1355277.gif "insertionSort.gif")

### OC 代码实现

```
- (void)insertionSort:(NSMutableArray *)arr
{
    for (int i=1; i<arr.count; i++)
    {
        id temp = arr[i];
        for (int j=i-1; j>=0; j--) {
            
            if ([arr[j] intValue] < [temp intValue]) {
                arr[j+1] = temp;
                break;
            }
            else {
                arr[j+1] = arr[j];
                // 若比较到 j=0, 说明此时temp最小
                if (j==0 ) arr[j] = temp;
            }
        }
        [self printfArr:arr];
    }
}

// 打印结果
2019-08-16 17:29:22.664543+0800 Sort[67298:1121473] 5 10 1 15 6 12 9
2019-08-16 17:29:22.664766+0800 Sort[67298:1121473] 1 5 10 15 6 12 9
2019-08-16 17:29:22.664896+0800 Sort[67298:1121473] 1 5 10 15 6 12 9
2019-08-16 17:29:22.665018+0800 Sort[67298:1121473] 1 5 6 10 15 12 9
2019-08-16 17:29:22.665133+0800 Sort[67298:1121473] 1 5 6 10 12 15 9
2019-08-16 17:29:22.665289+0800 Sort[67298:1121473] 1 5 6 9 10 12 15
```

这样写有点麻烦，要考虑结束条件和最小值。用`while`对其优化

```
- (void)insertionSort:(NSMutableArray *)arr
{
    for (int i=1; i<arr.count; i++)
    {
        id temp = arr[i];
        int j = i-1;
        // 先判断 j >= 0，再取数组元素
        while (j >= 0 && ([arr[j] intValue] > [temp intValue])) {
            arr[j+1] = arr[j];
            j--;
        }
        // while最后有 j--，跳出 while 循环时，若此时temp值最小 则j=-1
        // 执行下局代码会将最小值 放在序列的最左边
        arr[j+1] = temp;
        [self printfArr:arr];
    }
}

// 打印结果
2019-08-16 17:32:41.778675+0800 Sort[67374:1124139] 5 10 1 15 6 12 9
2019-08-16 17:32:41.778900+0800 Sort[67374:1124139] 1 5 10 15 6 12 9
2019-08-16 17:32:41.779036+0800 Sort[67374:1124139] 1 5 10 15 6 12 9
2019-08-16 17:32:41.779163+0800 Sort[67374:1124139] 1 5 6 10 15 12 9
2019-08-16 17:32:41.779286+0800 Sort[67374:1124139] 1 5 6 10 12 15 9
2019-08-16 17:32:41.779402+0800 Sort[67374:1124139] 1 5 6 9 10 12 15
```


### C 代码实现

```
void c_insertionSort(int arr[], int len)
{
    for (int i=1; i<len; i++)
    {
        int temp = arr[i];
        int j = i-1;
        while (j>=0 && arr[j] > temp) {
            arr[j+1] = arr[j];
            j--;
        }
        
        arr[j + 1] = temp;
        c_printfArr(arr, len);
    }
}

// 打印结果
5 10 1 15 6 12 9 
1 5 10 15 6 12 9 
1 5 10 15 6 12 9 
1 5 6 10 15 12 9 
1 5 6 10 12 15 9 
 6 9 10 12 15 
```


<br>

**参考：**[插入排序](https://github.com/hustcc/JS-Sorting-Algorithm/blob/master/3.insertionSort.md)

写于2019-08-16

	<br>