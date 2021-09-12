
# 20. Valid Parentheses

## 题目

```
Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.

An input string is valid if:
Open brackets must be closed by the same type of brackets.
Open brackets must be closed in the correct order.

Constraints:
1 <= s.length <= 104
s consists of parentheses only '()[]{}'.
```

## 解法

### 第一思路

```C
bool isValid(char *s)
{
    int length = (int)strlen(s);
    if (s[0] == ')' || s[0] == ']' || s[0] == '}') return false;
    
    char *stack = malloc((length + 1) * sizeof(char));
    stack[0] = '#';
    int stackSize = 0;
    
    for (int i=0; i<length; i++)
    {
        if (s[i] == ')' && stack[stackSize] == '(') {
            stackSize--;
        }
        else if (s[i] == ']' && stack[stackSize] == '[') {
            stackSize--;
        }
        else if (s[i] == '}' && stack[stackSize] == '{') {
            stackSize--;
        }
        else {
            stack[++stackSize] = s[i];
        }
    }
    free(stack);
    return stackSize == 0;
}
```

<br>

### 优化思路

```C
// '(' 与 ')' 的ASCII值差1，'[' 与 ']'，'{' 与 '}' 的ASCII值差2
bool isValid(char *s)
{
    if (s == NULL) return false;
    
    int length = (int)strlen(s);
    char *stack = malloc(sizeof(char) * (length + 1));
    stack[0] = '#';
    int top = 0;
    
    for (int i=0; i<length; i++)
    {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
            stack[++top] = s[i];
        }
        else if (s[i] == stack[top] + 1 || s[i] == stack[top] + 2) {
            top--;
        }
        else return false;
    }
    free(stack);
    return top == 0;
}
```

<br>

## Python

### 第一思路

```python3
class Solution:
    def isValid(self, s: str) -> bool:
        stack = ['#']

        brackets = {')':'(', ']':'[', '}':'{'}
        for ele in s:
            if ele in brackets.keys():
                if brackets[ele] == stack[len(stack) - 1]:
                    stack.pop()
                else:
                    stack.append(ele)
            else:
                stack.append(ele)

        return len(stack) == 1
```
<br>

### 优化

```python3
class Solution(object):
    def isValid(self, s):
        stack = []
        mapping = {")": "(", "}": "{", "]": "["}
        for char in s:
            if char in mapping:
                if mapping[char] != top_element:
                    return False
            else:
                stack.append(char)

        return not stack
```