## 题目

```
Implement the following operations of a queue using stacks.

push(x) -- Push element x to the back of queue.
pop() -- Removes the element from in front of queue.
peek() -- Get the front element.
empty() -- Return whether the queue is empty.

Notes:
- You must use only standard operations of a stack -- which means only push to top, peek/pop from top, size, and is empty operations are valid.
- Depending on your language, stack may not be supported natively.
  You may simulate a stack by using a list or deque (double-ended queue), as long as you use only standard operations of a stack.
- You may assume that all operations are valid (for example, no pop or peek operations will be called on an empty queue).
```

<br>

## 解法


### Python

- 第一思路 用list当做栈

```Python3
class MyQueue:

    def __init__(self):
    	self.myStack = []
    	self.stack_top = -1
    	self.out = []
    	self.out_top = -1
    	self.front = None

    def push(self, x: int) -> None:
    	if self.stack_top == -1:
    		self.front = x
    	self.myStack.append(x)
    	self.stack_top += 1

    def pop(self) -> int:
    	if self.out_top == -1:
    		while self.stack_top >= 0:
    			self.out.append(self.myStack.pop())
    			self.stack_top -= 1
    			self.out_top += 1
    	result = self.out.pop()
    	self.out_top -= 1
    	return result
        

    def peek(self) -> int:
        if self.out_top >= 0:
        	print(self.out)
        	return self.out[-1]
        return self.front if self.stack_top >= 0 else None
        

    def empty(self) -> bool:
    	return self.stack_top < 0 and self.out_top < 0
```


- 第一思路 先实现栈 再实现队列

```python3
class MyStack:
	def __init__(self):
		self.list = []
		self.top = -1

	def push(self, x:int) -> None:
		self.list.append(x)
		self.top += 1

	def pop(self) -> int:
		if self.top == -1: return None
		result = self.list.pop()
		self.top -= 1
		return result

	def peek(self) -> int:
		if self.top == -1: return None
		return self.list[self.top]

	def empty(self) -> bool:
		return self.top == -1


class MyQueue_myStack:

    def __init__(self):
    	self.front = 0
    	self.inStack = MyStack()
    	self.outStack = MyStack()

    def push(self, x: int) -> None:
    	if self.inStack.empty():
    		self.front = x
    	self.inStack.push(x)
        

    def pop(self) -> int:
    	if self.outStack.empty():
    		while not self.inStack.empty():
    			self.outStack.push(self.inStack.pop())
    	return self.outStack.pop()
        

    def peek(self) -> int:
    	if not self.outStack.empty():
    		return self.outStack.peek()
    	return self.front if not self.inStack.empty() else None
        

    def empty(self) -> bool:
        return self.inStack.empty() and self.outStack.empty()
```