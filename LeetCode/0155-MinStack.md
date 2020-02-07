
# 155. Min Stack


## 题目

```
Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.

push(x) -- Push element x onto stack.
pop() -- Removes the element on top of the stack.
top() -- Get the top element.
getMin() -- Retrieve the minimum element in the stack.
 

Example:
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin();   --> Returns -3.
minStack.pop();
minStack.top();      --> Returns 0.
minStack.getMin();   --> Returns -2.
```


<br>


## 解法


### Python


```
class MinStack:

    def __init__(self):
    	self.stack = []
        

    def push(self, x: int) -> None:
    	self.stack.append(x)
    	print(self.stack)
        

    def pop(self) -> None:
    	try:
    		self.stack.pop()
    	except:
    		print('MinStack is empty')        


    def top(self) -> int:
    	try:
    		return self.stack[-1]
    	except:
    		print('MinStack is empty')
        

    def getMin(self) -> int:
    	try:
    		return min(self.stack)
    	except:
    		print('MinStack is empty')


```