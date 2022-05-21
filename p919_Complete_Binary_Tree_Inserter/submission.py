from collections import deque 

# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class CBTInserter(object):
    def __init__(self, root):
        self.root = root
        self.q = deque()
        travq = collections.deque([root])
        while travq:
            f = travq.popleft()
            if not f.left or not f.right:
                self.q.append(f) # there're children slots for insertion
            if f.left: travq.append(f.left)
            if f.right: travq.append(f.right)

    def insert(self, v):
        f = self.q[0]
        self.q.append(TreeNode(v))
        if not f.left: 
            f.left = self.q[-1]
        else:
            f.right = self.q[-1]
            self.q.popleft()
        return f.val

    def get_root(self):
        return self.root

# Your CBTInserter object will be instantiated and called as such:
# obj = CBTInserter(root)
# param_1 = obj.insert(val)
# param_2 = obj.get_root()
