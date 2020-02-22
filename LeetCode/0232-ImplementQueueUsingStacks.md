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

- 第一思路

```
class MyQueue:

    def __init__(self):
    	self.myStack = []
    	self.stack_top = -1
    	self.empty = []
    	self.empty_top = -1

    def push(self, x: int) -> None:
    	self.myStack.append(x)
    	self.stack_top += 1

    def pop(self) -> int:
    	while self.stack_top >= 0:
    		a = self.myStack.pop()
    		self.stack_top -= 1
    		self.empty.append(a)
    		self.empty_top += 1

    	result = self.empty.pop()
    	self.empty_top -= 1
    	while self.empty_top >= 0:
    		b = self.empty.pop()
    		self.empty_top -= 1
    		self.myStack.append(b)
    		self.stack_top += 1
    	return result
        

    def peek(self) -> int:
        while self.stack_top >= 0:
        	a = self.myStack.pop()
        	self.stack_top -= 1
        	self.empty.append(a)
        	self.empty_top += 1
        result = self.empty[self.empty_top]
        while self.empty_top >= 0:
        	a = self.empty.pop()
        	self.empty_top -= 1
        	self.myStack.append(a)
        	self.stack_top += 1
        return result
        

    def empty_(self) -> bool:
    	return self.stack_top < 0
```