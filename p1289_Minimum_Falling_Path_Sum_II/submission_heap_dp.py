"""
test cases
[[1,2,3],[4,5,6],[7,8,9]]
[[7]]
[[-73,61,43,-48,-36],[3,30,27,57,10],[96,-76,84,59,-15],[5,-49,76,31,-7],[97,91,61,-46,67]]
"""
debug = False        
class Solution:
    def minFallingPathSum(self, matrix: List[List[int]]) -> int:
        pre, cur = 1, 0
        n = len(matrix)
        if 1 == n:
            return matrix[0][0]
        # dp is tuples of (minFallingPathSum@column-j, j)
        # unlike in https://github.com/genxium/Leetcode/tree/master/p931_Minimum_Falling_Path_Sum, the "dp[pre | cur][j]" here DOESN'T refer to "minFallingPathSum@column-j", the whole "dp[pre | cur]" is just a min-heap
        dp = [[(0, -1) for j in range(n)] for i in range(2)] 
        for i in range(0, n):
            newH = [] # a new min-heap to be formed w.r.t. dp[pre]
            for j in range(0, n):
                cand = dp[pre][0] 
                if cand[1] == j:
                    origtop = heappop(dp[pre])
                    cand = dp[pre][0]
                    heappush(dp[pre], origtop)
                newPathSumAtJ = cand[0]+matrix[i][j]
                heappush(newH, (newPathSumAtJ, j))
                if debug: print('newPathSum[i:%d][j:%d] = %d' % (i, j, newPathSumAtJ))
            dp[cur] = newH
            cur, pre = pre, cur
            
        return dp[pre][0][0]
