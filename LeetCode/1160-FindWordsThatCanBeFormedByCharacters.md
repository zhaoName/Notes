# 1160. Find Words That Can Be Formed by Characters

## 题目

```
You are given an array of strings words and a string chars.
A string is good if it can be formed by characters from chars (each character can only be used once).
Return the sum of lengths of all good strings in words.

Input: words = ["cat","bt","hat","tree"], chars = "atach"
Output: 6
Explanation: 
The strings that can be formed are "cat" and "hat" so the answer is 3 + 3 = 6.

Note:
1 <= words.length <= 1000
1 <= words[i].length, chars.length <= 100
All strings contain lowercase English letters only.
```

<br>

## 解法

### Pyhton

- 第一思路

```
## Time:O(max(len(chars), n))  n 为 words 每个字母的长度
## Space: O(26)
def countCharacters(self, words: list, chars: str) -> int:
	if not chars or not words: return 0

	res, charsD = 0, {}
	for a in chars:
		try:
			charsD[a] += 1
		except:
			charsD[a] = 1
	for i in range(len(words)):
		tempD, j, end = charsD.copy(), 0, False
		for j in range(len(words[i])):
			if words[i][j] in tempD:
				if tempD[words[i][j]] == 0:
					end = True
					break
				tempD[words[i][j]] -= 1
			else:
				end = True
				break
		## 注意 words = ["aa", "x"]  chars = "zay" 两种情况
		if j == len(words[i]) - 1 and not end:
			res += len(words[i])
	return res
```

- 借鉴思路   collections.Counter

```
def countCharacters_(self, words: list, chars: str) -> int:
	res = 0
	cnt = collections.Counter(chars)
	print(cnt)
	for w in words:
		wnt = collections.Counter(w)
		print(wnt)
		if all(wnt[i] <= cnt[i] for i in wnt):
			res += len(w)
	return res
```
