# 121. Best Time to Buy and Sell Stock


## 题目

```
Say you have an array for which the ith element is the price of a given stock on day i.
If you were only permitted to complete at most one transaction (i.e., buy one and sell one share of the stock), 
design an algorithm to find the maximum profit.

Note that you cannot sell a stock before you buy one.


Input: [7,1,5,3,6,4]
Output: 5
Explanation: Buy on day 2 (price = 1) and sell on day 5 (price = 6), profit = 6-1 = 5.
             Not 7-1 = 6, as selling price needs to be larger than buying price.
```

<br>

## 解法

### Python

- 第一思路  不明白为哈比借鉴思路效率高

```
def maxProfit(self, prices: list) -> int:
	profit = 0
	for i in range(len(prices)):
		for j in range(i+1, len(prices)):
			## 前面数字比后面数字大，那最佳买入时间肯定不会是前面数字
			if prices[i] >= prices[j]:
				i = j
				break
			else:
				if prices[j] - prices[i] > profit:
					profit = prices[j] - prices[i]
	return profit
```


- 借鉴思路

```
def maxProfit_(self, prices: list) -> int:
	profit, min = 0, sys.maxsize
	for i in range(len(prices)):
		if prices[i] < min:
			min = prices[i]
		else:
			if prices[i] - min > profit:
				profit = prices[i] - min
	return profit
```