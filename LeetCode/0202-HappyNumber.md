
# 202. Happy Number


## 题目


```
Write an algorithm to determine if a number is "happy".

A happy number is a number defined by the following process: Starting with any positive integer, 
replace the number by the sum of the squares of its digits, 
and repeat the process until the number equals 1 (where it will stay), 
or it loops endlessly in a cycle which does not include 1. 
Those numbers for which this process ends in 1 are happy numbers.

Example: 
Input: 19
Output: true
Explanation: 
12 + 92 = 82
82 + 22 = 68
62 + 82 = 100
12 + 02 + 02 = 1
```


<br>


## 解法

### Python


- 第一思路(这个思路虽提交成功  但不算答案)

```
def isHappy(self, n: int) -> bool:
    num = 0
    while n != 1:
        if num > 100:
            return False

        sum = 0
        while n > 0:
            sum += (n % 10) ** 2
            n = n // 10

        n = sum
        num += 1

    return True
```


- 优化思路 - 快慢指针

```
def isHappy_pointer_sum(self, n) -> int:
    sum = 0
    while n > 0:
        sum += (n % 10) ** 2
        n = n // 10
    return sum

## 快慢指针法
## 快指针每次走两步  满指针每次走一步
## 然后判断是否是因为 1 引起的循环
def isHappy_pointer(self, n) -> bool:
    slow, fast = n, 0
    while slow != fast:
        slow = self.isHappy_pointer_sum(slow)
        fast = self.isHappy_pointer_sum(n if fast == 0 else fast)
        fast = self.isHappy_pointer_sum(fast)
        print(slow, fast)

    return slow == 1
```

- 优化思路 - 所有非快乐数都会收敛到4 

```
## 所有非快乐数的数位平方和计算，最后都会进入 4 → 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4 的循环中
def isHappy_shoulian4(self, n) -> bool:
    while True:
        n = sum(int(i) ** 2 for i in str(n))
        if n == 4:
            return False
        if n == 1:
            return True
```

<br>

### C


```
/**
 * 202. Happy Number
 *
 * 所有不快乐数的数位平方和计算，最后都会进入 4 → 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4 的循环中
 */
bool isHappy(int n)
{
   while (1)
   {
      int sum = 0;
      while(n > 0) {
         sum += ((n % 10) * (n % 10));
         n = n / 10;
      }
      n = sum;
      if (n == 4) return false;
      if (n == 1) return true;
   }
   return false;
}
```



