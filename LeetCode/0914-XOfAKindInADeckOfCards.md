
# 914. X of a Kind in a Deck of Cards


## 题目

```
In a deck of cards, each card has an integer written on it.
Return true if and only if you can choose X >= 2 such that it is possible to split the entire deck into 1 or more groups of cards, 
where:
- Each group has exactly X cards.
- All the cards in each group have the same integer.

Input: deck = [1,1,2,2,2,2]
Output: true
Explanation: Possible partition [1,1],[2,2],[2,2].

Input: deck = [1]
Output: false
Explanation: No possible partition.

Constraints:
1 <= deck.length <= 10^4
0 <= deck[i] < 10^4
```

<br>

## 解法

### Python

- 第一思路

```
def hasGroupsSizeX(self, deck: list) -> bool:
	if len(deck) < 2: return False
	g, d = -1, {}
	for a in deck:
		try:
			d[a] += 1
		except:
			d[a] = 1
	for a in d.values():
		if g == -1:
			g = a
		else:
			g = gcd(g, a)
	return g >= 2
```

- 借鉴思路  reduce()

```
def hasGroupsSizeX_reduce(self, deck: list) -> bool:
	if len(deck) < 2: return False
	minLen, d = 0, {}
	for a in deck:
		try:
			d[a] += 1
		except:
			d[a] = 1
	return reduce(gcd, d.values()) >= 2
```