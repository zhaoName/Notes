
# 441. Arranging Coins

## 题目

```
You have n coins and you want to build a staircase with these coins. 
The staircase consists of k rows where the ith row has exactly i coins. 
The last row of the staircase may be incomplete.

Given the integer n, return the number of complete rows of the staircase you will build.

Input: n = 8
Output: 3
Explanation: Because the 4th row is incomplete, we return 3.

Constraints: 1 <= n <= 231 - 1
```


<br>

## 解法

### Python

```
def arrangeCoins(n: int) -> int:
        n_half = int(n * 2)
        n_sqrt = math.floor(n ** 0.5)
        
        for i in range(n_sqrt, n_half):
            if i ** 2 + i == 2 * n:
                return i
            if i ** 2 + i > 2 * n:
                return i - 1
        return 0
```