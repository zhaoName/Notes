
# 归并排序

<br>

归并排序(Merger Sort)是建立在归并操作上的一种有效的排序算法,该算法是采用分治法(Divide and Conquer)的一个非常典型的应用。


### 算法原理

- 申请空间，使其大小为两个已经排序序列之和，该空间用来存放合并后的序列；

- 设定两个指针，最初位置分别为两个已经排序序列的起始位置；

- 比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置；

- 重复步骤 3 直到某一指针达到序列尾；

- 将另一序列剩下的所有元素直接复制到合并序列尾


### 动画展示

![](../Images/Sort/mergeSort.gif)


### OC 代码实现

```
- (NSMutableArray *)mergeSort:(NSMutableArray *)arr
{
    if (arr.count < 2) return arr;
    
    int middle = floorf(arr.count / 2.0);
    NSArray *lefts = [arr subarrayWithRange:NSMakeRange(0, middle)];
    NSArray *rights = [arr subarrayWithRange:NSMakeRange(middle, arr.count-middle)];
    
    return [self mergeLeft:[self mergeSort:[lefts mutableCopy]] right:[self mergeSort:[rights mutableCopy]]];
}

- (NSMutableArray *)mergeLeft:(NSMutableArray *)lefts right:(NSMutableArray *)rights
{
    NSMutableArray *results = [NSMutableArray array];
    //NSLog(@"%@----%@", lefts, rights);
    
    while (lefts.count && rights.count)
    {
        if ([lefts[0] intValue] < [rights[0] intValue]) {
            [results addObject:lefts[0]];
            [lefts removeObjectAtIndex:0];
        }
        else {
            [results addObject:rights[0]];
            [rights removeObjectAtIndex:0];
        }
    }
    
    while (lefts.count) {
        [results addObject:lefts[0]];
        [lefts removeObjectAtIndex:0];
    }
    
    while (rights.count) {
        [results addObject:rights[0]];
        [rights removeObjectAtIndex:0];
    }
    
    [self printfArr:results];
    return results;
}

// 打印结果
2019-08-20 14:44:08.298320+0800 Sort[80949:1595140] 5 10
2019-08-20 14:44:08.298501+0800 Sort[80949:1595140] 6 15
2019-08-20 14:44:08.298630+0800 Sort[80949:1595140] 1 6 15
2019-08-20 14:44:08.298963+0800 Sort[80949:1595140] 1 5 6 10 15
2019-08-20 14:44:08.299083+0800 Sort[80949:1595140] 9 12
2019-08-20 14:44:08.299205+0800 Sort[80949:1595140] 19 22
2019-08-20 14:44:08.299306+0800 Sort[80949:1595140] 8 19 22
2019-08-20 14:44:08.299413+0800 Sort[80949:1595140] 8 9 12 19 22
2019-08-20 14:44:08.299531+0800 Sort[80949:1595140] 1 5 6 8 9 10 12 15 19 22
```

<br>

**参考：**

- [iOS/OC:归并排序的图解和实现](https://www.jianshu.com/p/8fce5bfb0013)

- [Merge Sort](https://github.com/hustcc/JS-Sorting-Algorithm/blob/master/5.mergeSort.md)

写于2019-08-20

<br>