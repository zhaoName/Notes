
# 66. Plus One


## 题目

```
Given a non-empty array of digits representing a non-negative integer, plus one to the integer.

The digits are stored such that the most significant digit is at the head of the list, and each element in the array contain a single digit.

You may assume the integer does not contain any leading zero, except the number 0 itself.
```


<br>


## 解法

### Python

- 第一思路

```
# 时间复杂度 O(n)  空间复杂度 O(n+1)
def plusOne(self, digits: List[int]) -> List[int]:
    resultList = [0] * (len(digits) +1)
    i = len(digits) - 1
    while i>=0 and (digits[i] +1) / 10 == 1:
        resultList[i+1] = 0
        i -= 1

    if i < 0:
        resultList[0] = 1
    else:
        resultList[1:i+2] = digits[:i+1]
        resultList[i+1] += 1
        del resultList[0]

    return resultList
```

- 优化思路

```
# 时间复杂度 O(n)  空间复杂度 O(1)
def plusOne(self, digits: List[int]) -> List[int]:
	i = len(digits)-1
	mo = 1
	while i>=0:
	    temp = digits[i]
	    digits[i] = (temp + mo) % 10
	    mo = (temp + mo) // 10
	    i -= 1
	
	if digits[0] == 0:
	    digits.insert(0, 1)
	return digits
```

<br>

### C

```
/**
 * 66. Plus One
 * 时间复杂度 O(n)  空间复杂度 O(n+1)
 */
int* plusOne(int* digits, int digitsSize, int* returnSize)
{
    // 有可能进位
    int *result = malloc(sizeof(int) * (digitsSize + 1));
    
    int i = digitsSize-1;
    while(i >=0 && (digits[i] + 1) / 10) {
        result[i+1] = 0;
        i--;
    }
    if (i < 0) {
        result[0] = 1;
        *returnSize = digitsSize + 1;
    }
    else {
        *returnSize = digitsSize;
        memcpy(result+1, digits, (i+1)*sizeof(int));
        result[i+1] += 1;
        result++;
    }
    return result;
}
```