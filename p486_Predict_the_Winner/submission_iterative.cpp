/*
The solution here is very similar to 
- that of https://github.com/genxium/Leetcode/tree/master/p877_Stone_Game,
- that of https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons, and
- that of https://github.com/genxium/Leetcode/tree/master/p1563_Stone_Game_V, and
- that of https://github.com/genxium/Leetcode/blob/master/p1547_Minimum_Cost_to_Cut_a_Stick/submission_dp_iterative.cpp.

"dp[l][r] == x" means that if "Player#1" is facing piles[l, ..., r], she can finally get up to "x" stones by following the rules to take turns.
*/
#define MAXN 20
int dp[MAXN][MAXN];
int ps[MAXN]; // "PrefixSum"

class Solution {
public:
    bool PredictTheWinner(vector<int>& nums) {
        memset(dp, 0, sizeof(dp));
        memset(ps, 0, sizeof(ps));
        
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (i == 0) ps[i] = nums[i];
            else ps[i] = ps[i-1]+nums[i];
        }
        
        for (int i = 0; i < n; ++i) {
            dp[i][i] = nums[i];
        }
        
        for (int len = 2; len <= n; ++len) {
            for (int l = 0; l <= n-len; ++l) {
                int r = l+len-1;
                int subarrSum = ps[r]-(l > 0 ? ps[l-1] : 0);
                // if taking nums[l], "Player#2" will play optimally to obtain "dp[l+1][r]"
                dp[l][r] = max(dp[l][r], nums[l]+(subarrSum-nums[l]-dp[l+1][r]));
                
                // if taking nums[r], "Player#2" will play optimally to obtain "dp[l][r-1]"
                dp[l][r] = max(dp[l][r], nums[r]+(subarrSum-nums[r]-dp[l][r-1]));
            }
        }
        
        //printf("Overall Player#1 can optimally obtain %d\n", dp[0][n-1]);
        int overallOptimallyByPlayer2 = ps[n-1] - dp[0][n-1];
        return (dp[0][n-1] >= overallOptimallyByPlayer2);
    }
};
