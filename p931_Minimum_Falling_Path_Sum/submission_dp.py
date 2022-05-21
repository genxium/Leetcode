debug = False
class Solution:
    def minFallingPathSum(self, matrix: List[List[int]]) -> int:
        pre, cur = 1, 0
        n = len(matrix)
        dp = [[0 for j in range(n)] for i in range(2)]
        
        for i in range(0, n):
            for j in range(0, n):
                # straight down
                dp[cur][j] = dp[pre][j]+matrix[i][j]
                # left down
                if j-1 >= 0:
                    dp[cur][j] = min(dp[pre][j-1]+matrix[i][j], dp[cur][j])
                # right down
                if j+1 < n:
                    dp[cur][j] = min(dp[pre][j+1]+matrix[i][j], dp[cur][j])
                if debug: print('dp[i:%d][j:%d] = %d' % (i, j, dp[cur][j]))
            cur, pre = pre, cur
            
        ans = min(dp[pre])
        return ans
