# 38.Count and Say

## 题

```
「外观数列」是一个数位字符串序列，由递归公式定义：
countAndSay(1) = "1"
countAndSay(n) 是 countAndSay(n-1) 的行程长度编码。

行程长度编码（RLE）是一种字符串压缩方法，其工作原理是通过将连续相同字符（重复两次或更多次）替换为字符重复次数（运行长度）和字符的串联。
例如，要压缩字符串 "3322251" ，我们将 "33" 用 "23" 替换，将 "222" 用 "32" 替换，将 "5" 用 "15" 替换并将 "1" 用 "11" 替换。因此压缩后字符串变为 "23321511"。

给定一个整数 n ，返回 外观数列 的第 n 个元素。

输入：n = 4
输出："1211"
解释：
countAndSay(1) = "1"
countAndSay(2) = "1" 的行程长度编码 = "11"
countAndSay(3) = "11" 的行程长度编码 = "21"
countAndSay(4) = "21" 的行程长度编码 = "1211"

输入：n = 1
输出："1"

提示：
1 <= n <= 30
```


## 解

```python3
def countAndSay(self, n: int) -> str:
    ans = '1'
    for _ in range(1, n):
        left, right = 0, 0
        temp = ''
        while right < len(ans):
            while right < len(ans) and ans[right] == ans[left]:
                right += 1
            temp += (str(right - left) + ans[left])
            left = right
        ans = temp
    return ans
```
