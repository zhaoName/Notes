# 1111. Maximum Nesting Depth of Two Valid Parentheses Strings

## 题目

```
A string is a valid parentheses string (denoted VPS) if and only if it consists of "(" and ")" characters only, and:
- It is the empty string, or
- It can be written as AB (A concatenated with B), where A and B are VPS's, or
- It can be written as (A), where A is a VPS.

We can similarly define the nesting depth depth(S) of any VPS S as follows:
- depth("") = 0
- depth(A + B) = max(depth(A), depth(B)), where A and B are VPS's
- depth("(" + A + ")") = 1 + depth(A), where A is a VPS.

For example,  "", "()()", and "()(()())" are VPS's (with nesting depths 0, 1, and 2), and ")(" and "(()" are not VPS's.
Given a VPS seq, split it into two disjoint subsequences A and B, such that A and B are VPS's (and A.length + B.length = seq.length).
Now choose any such A and B such that max(depth(A), depth(B)) is the minimum possible value.
Return an answer array (of length seq.length) that encodes such a choice of A and B:  answer[i] = 0 if seq[i] is part of A, else answer[i] = 1.  
Note that even though multiple answers may exist, you may return any of them.


Input: seq = "()(())()"
Output: [0,0,0,1,1,0,1,1]
 
Constraints:
1 <= seq.size <= 10000
```

<br>

## 解法

### Python 

- 第一思路

```
def maxDepthAfterSplit(self, seq: str) -> list:
    if not seq: return list()

    res, A, B = list(), list(), list()
    for i in range(len(seq)):
        if seq[i] == '(':
            if len(A) <= len(B):
                A.append('(')
                res.append(0)
            else:
                B.append('(')
                res.append(1)
        else:
            if A and A[-1] == '(':
                A.pop()
                res.append(0)
            else:
                B.pop()
                res.append(1)
    return res
```

- 借鉴思路

```
# 栈上的左括号只要按奇偶分配给A和B就可以了
# 括号本身字符的ascii就是一奇一偶了，直接和index异或取最低位就可以了
def maxDepthAfterSplit(self, seq: str) -> list:
	if not seq: return list()
	idx, res = 0, [0] * len(seq)
	for a in seq:
		#res[idx] = idx & 1 if a == '(' else (idx+1) & 1
		res[idx] = (ord(a) ^ idx) & 1
		idx += 1
	return res
```



