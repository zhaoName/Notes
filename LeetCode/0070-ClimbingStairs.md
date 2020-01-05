# 70. Climbing Stairs


## 题目

```
You are climbing a stair case. It takes n steps to reach to the top.

Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

Note: Given n will be a positive integer
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