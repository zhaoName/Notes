
# 524. Longest Word in Dictionary through Deleting


## 题目 

```
Given a string s and a string array dictionary, return the longest string in the dictionary that can be formed by deleting some of the given string characters.
If there is more than one possible result, return the longest word with the smallest lexicographical order. If there is no possible result, return the empty string.

Input: s = "abpcplea", dictionary = ["ale","apple","monkey","plea"]
Output: "apple"

Constraints:
1 <= s.length <= 1000
1 <= dictionary.length <= 1000
1 <= dictionary[i].length <= 1000
s and dictionary[i] consist of lowercase English letters.
```


## 解法

```python3
def findLongestWord_sort(self, s: str, dictionary: List[str]) -> str:
    # sorted 中 key 是升序排序的，如果想要让 word 长度比较长的排在前面，可以取反 -len(x)
    sorted_dictionary = sorted(dictionary, key=lambda x: [-len(x), x])

    for d in sorted_dictionary:
        p, q = 0, 0
        while q < len(s):
            if d[p] == s[q]:
                p += 1
            q += 1
            if p == len(d): return d
    return ""


def findLongestWord_(self, s: str, dictionary: List[str]) -> str:
    ans = ""
    for d in dictionary:
        p, q = 0, 0
        while q < len(s) and p < len(d):
            if d[p] == s[q]:
                p += 1
            q += 1
        if p == len(d):
            if len(d) > len(ans) or (len(d) == len(ans) and d < ans):
                ans = d
    return ans
```