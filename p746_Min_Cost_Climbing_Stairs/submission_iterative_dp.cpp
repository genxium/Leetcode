int const MAXN = 1000;
int dp[MAXN+1]; // "dp[i] == x" means that to reach the i-th step, the minimum cost will be "x"

/*
test cases
[0,0,0,0]
[10, 15, 20]
[1, 100, 1, 1, 1, 100, 1, 1, 100, 1]
*/
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        memset(dp, -1, sizeof dp);
        int n = cost.size();
        
        // init
        dp[0] = cost[0];
        dp[1] = cost[1];
        
        for (int i = 2; i <= n; ++i) {
            dp[i] = min(dp[i-1], dp[i-2]) + (i < n ? cost[i] : 0);
        }
        
        return dp[n];
    }
};
