
# 选择排序

选择排序(Selection sort)是一种简单直观的排序算法，无论什么数据进去都是 O(n²) 的时间复杂度。所以用到它的时候，数据规模越小越好。选择排序相比较冒泡来说，交换次数较少，所以比冒泡排序快。选择排序是不稳定的排序方法。


### 算法原理

- 首先在未排序的数据中找到最小(大)值，将其放在起始位置。

- 再从剩余未排序的数据中继续寻找最小（大）元素，然后放到已排序序列的末尾。

- 重复第二步，直到所有元素均排序完毕。
 

### 动画展示

![](https://images.gitee.com/uploads/images/2019/0816/135216_65e5d593_1355277.gif "selectionSort.gif")


### OC 代码实现

```
- (void)selectionSort:(NSMutableArray *)arr
{
    for (int i=0; i<arr.count - 1; i++)
    {
        int minIndex = i;
        for (int j=i; j<arr.count - 1; j++) {
            
            if ([arr[minIndex] intValue] > [arr[j+1] intValue]) {
                minIndex = j+1;
            }
        }
        if (i != minIndex) [arr exchangeObjectAtIndex:i withObjectAtIndex:minIndex];
        [self printfArr:arr];
    }
}

// 打印结果
2019-08-16 14:59:59.213768+0800 Sort[64278:1041371] 1 10 5 15 6 12 9
2019-08-16 14:59:59.214003+0800 Sort[64278:1041371] 1 5 10 15 6 12 9
2019-08-16 14:59:59.214150+0800 Sort[64278:1041371] 1 5 6 15 10 12 9
2019-08-16 14:59:59.214459+0800 Sort[64278:1041371] 1 5 6 9 10 12 15
2019-08-16 14:59:59.214600+0800 Sort[64278:1041371] 1 5 6 9 10 12 15
2019-08-16 14:59:59.214722+0800 Sort[64278:1041371] 1 5 6 9 10 12 15
```


### C 代码实现

```
void c_selectionSort(int arr[], int len)
{
    for (int i=0; i< len - 1; i++)
    {
        int minIndex = i;
        for (int j=i; j< len - 1; j++) {
            
            if (arr[minIndex] > arr[j+1]) {
                minIndex = j+1;
            }
        }
        if (i != minIndex) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
        c_printfArr(arr, len);
    }
}

// 打印结果
1 10 5 15 6 12 9 
1 5 10 15 6 12 9 
1 5 6 15 10 12 9 
1 5 6 9 10 12 15 
1 5 6 9 10 12 15 
1 5 6 9 10 12 15 
```


<br>

**参考：**[选择排序](https://github.com/hustcc/JS-Sorting-Algorithm/blob/master/2.selectionSort.md)

写于2019-08-16

<br>