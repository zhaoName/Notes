# 122.Best Time To Buy and Sell Stock II

## 题目

```
You are given an integer array prices where prices[i] is the price of a given stock on the ith day.
On each day, you may decide to buy and/or sell the stock.
You can only hold at most one share of the stock at any time. However, you can buy it then immediately sell it on the same day.
Find and return the maximum profit you can achieve.

Example 1:
Input: prices = [7,1,5,3,6,4]
Output: 7
Explanation: Buy on day 2 (price = 1) and sell on day 3 (price = 5), profit = 5-1 = 4.
Then buy on day 4 (price = 3) and sell on day 5 (price = 6), profit = 6-3 = 3.
Total profit is 4 + 3 = 7.

Example 2:
Input: prices = [1,2,3,4,5]
Output: 4
Explanation: Buy on day 1 (price = 1) and sell on day 5 (price = 5), profit = 5-1 = 4.
Total profit is 4.

Example 3:
Input: prices = [7,6,4,3,1]
Output: 0
Explanation: There is no way to make a positive profit, so we never buy the stock to achieve the maximum profit of 0.


Constraints:
1 <= prices.length <= 3 * 10^4
0 <= prices[i] <= 10^4
```

## 解法

```python3
def maxProfit(self, prices: List[int]) -> int:
    if len(prices) == 0: return 0

    ans, pre, cur = 0, 0, 1
    while cur < len(prices):
        if prices[cur] - prices[pre] > 0:
            ans += (prices[cur] - prices[pre])
        pre = cur
        cur += 1
    return ans
```

- dp

```python3
# dp[i][0]: 第 i 天买后所得利润
# dp[i][1]: 第 i 天卖所得利润
def maxProfit_dp(self, prices: List[int]) -> int:
    if len(prices) == 0: return 0

    dp = [[0, 0] for _ in range(len(prices))]
    dp[0] = [-prices[0], 0]

    for i in range(1, len(prices)):
        # dp[i-1][1]是我之前卖股票赚的钱
        dp[i][0] = max(dp[i - 1][0], -prices[i] + dp[i - 1][1])
        # dp[i-1][0]是我用低价买入花的钱
        dp[i][1] = max(dp[i - 1][1], prices[i] + dp[i - 1][0])
    return dp[-1][-1]
```