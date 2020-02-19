
# 204. Count Primes


## 题目

```
Count the number of prime numbers less than a non-negative number, n.

Example:
Input: 10
Output: 4
Explanation: There are 4 prime numbers less than 10, they are 2, 3, 5, 7.
```

<br>

## 解法

### Python

- 第一思路 暴力求解 超时

```
# O(n * sqrt(i)) 超时
def countPrimes_simple(self, n: int) -> int:
	if n == 1: return 0
	if n <= 4: return n - 2
	if n == 5: return 2
	count = 0
	for i in range(7, n, 2):
		if i % 5 == 0: continue
		if i % 3 == 0: continue
		j, k = 2, math.floor(i ** 0.5)
		while(j <= k):
			if i % j == 0:
				break
			j += 1
		print(i, j, k)
		# 注意最后一次循环，会执行j++，此时 j=k+1，所以有j>k
		if j > k:
			count += 1
	return count + 3
```


- 借鉴思路 筛选

```

## Time Complexity:O(nloglogn)  Space Time:O(n)
def countPrimes_multiple(self, n) -> int:
	if n <= 2: return 0
	isPrimes, count = 0.5 * n* [0], 0
	for i in range(2, n, 2):
		if isPrimes[i] == 0:
			count += 1
			for j in range(i*2, n, i):
				isPrimes[j] = 1
	return count
```


- 借鉴思路  在筛选的基础上优化

```
def countPrimes_sqrt(self, n) -> int:
	if n<= 2: return 0
	isPrimes = [1] * n
	isPrimes[0] = isPrimes[1] = 0
	## i > (n ** 0.5)  ==> i * i > n, 所以只需要遍历到根号n就可以了
	for i in range(2, math.ceil(n ** 0.5)):
		if isPrimes[i] == 1:
			isPrimes[i * i: n: i] = [0] * len(isPrimes[i * i: n: i])
	return sum(isPrimes)
```