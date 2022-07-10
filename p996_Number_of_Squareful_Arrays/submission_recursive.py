debug = True

def isSquare(val):
    return math.isqrt(val) ** 2 == val
    
def buildAdj(cnt):
    adj = defaultdict(set)
    for k1 in cnt.keys():            
        for k2 in cnt.keys():
            if isSquare(k1+k2):
                adj[k1].add(k2)
    return adj
    
def dfs(buff, n, cnt, adj):
    if len(buff) == n:
        return 1
    
    res = 0
    if not buff:
        for k in cnt.keys():
            buff.append(k)
            cnt[k] -=1 
            res += dfs(buff, n, cnt, adj)
            cnt[k] +=1 # backtracking
            buff.pop() # backtracking
    else:
        for k in adj[buff[-1]]:
            if 0 == cnt[k]:
                continue
            buff.append(k)
            cnt[k] -=1
            res += dfs(buff, n, cnt, adj)
            cnt[k] +=1 # backtracking
            buff.pop() # backtracking
    return res
    
"""
test cases
[1,17,8]
[2,2,2]
[2,2,2,2,2,2,2,2,2,2,2,2]
"""
class Solution:
    def numSquarefulPerms(self, nums: List[int]) -> int:
        n = len(nums)
        cnt = defaultdict(int)
        for val in nums:
            cnt[val] += 1
        
        adj = buildAdj(cnt)
        buff = []
        ans = dfs(buff, n, cnt, adj)
        return ans
