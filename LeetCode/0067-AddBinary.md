
# 67. Add Binary


## 题目

```
Given two binary strings, return their sum (also a binary string).

The input strings are both non-empty and contains only characters 1 or 0.

Example 1:

Input: a = "11", b = "1"
Output: "100"
Example 2:

Input: a = "1010", b = "1011"
Output: "10101"
```

<br>

## 解法


### C 

```
/**
 * 0067 Add Binary
 * 时间复杂度： O(max(lenA, lenB))  空间复杂度：O(max(lenA, lenB) + 2)
 */
char* addBinary(char *a, char *b)
{
   int len = strlen(a) > strlen(b) ? (int)strlen(a) : (int)strlen(b);
   // a+b 可能导致进位 所以 result 多申请一位
   char *result = malloc(sizeof(char) * (len + 2));
   // C 中字符串以 '\0' 结尾
   memset(result, '\0', len + 2);
   
   int m = 0, lenA = (int)strlen(a), lenB = (int)strlen(b);
   while(lenA > 0 || lenB > 0)
   {
      // 转化为数字
      int x = (int)((lenA > 0 ? a[--lenA] : '0') - '0');
      int y = (int)((lenB > 0 ? b[--lenB] : '0') - '0');
      // 保留位的值
      result[len--] = (char)((x + y + m) % 2 + '0');
      // 是否有进位
      m = (x + y + m) / 2;
   }
   
   if (m == 1) result[0] = '1';
   else result++;
   return result;
}
```
