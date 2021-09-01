# 70. Climbing Stairs


## 题目

```
You are climbing a stair case. It takes n steps to reach to the top.
Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

Input: n = 3
Output: 3
Explanation: There are three ways to climb to the top.
1. 1 step + 1 step + 1 step
2. 1 step + 2 steps
3. 2 steps + 1 step

Constraints:
1 <= n <= 45
```

<br>


## 解法


### C

```
/**
 * 70. Climbing Stairs
 *
 * 递归解法：对于 n 个阶梯的走法 = 第一步迈一个阶梯的走法 + 第一步迈两个阶梯的走法
 * 即 f(n) = f(n-1) + f(n-2) 类似求斐波那锲数列的第 n 个值
 * 但使用递归 时间复杂度太高 O(2^n)
 */
int climbStairs(int n)
{
    if (n <=2) return n;
    return climbStairs(n-1) + climbStairs(n-2);
}
```

```
/**
 * 时间复杂度: O(n) 空间复杂度：O(1)
 */
int climbStairs_1(int n)
{
   if (n==1) return 1;
   
   int s1 = 1;
   int s2 = 2;
   for (int i=3; i<=n; i++) {
      int s3 = s1 + s2;
      s1 = s2;
      s2 = s3;
   }
   return s2;
}
```

<br>

### 二刷

```python3
## dp数组
def climbStairs_dp(self, n: int) -> int:
    if n == 1 or n == 2: return n
    dp = [1, 2]
    for i in range(2, n):
        dp.append((dp[i-2] + dp[i-1]))
    return dp.pop()

def climbStairs(self, n: int) -> int:
    if n == 1 or n == 2: return n
    s1, s2 = 1, 2
    for i in range(2, n):
        temp = s2
        s2 = s1 + s2
        s1 = temp
    return s2
```