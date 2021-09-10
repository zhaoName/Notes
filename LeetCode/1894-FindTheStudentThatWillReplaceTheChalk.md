# 1894. Find the Student that Will Replace the Chalk

## 题目

```
There are n students in a class numbered from 0 to n - 1. The teacher will give each student a problem starting with the student number 0, then the student number 1, and so on until the teacher reaches the student number n - 1.
After that, the teacher will restart the process, starting with the student number 0 again.
You are given a 0-indexed integer array chalk and an integer k. There are initially k pieces of chalk. When the student number i is given a problem to solve,
they will use chalk[i] pieces of chalk to solve that problem. However, if the current number of chalk pieces is strictly less than chalk[i], then the student number i will be asked to replace the chalk.
Return the index of the student that will replace the chalk.

Example 1:
Input: chalk = [5,1,5], k = 22
Output: 0
Explanation: The students go in turns as follows:
- Student number 0 uses 5 chalk, so k = 17.
- Student number 1 uses 1 chalk, so k = 16.
- Student number 2 uses 5 chalk, so k = 11.
- Student number 0 uses 5 chalk, so k = 6.
- Student number 1 uses 1 chalk, so k = 5.
- Student number 2 uses 5 chalk, so k = 0.
Student number 0 does not have enough chalk, so they will have to replace it.

Constraints:
chalk.length == n
1 <= n <= 105
1 <= chalk[i] <= 105
1 <= k <= 109
```

<br>

## 解法

```python3
def chalkReplacer(self, chalk: List[int], k: int) -> int:
    if len(chalk) == 1: return 0

    sumChalk, l, ans = sum(chalk), len(chalk), 0
    n = k % sumChalk
    if n == 0 or chalk[0] > n: return 0

    dp = chalk[0]
    for i in range(1, l):
        dp += chalk[i % l]
        if dp > n:
            ans = i % l
            break
    return ans
```