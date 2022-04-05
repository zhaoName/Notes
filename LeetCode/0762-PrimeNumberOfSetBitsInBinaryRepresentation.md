# 762. Prime Number of Set Bits in Binary Representation


## 题目

```
Given two integers left and right, return the count of numbers in the inclusive range [left, right] having a prime number of set bits in their binary representation.
Recall that the number of set bits an integer has is the number of 1's present when written in binary.
For example, 21 written in binary is 10101, which has 3 set bits.

Input: left = 10, right = 15
Output: 5
Explanation:
10 -> 1010 (2 set bits, 2 is prime)
11 -> 1011 (3 set bits, 3 is prime)
12 -> 1100 (2 set bits, 2 is prime)
13 -> 1101 (3 set bits, 3 is prime)
14 -> 1110 (3 set bits, 3 is prime)
15 -> 1111 (4 set bits, 4 is not prime)
5 numbers have a prime number of set bits.
```

<br>

## 解法

```python
## 二进制数 mask = 665772 = (10100010100010101100)2 来存储这些质数，
## 其中 mask 二进制的从低到高的第 i 位为 1 表示 i 是质数，为 0 表示 i 不是质数
## 设整数 x 的二进制中 1 的个数为 c，若 mask 按位与 2^c 不为 0，则说明 c 是一个质数
def countPrimeSetBits(self, left: int, right: int) -> int:
    return sum(((1 << bin(index).count("1")) & 665772) != 0 for index in range(left, right+1))
```

```python
class Solution:
    def _countPrimeSetBits(self, left: int, right: int) -> int:
        return sum(self.isPrime(self.bit_count(index)) for index in range(left, right + 1))

    def bit_count(self, n: int) -> int:
        result = 0
        while n != 0:
            result += (n & 1)
            n >>= 1
        return result

    def isPrime(self, n: int) -> bool:
        if n < 2: return False
        i = 2
        while i * i <= n:
            if n % i == 0: return False
            i += 1
        return True
```