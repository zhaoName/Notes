
# 69. Sqrt(x)

## 题目

```
Implement int sqrt(int x).

Compute and return the square root of x, where x is guaranteed to be a non-negative integer.

Since the return type is an integer, the decimal digits are truncated and only the integer part of the result is returned.
```

<br>


## 解法


### C

- 暴力遍历

```
// O(n^1/2)
int mySqrt(int x)
{
   if (x == 0 || x == 1) return x;
   
   long i = 2;
   for (i = 2; i <= x/2; i++) {
      if (i * i > x) {
         return i-1;
      }
   }
   return x/2;
}
```

- 折半查找

```
// O(logn)
int mySqrt(int x)
{
   if (x == 0 || x == 1) return x;
   
   long left = 2;
   long right = x/2;
   while (left <= right)
   {
      long middle = left + (right - left) / 2;
      if (middle * middle > x) {
         if ((middle-1) * (middle-1) <= x) return middle-1;
         right = middle - 1;
      }
      else if(middle * middle < x) {
         // 这里不能用 >=  因为返回的是 middle
         if ((middle+1) * (middle+1) > x) return middle;
         left = middle + 1;
      }
      else {
         return (int)middle;
      }
   }
   return x/2;
}
```

<br>

### Python

- 牛顿迭代法

```
def mySqrt(x):
    if x == 0:
        return 0
    cur = x
    while True:
        pre = cur
        cur = (cur + x / cur) / 2
        if abs(cur - pre) < 1e-6:
            return int(cur)
```


<br>


关于牛顿迭代法的原理可参考[这里](https://www.zhihu.com/question/20690553)

