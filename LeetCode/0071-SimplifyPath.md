# 71. Simplify Path


<br>

## 题目

```
Given a string path, which is an absolute path (starting with a slash '/') to a file or directory in a Unix-style file system, convert it to the simplified canonical path.
In a Unix-style file system, a period '.' refers to the current directory, a double period '..' refers to the directory up a level, and any multiple consecutive slashes (i.e. '//') are treated as a single slash '/'.
For this problem, any other format of periods such as '...' are treated as file/directory names.
The canonical path should have the following format:
The path starts with a single slash '/'.
Any two directories are separated by a single slash '/'.
The path does not end with a trailing '/'.
The path only contains the directories on the path from the root directory to the target file or directory (i.e., no period '.' or double period '..')
Return the simplified canonical path.

Input: path = "/home//foo/"
Output: "/home/foo"
Explanation: In the canonical path, multiple consecutive slashes are replaced by a single one.

Constraints:
1 <= path.length <= 3000
path consists of English letters, digits, period '.', slash '/' or '_'.
path is a valid absolute Unix path.
```

<br>

## 解法

```python3
## ".."表示上级目录即 /a/b/../ -> /a
def simplifyPath(self, path: str) -> str:
    names = path.split("/")
    stack = list()
    for name in names:
        if name == "..":
            if stack:
                stack.pop()
        elif name and name != ".":
            stack.append(name)
    return "/" + "/".join(stack)
```

<br>