
# 20. Valid Parentheses


## Python

### 第一思路



### 优化

```
class Solution(object):
    def isValid(self, s):
        """
        :type s: str
        :rtype: bool
        """
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