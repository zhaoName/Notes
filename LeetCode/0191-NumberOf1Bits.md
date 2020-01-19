
# 191. Number of 1 Bits


## 题目


```
Write a function that takes an unsigned integer and return the number of '1' bits it has (also known as the Hamming weight).

Example 1:
Input: 00000000000000000000000000001011
Output: 3
Explanation: The input binary string 00000000000000000000000000001011 has a total of three '1' bits.

Note:
- Note that in some languages such as Java, there is no unsigned integer type. In this case, 
the input will be given as signed integer type and should not affect your implementation, 
as the internal binary representation of the integer is the same whether it is signed or unsigned.

- In Java, the compiler represents the signed integers using 2's complement notation. 
Therefore, in Example 3 above the input represents the signed integer -3.
```

<br>


## 解法


### C

- 第一思路

```
int hammingWeight(uint32_t n)
{
    int sum = 0;
    for (int i=0; i<32; i++) {
       sum += n >> i & 1;
    }
    return sum;
}
```

<br>


### Python

- 类似上面 C 的写法 会报错！！

```
def hammingWeight(self, n: int) -> int:
	sum = 0
	while n:
		sum += n & 1
		n >>= 1
	return sum
```

- 优化思路 转成二进制

```
def hammingWeight_bin(self, n) -> int:
	return bin(n).count('1')
```