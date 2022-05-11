# 17. Letter Combinations of a Phone Number

<br>

## 题目

```
Given a string containing digits from 2-9 inclusive, return all possible letter combinations that the number could represent. Return the answer in any order.
A mapping of digit to letters (just like on the telephone buttons) is given below. Note that 1 does not map to any letters.

Input: digits = "23"
Output: ["ad","ae","af","bd","be","bf","cd","ce","cf"]

Constraints:
0 <= digits.length <= 4
digits[i] is a digit in the range ['2', '9'].
```

<br>

## 解法

```python3
# 队列
def letterCombinations_queue(self, digits: str) -> List[str]:
    if len(digits) == 0: return []

    dict = {"2": ["a", "b", "c"],
            "3": ["d", "e", "f"],
            "4": ["g", "h", "i"],
            "5": ["j", "k", "l"],
            "6": ["m", "n", "o"],
            "7": ["p", "q", "r", "s"],
            "8": ["t", "u", "v"],
            "9": ["w", "x", "y", "z"]}
    queue = [""]
    for digit in digits:
        for _ in range(len(queue)):
            # pop队列首元素, 组合下一个字母，再添加到队列
            # 如[a, b, c], pop的是a, 下面 for 循环结束后queue=[b, c, ad, ae, af]
            temp = queue.pop(0)
            for letter in dict[digit]:
                queue.append(temp + letter)
    return queue
```


```python3
# 回溯
def letterCombinations(self, digits: str) -> List[str]:
    if not digits: return []

    dict = {"2": ["a", "b", "c"],
            "3": ["d", "e", "f"],
            "4": ["g", "h", "i"],
            "5": ["j", "k", "l"],
            "6": ["m", "n", "o"],
            "7": ["p", "q", "r", "s"],
            "8": ["t", "u", "v"],
            "9": ["w", "x", "y", "z"]}

    # n层for循环 转化成回溯算法
    def dfs(temp: str, index: int):
        # 递归结束条件
        if index == len(digits):
            ans.append(temp)
            return
        digit = digits[index]
        for letter in dict[digit]:
            dfs(temp+letter, index+1)

    ans = []
    dfs("", 0)
    return ans
```

<br>