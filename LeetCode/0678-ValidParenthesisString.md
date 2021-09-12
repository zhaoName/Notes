# 678. Valid Parenthesis String

## 题目

```
Given a string s containing only three types of characters: '(', ')' and '*', return true if s is valid.

The following rules define a valid string:
Any left parenthesis '(' must have a corresponding right parenthesis ')'.
Any right parenthesis ')' must have a corresponding left parenthesis '('.
Left parenthesis '(' must go before the corresponding right parenthesis ')'.
'*' could be treated as a single right parenthesis ')' or a single left parenthesis '(' or an empty string "".

Constraints:
1 <= s.length <= 100
s[i] is '(', ')' or '*'.
```

<br>


## 解法

```python3
class Solution:
    ## 两个栈 一个存放'(' 一个存放'*'
    ## Time Complexity: O(n), Space Complexity: O(1)
    def checkValidString(self, s: str) -> bool:
        leftStack, starStack = [], []
        for i in range(len(s)):
            if s[i] == '(':
                leftStack.append(i)
            elif s[i] == '*':
                starStack.append(i)
            else:
                # 优先 pop 括号，若两个都为空则返回 False
                if leftStack:
                    leftStack.pop()
                elif starStack:
                    starStack.pop()
                else:
                    return False
        # 当两个栈都不为空时, 若左括号下标大于星号下标 则不满足要求
        while len(leftStack) != 0 and len(starStack) != 0:
            if leftStack.pop() > starStack.pop():
                return False
        return len(leftStack) == 0

    # 贪心算法 Time Complexity: O(n), Space Complexity: O(1)
    def checkValidString_(self, s: str) -> bool:
        smaller, biggest = 0, 0
        for ch in s:
            if ch == '(':
                smaller += 1
                biggest += 1
            elif ch == ')':
                smaller = max(smaller-1, 0)
                biggest -= 1
                if biggest < 0:
                    # 说明没有足够的 '(' 和 '*' 与 ')' 匹配
                    return False
            else:
                smaller = max(smaller-1, 0)
                biggest += 1
        # '(' 需匹配完
        return smaller == 0
```