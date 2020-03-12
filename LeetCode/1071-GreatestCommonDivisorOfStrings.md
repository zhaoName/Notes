# 1071. Greatest Common Divisor of Strings


## 题目

```
For strings S and T, we say "T divides S" if and only if S = T + ... + T  (T concatenated with itself 1 or more times)
Return the largest string X such that X divides str1 and X divides str2.

Input: str1 = "ABCABC", str2 = "ABC"
Output: "ABC"

Note:
1 <= str1.length <= 1000
1 <= str2.length <= 1000
str1[i] and str2[i] are English uppercase letters.
```

<br>

## 解法

### Python

- 借鉴思路 辗转相除法

```
## Time Complexity:O(n)    Space Complexity:O(n)
def gcdOfStrings_gcd(self, str1: str, str2: str) -> str:
	## 辗转相除法算出最大公约数
	maxLen = math.gcd(len(str1), len(str2))
	maxStr = str1[:maxLen]
	## 整倍数
	if maxStr * (len(str1) // maxLen) == str1 and maxStr * (len(str2) // maxLen) == str2:
		return maxStr
	return ''
```

- 借鉴思路  str1 + str2 = str2 + str1

```
## Time Complexity:O(n)    Space Complexity:O(n)
def gcdOfStrings_(self, str1: str, str2: str) -> str:
	## 辗转相除法算出最大公约数
	maxLen = math.gcd(len(str1), len(str2))
	maxStr = str1[:maxLen]
	## 如果 str1 和 str2 拼接后等于 str2和 str1 拼接起来的字符串, 那么一定存在符合条件的字符串 X
	if str1 + str2 == str2 + str1:
		return maxStr
	return ''
```