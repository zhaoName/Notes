# 345. Reverse Vowels of a String


## 题目

```
Write a function that takes a string as input and reverse only the vowels of a string.

Example 1:
Input: "hello"
Output: "holle"

Example 2:
Input: "leetcode"
Output: "leotcede"

Note:
The vowels does not include the letter "y".
```


<br>

## 解法


### Python

```
## Time Complexity: O(10*n)   Space Complexity:O(n)
def reverseVowels(self, s: str) -> str:
	vowels = ['a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U']
	letters = list(s)
	i , j = 0, len(s) -1
	while(i < j):
		while i<j and not s[i] in vowels:
			i += 1
		while i<j and not s[j] in vowels:
			j -= 1
		letters[i], letters[j] = letters[j], letters[i]
		i += 1
		j -= 1
	return ''.join(letters)
```