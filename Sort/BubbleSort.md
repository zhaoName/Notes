
# 冒泡排序

冒泡排序(Bubble Sort)重复地走访过要排序的元素列，依次比较两个相邻的元素，如果他们的顺序（如从大到小、首字母从A到Z）错误就把他们交换过来。走访元素的工作是重复地进行直到没有相邻元素需要交换，也就是说该元素列已经排序完成。


### 算法原理

从小到大排序

- 比较相邻两个两个元素，若第一个元素比第二个元素大，则交换。

- 然后比较接下来的相邻两个元素，直到最后一对，这时最后面的元素是数组中的最大值。

- 从头开始再对剩下未排序元素重复上述步骤。

- 持续每次对越来越少的元素重复上面的步骤，直到没有任何一对数字需要比较。


### 动画演示

![](../Images/Sort/bubbleSort.gif)


### 最好情况

当要排序的数据已经是正序时，此时时间复杂度为O(1)

### 最坏情况

当要排序的数据已经是反序时，此时时间复杂度为O(n²)


### OC 代码实现

```
- (void)bubbleSort:(NSMutableArray *)arr
{
    for (int i=0; i<arr.count - 1; i++)
    {
        for (int j=0; j<arr.count-i-1; j++) {
            
            if ([arr[j] intValue] > [arr[j+1] intValue]) {
                [arr exchangeObjectAtIndex:j withObjectAtIndex:j+1];
            }
        }
        [self printfArr:arr];
    }
}

// 打印结果
2019-08-16 11:06:13.429013+0800 Sort[61110:955690] 5 1 10 6 12 9 15
2019-08-16 11:06:13.429221+0800 Sort[61110:955690] 1 5 6 10 9 12 15
2019-08-16 11:06:13.429332+0800 Sort[61110:955690] 1 5 6 9 10 12 15
2019-08-16 11:06:13.429481+0800 Sort[61110:955690] 1 5 6 9 10 12 15
2019-08-16 11:06:13.429586+0800 Sort[61110:955690] 1 5 6 9 10 12 15
2019-08-16 11:06:13.429686+0800 Sort[61110:955690] 1 5 6 9 10 12 15
```

### 优化

冒泡排序还有一种优化算法，就是立一个 flag，当在一趟序列遍历中元素没有发生交换，则证明该序列已经有序。但这种改进对于提升性能来说并没有什么太大作用。

```
- (void)bubbleSort:(NSMutableArray *)arr
{
    BOOL isOrder = NO;
    for (int i=0; i<arr.count - 1; i++)
    {
        for (int j=0; j<arr.count-i-1; j++) {
            isOrder = YES;
            if ([arr[j] intValue] > [arr[j+1] intValue]) {
                isOrder = NO;
                [arr exchangeObjectAtIndex:j withObjectAtIndex:j+1];
            }
        }
        if (isOrder) break;
        [self printfArr:arr];
    }
}

// 打印结果
2019-08-16 11:51:32.772089+0800 Sort[61949:981137] 5 1 10 6 12 9 15
2019-08-16 11:51:32.772323+0800 Sort[61949:981137] 1 5 6 10 9 12 15
2019-08-16 11:51:32.772470+0800 Sort[61949:981137] 1 5 6 9 10 12 15
```


### C 代码实现

```
void c_bubbleSort(int arr[], int len)
{
    for (int i=0; i<len-1; i++)
    {
        for (int j=0; j<len-1-i; j++) {
            
            if (arr[j] > arr[j+1]) {
                int temp = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
        }
        c_printfArr(arr, len);
    }
}

// 打印结果
5 1 10 6 12 9 15 
1 5 6 10 9 12 15 
1 5 6 9 10 12 15 
1 5 6 9 10 12 15 
1 5 6 9 10 12 15 
1 5 6 9 10 12 15 
```



<br>


参考： [bubbleSort](https://github.com/hustcc/JS-Sorting-Algorithm/blob/master/1.bubbleSort.md)

写于2019-08-16

<br>