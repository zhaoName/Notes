
# 190. Reverse Bits

<br>


## 题目

```
Reverse bits of a given 32 bits unsigned integer.

Example 1:
Input: 00000010100101000001111010011100
Output: 00111001011110000010100101000000
Explanation: The input binary string 00000010100101000001111010011100 represents the unsigned integer 43261596, 
so return 964176192 which its binary representation is 00111001011110000010100101000000.
```

<br>

## 解法

### Python

- 借用 python 自带函数

```
## unsigned int：0 ~ 4294967296
## 借助 python 自带函数
def reverseBits(self, n) -> int:
	re =  bin(n)[2:][::-1]
	re += '0' * (32 - len(re))
	#re = '0b' + re
	return int(re, 2)
```

- 位运算

```
## 位运算
def reverseBits_bit(self, n) -> int:
	re = ''
	for i in range(0, 32):
		re += str((n >> i) & 1)
	return int(re, 2)
```