# 6.Zigzag Conversion


## 题

```
将一个给定字符串 s 根据给定的行数 numRows ，以从上往下、从左到右进行 Z 字形排列。
比如输入字符串为 "PAYPALISHIRING" 行数为 3 时，排列如下：
P   A   H   N
A P L S I I G
Y   I   R
之后，你的输出需要从左往右逐行读取，产生出一个新的字符串，比如："PAHNAPLSIIGYIR"。

输入：s = "PAYPALISHIRING", numRows = 4
输出："PINALSIGYAHRPI"
解释：
P     I     N
A   L S   I G
Y A   H R
P     I

示例 3：
输入：s = "A", numRows = 1
输出："A"


提示：
1 <= s.length <= 1000
s 由英文字母（小写和大写）、',' 和 '.' 组成
1 <= numRows <= 1000
```


## 解

```python3
# Time Complexity: O(n), Space: O(row*column)
def convert_01(self, s: str, numRows: int) -> str:
    if numRows == 1: return s
    dop = (numRows - 1) * 2
    left = len(s) % dop
    left = 1 if left <= numRows else (left - numRows) + 1
    column = len(s) // dop * (numRows - 1) + left

    list = [['0'] * column for _ in range(numRows)]
    count, temp = 0, 0
    while count < len(s):
        for i in range(numRows - 1):
            if count >= len(s):
                break
            list[i][temp] = s[count]
            count += 1
        for i in range(numRows - 1, 0, -1):
            if count >= len(s):
                break
            list[i][temp] = s[count]
            count += 1
            temp += 1
    ans = ''
    for i in range(numRows):
        ans += ''.join(filter(lambda x: x != '0', list[i]))
    return ans
```

```python3
def convert(self, s: str, numRows: int) -> str:
    if numRows == 1: return s

    res = ["" for _ in range(numRows)]
    i, flag = 0, -1
    for ch in s:
        res[i] += ch
        if i == 0 or i == numRows - 1:
            flag = -flag
        i += flag
    return ''.join(res)
```
