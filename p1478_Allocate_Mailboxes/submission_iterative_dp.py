debug = False
"""
test cases
[1,4,8,10,20]
1
[1,4,8,10,20]
3
[2,3,5,12,18]
2
"""
class Solution:
    def minDistance(self, houses: List[int], k: int) -> int:
        n = len(houses)
        houses.sort()
        oneBoxMin = [[sys.maxsize for i in range(n)] for i in range(n)] # oneBoxMin[i][j] is the minimum total distance to place only 1 mailbox for houses[i, ..., j]
        # The best single position to place the mailbox for houses[i, ..., j] is just the median position of them, for a simple proof see https://github.com/genxium/Leetcode/tree/master/p462_Minimum_Moves_to_Equal_Array_Elements_II
        for i in range(n):
            for j in range(i,n):
                bestP = statistics.median(houses[i:j+1])
                oneBoxMin[i][j] = int(sum(abs(bestP-houses[t]) for t in range(i,j+1)))
                # if debug: print('oneBoxMin[i:%d][j:%d] = %d' % (i, j, oneBoxMin[i][j]))
                
        dp = [[sys.maxsize for v in range(k+1)] for u in range(n+1)] # dp[u][v] is the minimum total distance to place v boxes for "the first u houses"
        dp[0][:] = [0]*k # no house of course 0 total distance
        dp[:][0] = [0]*n # no mailbox of course 0 total distance
        
        for u in range(1, n+1):
            for v in range(1,k+1):
                dp[u][v] = sys.maxsize
                for s in range(0, u):
                    dp[u][v] = min(dp[u][v], dp[s][v-1]+oneBoxMin[s][u-1])
                    if debug: print('dp[u:%d][v:%d] = %d' % (u, v, dp[u][v]))
                    
        return dp[n][k]
