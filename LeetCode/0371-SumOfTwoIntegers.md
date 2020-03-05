# 371. Sum of Two Integers


## 题目

```
Calculate the sum of two integers a and b, but you are not allowed to use the operator + and -.

Example 1:
Input: a = 1, b = 2
Output: 3
```

<br>

## 解法

### Python

- 借鉴思路

```
def getSum(self, a: int, b: int) -> int:
	# 2^32
    MASK = 0x100000000
    # 整型最大值
    MAX_INT = 0x7FFFFFFF
    MIN_INT = MAX_INT + 1
    while b != 0:
        # 计算进位
        carry = (a & b) << 1 
        # 取余范围限制在 [0, 2^32-1] 范围内
        a = (a ^ b) % MASK
        b = carry % MASK
    return a if a <= MAX_INT else ~((a % MIN_INT) ^ MAX_INT)
```

### C

```
int getSum(int a, int b)
{
    while(b != 0){
        int temp = a ^ b;
        b = (unsigned int)(a & b) << 1;
        a = temp;
    }
    return a;
}
```