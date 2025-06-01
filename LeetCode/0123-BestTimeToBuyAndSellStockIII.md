# 123.Best Time to Buy and Sell Stock III

## 题目

```
给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。
设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易。
注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

示例 1:
输入：prices = [3,3,5,0,0,3,1,4]
输出：6
解释：在第 4 天（股票价格 = 0）的时候买入，在第 6 天（股票价格 = 3）的时候卖出，这笔交易所能获得利润 = 3-0 = 3 。
     随后，在第 7 天（股票价格 = 1）的时候买入，在第 8 天 （股票价格 = 4）的时候卖出，这笔交易所能获得利润 = 4-1 = 3 。

示例 2：
输入：prices = [1,2,3,4,5]
输出：4
解释：在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。   
     注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。   
     因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。

示例 3：
输入：prices = [7,6,4,3,1] 
输出：0 
解释：在这个情况下, 没有交易完成, 所以最大利润为 0。

示例 4：
输入：prices = [1]
输出：0

提示：
1 <= prices.length <= 105
0 <= prices[i] <= 105
```


## 解法

- 官方：状态转移

```python3
# buy1: 只进行一次买操作
# sell1: 完成一次买和卖操作
# buy2: 完成一笔交易的情况下，又买进行一次买操作
# sell2: 完成两笔交易
def maxProfit(self, prices: List[int]) -> int:
    if len(prices) <= 1: return 0

    buy1 = buy2 = -prices[0]
    sell1 = sell2 = 0
    for i in range(1, len(prices)):
        buy1 = max(buy1, -prices[i])
        sell1 = max(sell1, prices[i] + buy1)

        buy2 = max(buy2, sell1 - prices[i])
        sell2 = max(sell2, prices[i] + buy2)
        print(buy1, sell1, buy2, sell2)
    return sell2
```

- dp

```python3
def maxProfit_dp(self, prices: List[int]) -> int:
    if len(prices) == 0: return 0

    dp = [[0, 0, 0, 0] for _ in range(len(prices))]
    dp[0] = [-prices[0], 0, -prices[0], 0]

    for i in range(1, len(prices)):
        dp[i][0] = max(dp[i - 1][0], -prices[i])
        dp[i][1] = max(dp[i - 1][1], prices[i] + dp[i][0])

        dp[i][2] = max(dp[i - 1][2], dp[i][1] - prices[i])
        dp[i][3] = max(dp[i - 1][3], prices[i] + dp[i][2])
    return dp[-1][-1]
```