# 0003-LongestSubstringWithoutRepeatingCharacters



## 题目

```
Given a string s, find the length of the longest substring without repeating characters.

Example 1:
Input: s = "abcabcbb"
Output: 3
Explanation: The answer is "abc", with the length of 3.
```

<br>

## 解法

```
class Solution:
    def lengthOfLongestSubstring(self, s: str) -> int:
    	subStringSet = set()
    	result, right = 0, -1
    	for i in range(len(s)):
    		if i != 0:
    			subStringSet.remove(s[i - 1])
    		while right + 1 < len(s) and s[right + 1] not in subStringSet:
    			subStringSet.add(s[right+1])
    			right += 1
    		result = max(result, right - i + 1)
    	return result
```
