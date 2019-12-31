# 14. Longest Common Prefix


## 题目

```
Write a function to find the longest common prefix string amongst an array of strings.

If there is no common prefix, return an empty string "".

Example 1:
Input: ["flower","flow","flight"]
Output: "fl"

Example 2:
Input: ["dog","racecar","car"]
Output: ""

Explanation: There is no common prefix among the input strings.

Note:
All given inputs are in lowercase letters a-z.
```

<br>

## 解法

### C


- 第一思路

```
/**
 * 14. Longest Common Prefix
 */
char* longestCommonPrefix(char **strs, int strsSize)
{
   if (strsSize <= 0) return "";
   
   int min = 10000;
   for (int i=0; i<strsSize; ++i)
   {
      if (min > strlen(strs[i])) {
         min = strlen(strs[i]);
      }
   }
   char *result = malloc(sizeof(char) * (min + 2));
   memset(result, '\0', (min + 2));
   
   for (int i=0; i<min; ++i)
   {
      int j = 1;
      while(j<strsSize)
      {
         if (strs[j][i] != strs[0][i]) {
            return result;
         }
         j++;
      }
      result[i] = strs[0][i];
   }
   return result;
}
```


- 优化思路

```
char * longestCommonPrefix_1(char ** strs, int strsSize)
{
    if (strsSize <= 0) return "";

    char *first = strs[0];
    for (int i=1; i<strsSize; i++)
    {
        int j = 0;
        while (first[j] != '\0' && strs[i][j] != '\0') {
        if (first[j] != strs[i][j]) break;
            j++;
        }
        // leetcode 中没报错，但 Xcode 中报错
        // char *first = "xxx" 存储在常量区 不能修改
        first[j] = '\0';
    }
    return first;
}
```


<br>


### Python


```
class Solution:
    def longestCommonPrefix(self, strs: List[str]) -> str:
        if len(strs) == 0: return ""
        firstStr = strs[0]
        for i in range(1, len(strs)):
            while strs[i].find(firstStr) != 0:
                firstStr = firstStr[0:len(firstStr)-1]
                if len(firstStr) == 0:
                    return ""

        return firstStr
```