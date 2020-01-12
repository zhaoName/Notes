
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

```

- 优化思路 - 

```

```