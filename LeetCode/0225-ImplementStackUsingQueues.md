
# 225. Implement Stack using Queues

## 题目

```
Implement the following operations of a stack using queues.

push(x) -- Push element x onto stack.
pop() -- Removes the element on top of the stack.
top() -- Get the top element.
empty() -- Return whether the stack is empty.

Notes:
- You must use only standard operations of a queue -- which means only push to back, peek/pop from front, size, and is empty operations are valid.
- Depending on your language, queue may not be supported natively. 
  You may simulate a queue by using a list or deque (double-ended queue), as long as you use only standard operations of a queue.
- You may assume that all operations are valid (for example, no pop or top operations will be called on an empty stack).
```

<br>

## 解法

### python 

- 第一思路 push: O(1)  pop:O(n)

```
import queue

class MyStack:
    def __init__(self):
        self.my_queue = queue.Queue()
        self.last = None

    def push(self, x: int) -> None:
    	self.last = x
    	self.my_queue.put(x)
        

    def pop(self) -> int:
    	for i in range(self.my_queue.qsize()):
    		if i == self.my_queue.qsize() - 1:
    			return self.my_queue.get()
    		else:
    			self.last = self.my_queue.get()
    			self.my_queue.put(self.last)
        

    def top(self) -> int:
    	return self.last if not self.my_queue.empty() else None
        

    def empty(self) -> bool:
    	return self.my_queue.empty()
``` 