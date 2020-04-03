# 8. String to Integer (atoi)


## 题目

```
Implement atoi which converts a string to an integer.
The function first discards as many whitespace characters as necessary until the first non-whitespace character is found. 
Then, starting from this character, takes an optional initial plus or minus sign followed by as many numerical digits as possible, and interprets them as a numerical value.
The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.
If the first sequence of non-whitespace characters in str is not a valid integral number, 
or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.
If no valid conversion could be performed, a zero value is returned.

Note:
Only the space character ' ' is considered as whitespace character.
Assume we are dealing with an environment which could only store integers within the 32-bit signed integer range: [−2^31,  2^31 − 1]. 
If the numerical value is out of the range of representable values, INT_MAX (2^31 − 1) or INT_MIN (−2^31) is returned.
```

<br>

## 解法

### Python

- 第一思路

```
def myAtoi(self, str: str) -> int:
    	if not str: return 0
    	res = ''
    	for a in str:
    		## str 开头是 ' '
    		if len(res) == 0 and ord(a) == 32:
    			continue
    		## 最先遍历到的是 '-' 或 '+'
    		if len(res) == 0 and (ord(a) == 45 or ord(a) == 43):
    			res += a
    			continue
    		## 如果是数字
    		if ord(a) >= 48 and ord(a) <= 57:
    			res += a
    		## 若 res 有值且当前遍历到的字符不是数字
    		if ord(a) < 48 or ord(a) > 57:
    			break
    	## res 为空，后者 res 只有一个 -/+  都不能构成有效数字
    	if len(res) == 0 or  res == '-' or res == '+':
    		return 0
    	return min(max(int(res), -2147483648), 2147483647)
```

- 借鉴思路 自动机

```
class Automaton:
	def __init__(self):
		self.state = 'start'
		self.sign = 1
		self.ans = 0
		self.table = {
            'start':['start', 'signed', 'in_number', 'end'],
            'signed':['end', 'end', 'in_number', 'end'],
            'in_number': ['end', 'end', 'in_number', 'end'],
            'end': ['end', 'end', 'end', 'end']}


	def get_col(self, c):
		if c.isspace():
			return 0
		if c == '+' or c =='-':
			return 1
		if c.isdigit():
			return 2
		return 3

	def get(self, c):
		self.state = self.table[self.state][self.get_col(c)]
		if self.state == 'in_number':
			self.ans = self.ans * 10 + int(c)
			self.ans = min(max(self.ans, -2147483648), 2147483647)
		elif self.state == 'signed':
			self.sign = 1 if c == '+' else -1
			
			
class Solution:
    def myAtoi_auto(self, str: str) -> int:
    	auto = Automaton()
    	for a in str:
    		print('---')
    		auto.get(a)
    	return auto.sign * auto.ans
```