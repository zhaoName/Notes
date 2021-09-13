
# 7. Reverse Integer

## 题目

```
Given a signed 32-bit integer x, return x with its digits reversed.
If reversing x causes the value to go outside the signed 32-bit integer range [-231, 231 - 1], then return 0.
Assume the environment does not allow you to store 64-bit integers (signed or unsigned).

Input: x = 123
Output: 321

Constraints: -2^31 <= x <= 2^31 - 1
```

## 解法

- 第一思路


```C
int reverse(int x)
{
    // 32-bit signed int 最大为 2147483647
    char *numString = malloc(16 * sizeof(char));
    sprintf(numString, "%d", x);
    
    char flag = '+';
    if (numString[0] == '-') {
        flag = '-';
        ++numString;
    }
    
    unsigned long len = strlen(numString);
    int i = 0;
    // int 类型数据反转后的值可能 会溢出
    long sum = 0;
    while (i<len) {
        int a = 0;
        char c = numString[i];
        sscanf(&c, "%d", &a);
        sum += (a * pow(10, i));
        i++;
    }
    if (flag == '-') sum = -sum;
    if (sum > 2147483647 || sum < -2147483647) return 0;
    return (int)sum;
}
```

<br>

- C 优化

```C
/**
 * 32-bit int: -2147483648 ~ 2147483647
 */
int reverse(int x) {
    int rev = 0;
    while (x != 0) {
        if (rev < INT_MIN / 10 || rev > INT_MAX / 10) {
            return 0;
        }
        int digit = x % 10;
        x /= 10;
        rev = rev * 10 + digit;
    }
    return rev;
}
```

- python 不考虑移除写法

```python3
## [-2147483648, 2147483647]
def reverse_str(self, x: int) -> int:
    if -10 < x < 10: return x
    xs, ans = str(x), 0
    if xs[0] == '-':
        xs = xs[:0:-1]
        ans = -(int(xs))
    else:
      ans = int(xs[::-1])
    return 0 if ans > 2147483647 or ans < -2147483648 else ans
```





