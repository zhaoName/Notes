
# 7. Reverse Integer

## C

### 第一思路


```
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

### 优化

```
/**
 * 32-bit int: -2147483648 ~ 2147483647
 *
 * 若 temp = sum * 10 + x % 10 溢出 则 sum ≥ IntMax / 10
 * 若 sum > IntMax / 10, 则 sum * 10 + x % 10 一定溢出
 * 若 sum = IntMax / 10, 则  x % 10 > 7 一定溢出
 */
int reverse(int x)
{
    long sum = 0;
    while (x != 0) {
        sum = sum * 10 + x % 10;
        x = x / 10;
    }
    if (sum > 2147483647 || sum < -2147483647) return 0;
    return (int)sum;
}
```