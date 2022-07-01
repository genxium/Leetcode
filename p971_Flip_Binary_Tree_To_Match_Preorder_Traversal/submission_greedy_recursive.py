
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
"""
test cases
[1,2]
[2,1]
[1,2,3]
[1,3,2]
[1,2,3]
[1,2,3]
[1,null,2]
[1,2]
[2,1,null,4,3]
[2,1,3,4]
"""
debug = False
class Solution:
    def dfs(self, cand, root, i, voyage, ans, level):
        # returns the successfully traversed tree size of the current "root", or None if traversal fails
        indent = " "*(level << 1)
        if i == len(voyage):
            if root: 
                return None
            if (ans and ans[0] == -1) or (len(cand) < len(ans)):
                ans[:] = [p for p in cand]
            return 0
            
        if not root:
            return 0
        
        if voyage[i] != root.val:
            return None
    
        if debug: print(f'{indent}visiting root.val={None if not root else root.val} at voyage[{i}]={None if i == len(voyage) else voyage[i]}, cand={cand}')

        res = 1
        leftSize = None
        rightSize = None
        leftSize = self.dfs(cand, root.left, i+1, voyage, ans, level+1)
        if leftSize is not None:
            rightSize = self.dfs(cand, root.right, i+1+leftSize, voyage, ans, level+1)
        else:
            cand.append(root.val) # need a flip here 
            leftSize = self.dfs(cand, root.right, i+1, voyage, ans, level+1)
            rightSize = self.dfs(cand, root.left, i+1+leftSize, voyage, ans, level+1) if leftSize else None 
            if leftSize is None or rightSize is None:
                cand.pop() # backtracking, note that "cand" shouldn't be backtracked if the traversal is successful
                if debug: print(f'{indent}popped, cand={cand}')

        if leftSize is None or rightSize is None:
            return None
        
        return res+leftSize+rightSize
        
    def flipMatchVoyage(self, root: Optional[TreeNode], voyage: List[int]) -> List[int]:
        cand, ans = [], [-1]
        self.dfs(cand, root, 0, voyage, ans, 0)
        if debug: print(f'Now ans = {ans}')
        return ans
