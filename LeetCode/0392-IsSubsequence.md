

```
def isSubsequence(s: str, t: str) -> bool:
	sl = len(s)
	tl = len(t)
	
	for i in range(tl - 1, -1, -1):
		print(i, sl)
		if t[i] == s[sl - 1]:
			print(t[i])
			sl -= 1
		if sl <= 0:
			return True
	return False
```