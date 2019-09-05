
# 快速排序

<br>

快速排序 (Quick Sort) 是对冒泡排序的改进，由C. A. R. Hoare在1960年提出。它的基本思想是：通过一趟排序将要排序的数据分割成独立的两部分，其中一部分的所有数据都比另外一部分的所有数据都要小，然后再按此方法对这两部分数据分别进行快速排序，整个排序过程可以递归进行，以此达到整个数据变成有序序列。

### 算法原理

- 先取一个元素当做分界值(一般选数组的第一个元素)，通过该分界值将数组分成左右两部分。

- 将大于或等于分界值的数据集中到数组右边，小于分界值的数据集中到数组的左边。此时分界值位于数组的中间位置。

- 然后将左边、右边的数组独立排序，重复上述步骤。

- 可以看出，这是一个递归定义。通过递归将左侧部分排好序后，再递归排好右侧部分的顺序。当左、右两个部分各数据排序完成后，整个数组的排序也就完成了。


### 动画展示

![](https://images.gitee.com/uploads/images/2019/0822/112432_a1eff311_1355277.gif "quickSort.gif")


### OC 代码实现

```
- (void)quickSort:(NSMutableArray *)arr left:(int)left right:(int)right
{
    if (left >= right) return;

    id key = arr[left];
    int i = left, j = right;

    while (i < j)
    {
        while (i<j && [arr[j] intValue] >= [key intValue]) {
            j--;
        }
        arr[i] = arr[j];


        while (i<j  && [arr[i] intValue] <= [key intValue]) {
            i++;
        }
        arr[j] = arr[i];
        
        NSLog(@"i=%d--------j=%d", i, j);
        [self printfArr:arr];
    }
    NSLog(@"************i=%d--------j=%d***********", i, j);
    // 将分界值 赋值给中间元素
    arr[i] = key;
    [self quickSort:arr left:left right:i];
    [self quickSort:arr left:i+1 right:right];
}

// 打印结果
2019-08-22 11:50:47.828071+0800 Sort[5111:2207468] i=1--------j=2
2019-08-22 11:50:47.828287+0800 Sort[5111:2207468] 1 10 10 15 6 12 9 8 22 19
2019-08-22 11:50:47.828402+0800 Sort[5111:2207468] i=1--------j=1
2019-08-22 11:50:47.828552+0800 Sort[5111:2207468] 1 10 10 15 6 12 9 8 22 19
2019-08-22 11:50:47.828645+0800 Sort[5111:2207468] ************i=1--------j=1***********
2019-08-22 11:50:47.828721+0800 Sort[5111:2207468] i=0--------j=0
2019-08-22 11:50:47.828843+0800 Sort[5111:2207468] 1 5 10 15 6 12 9 8 22 19
2019-08-22 11:50:47.828928+0800 Sort[5111:2207468] ************i=0--------j=0***********
2019-08-22 11:50:47.829003+0800 Sort[5111:2207468] i=3--------j=7
2019-08-22 11:50:47.829108+0800 Sort[5111:2207468] 1 5 8 15 6 12 9 15 22 19
2019-08-22 11:50:47.829224+0800 Sort[5111:2207468] i=5--------j=6
2019-08-22 11:50:47.829484+0800 Sort[5111:2207468] 1 5 8 9 6 12 12 15 22 19
2019-08-22 11:50:47.829652+0800 Sort[5111:2207468] i=5--------j=5
2019-08-22 11:50:47.829884+0800 Sort[5111:2207468] 1 5 8 9 6 12 12 15 22 19
2019-08-22 11:50:47.830069+0800 Sort[5111:2207468] ************i=5--------j=5***********
2019-08-22 11:50:47.830256+0800 Sort[5111:2207468] i=3--------j=4
2019-08-22 11:50:47.830475+0800 Sort[5111:2207468] 1 5 6 9 9 10 12 15 22 19
2019-08-22 11:50:47.830636+0800 Sort[5111:2207468] i=3--------j=3
2019-08-22 11:50:47.835032+0800 Sort[5111:2207468] 1 5 6 9 9 10 12 15 22 19
2019-08-22 11:50:47.835169+0800 Sort[5111:2207468] ************i=3--------j=3***********
...
```



### C 代码实现

```
void c_quickSort(int arr[], int left, int right)
{
    if (left >= right) return;
    
    int key = arr[left];
    int i = left, j = right;
    
    while (i < j)
    {
        while (i < j && arr[j] >= key) {
            j--;
        }
        arr[i] = arr[j];
        
        while (i < j && arr[i] <= key) {
            i++;
        }
        arr[j] = arr[i];
        
        printf("i=%d--------j=%d \n", i, j);
        c_printfArr(arr, 10);
    }
    printf("************i=%d--------j=%d***********\n", i, j);
    
    arr[i] = key;
    c_quickSort(arr, left, i);
    c_quickSort(arr, i+1, right);
}

// 打印结果
i=1--------j=2 
1 10 10 15 6 12 9 8 22 19 
i=1--------j=1 
1 10 10 15 6 12 9 8 22 19 
************i=1--------j=1***********
i=0--------j=0 
1 5 10 15 6 12 9 8 22 19 
************i=0--------j=0***********
i=3--------j=7 
1 5 8 15 6 12 9 15 22 19 
i=5--------j=6 
1 5 8 9 6 12 12 15 22 19 
i=5--------j=5 
1 5 8 9 6 12 12 15 22 19 
************i=5--------j=5***********
i=3--------j=4 
1 5 6 9 9 10 12 15 22 19 
i=3--------j=3 
1 5 6 9 9 10 12 15 22 19 
************i=3--------j=3***********
...
```


<br>

**参考：**

- [快速排序](https://baike.baidu.com/item/%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F%E7%AE%97%E6%B3%95/369842?fromtitle=%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F&fromid=2084344&fr=aladdin)

- [Quick Sort](https://github.com/hustcc/JS-Sorting-Algorithm/blob/master/6.quickSort.md)

写于2019-08-22

<br>