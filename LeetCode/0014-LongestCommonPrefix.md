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