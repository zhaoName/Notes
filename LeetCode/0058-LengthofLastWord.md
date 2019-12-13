

# 58. Length of Last Word


## C

### 第一思路

```
int lengthOfLastWord(char * s)
{
    int length = (int)strlen(s);
    if (length == 0) return 0;
    
    int num = 0;
    while (s[length-1] == ' ') {
        length--;
        // 可能 s 只包含' '
        if (length == 0) return 0;
    }
    for (int i=length-1; i>=0; i--) {
        if (s[i] != ' ') {
            num++;
        }
        else {
            break;
        }
    }
    return num;
}
```