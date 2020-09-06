#define MAXN 500
/*
The solution here is very similar to 
- that of https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons, and
- that of https://github.com/genxium/Leetcode/blob/master/p1547_Minimum_Cost_to_Cut_a_Stick/submission_dp_iterative.cpp, and
- that of https://github.com/genxium/Leetcode/tree/master/p1563_Stone_Game_V.

"dp[l][r] == x" means that if Alex is facing piles[l, ..., r], she can finally get up to "x" stones by following the rules to take turns.
*/
int dp[MAXN][MAXN];
int ps[MAXN];

class Solution {
public:
    bool stoneGame(vector<int>& piles) {
        memset(dp, 0, sizeof(dp));
        memset(ps, 0, sizeof(ps));
        
        int n = piles.size();
        for (int i = 0; i < n; ++i) {
            if (i == 0) ps[i] = piles[i];
            else ps[i] = ps[i-1]+piles[i];
        }
        
        for (int i = 0; i < n; ++i) {
            dp[i][i] = piles[i];
        }
        
        for (int len = 2; len <= n; ++len) {
            for (int l = 0; l <= n-len; ++l) {
                int r = l+len-1;
                int subarrSum = ps[r]-(l > 0 ? ps[l-1] : 0);
                // if taking piles[l], Lee will play optimally to obtain "dp[l+1][r]"
                dp[l][r] = max(dp[l][r], piles[l]+(subarrSum-piles[l]-dp[l+1][r]));
                
                // if taking piles[r], Lee will play optimally to obtain "dp[l][r-1]"
                dp[l][r] = max(dp[l][r], piles[r]+(subarrSum-piles[r]-dp[l][r-1]));
            }
        }
        //printf("Overall Alex can optimally obtain %d\n", dp[0][n-1]);
        int overallOptimallyByLee = ps[n-1] - dp[0][n-1];
        return (dp[0][n-1] > overallOptimallyByLee);
    }
};
