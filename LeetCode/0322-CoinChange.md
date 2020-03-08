# 322. Coin Change


## 题目

```
You are given coins of different denominations and a total amount of money amount.
Write a function to compute the fewest number of coins that you need to make up that amount. 
If that amount of money cannot be made up by any combination of the coins, return -1.

Input: coins = [1, 2, 5], amount = 11
Output: 3 
Explanation: 11 = 5 + 5 + 1

Note:
You may assume that you have an infinite number of each kind of coin.
```

<br>

## 解法

### Python 

- 借鉴思路 动态规划 自下而上

```
def coinChange_dp(self, coins: list, amount: int):
    if len(coins) == 0: return -1
    ## memo[n]的值： 表示的凑成总金额为n所需的最少的硬币个数
    memo = [0] * (amount + 1)
    for i in range(1, amount + 1):
    	min = 1000000000
    	for j in range(len(coins)):
    		if i - coins[j] >= 0 and memo[i - coins[j]] < min:
    			print(i - coins[j])
    			min = memo[i - coins[j]] + 1
    	memo[i] = min
    	print(memo)
    return -1 if memo[amount] == 1000000000 else memo[amount]
```