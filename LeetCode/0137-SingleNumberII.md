
# 137. Single Number II


## 题目

```
Given a non-empty array of integers, every element appears three times except for one, which appears exactly once. Find that single one.

Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
```

<br>

## 解法


### Python

- hash_table

```
## hash_table
## 时间复杂度O(n)  空间复杂度O(n)
def singleNumber_hash(self, nums) -> int:
	hash_table = {}
	for i in nums:
		try:
			hash_table[i] += 1
		except:
			hash_table[i] = 1
	return min(hash_table, key=lambda k:hash_table[k])
```


- 数学


```
## 数学方法 3(a + b + c) - (a+a+a +b+b+b +c) = 2c
## 时间复杂度O(n)  空间复杂度O(n)
def singleNumber_math(self, nums)->int:
	return (3 * sum(set(nums)) - sum(nums)) // 2
```

<br>


### C


```
/**
 * 137. Single Number II
 *
 * 时间复杂度 O(32n) 空间复杂度O(32)
 */
int singleNumber(int* nums, int numsSize)
{
   int s[32] = {0};
   
   for (int i = 0; i<numsSize; i++)
   {
      for (int j=0; j<32; j++) {
         // 将 nums[i] 拆分成二进制 并放到数组 s 的对应位置
         s[j] += ((unsigned int)nums[i] >> j) % 2;
         // 若 s[j] == 3 可以认为某个数字出现三次 将其置为 0
         // 换句话说 就是将出现3次的元素 删除
         if (s[j] == 3) s[j] = 0;
      }
   }
   // 当要查找的数字为奇数 s[0] 必为1, 而对 1(flag) 左移>0位 得到结果必为偶数
   // 所以 左移得到的结果 | s[0],以保证从 二进制 => 10进制的正确性
   int res = s[0];
   unsigned int flag = 1;
   for (int i=0; i<32; i++) {
      if (s[i]) res |= flag << i;
   }
   return res;
}
```