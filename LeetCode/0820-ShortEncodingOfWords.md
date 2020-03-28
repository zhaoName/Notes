# 820. Short Encoding of Words


## 题目

```
Given a list of words, we may encode it by writing a reference string S and a list of indexes A.
For example, if the list of words is ["time", "me", "bell"], we can write it as S = "time#bell#" and indexes = [0, 2, 5].
Then for each index, we will recover the word by reading from the reference string from that index until we reach a "#" character.
What is the length of the shortest reference string S possible that encodes the given words?

Input: words = ["time", "me", "bell"]
Output: 10
Explanation: S = "time#bell#" and indexes = [0, 2, 5].

Note:
1 <= words.length <= 2000.
1 <= words[i].length <= 7.
Each word has only lowercase letters.
```

<br>

## 解法

### Python

- 第一思路  

```
def minimumLengthEncoding_sort(self, words: list) -> int:
	words.sort(key = lambda a:len(a), reverse=True) 
	compress, i = words[0] + '#', 1
	while i < len(words):
		temp = words[i] + "#"
		if temp not in compress:
			compress += temp
		i += 1
	return len(compress)
```