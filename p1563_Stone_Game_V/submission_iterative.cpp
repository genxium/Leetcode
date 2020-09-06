#define MAXN 500
/*
Note that in this problem, "stoneValue" cannot be re-ordered.

The solution here is very similar to 
- that of https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons, and
- that of https://github.com/genxium/Leetcode/tree/master/p877_Stone_Game, and
- that of https://github.com/genxium/Leetcode/blob/master/p1547_Minimum_Cost_to_Cut_a_Stick/submission_dp_iterative.cpp.

"dp[l][r] == x" means that by using "stoneValue[l, ..., r]" Alice can obtain at most "x" points

dp[l][l] = 0 for each "l"
dp[l][k] = max{
            if (sum(stoneValue[l, ..., k]) < sum(stoneValue[k+1, ..., r])) sum(stoneValue[l, ..., k]+dp[l][k]
            elif (sum(stoneValue[l, ..., k]) > sum(stoneValue[k+1, ..., r])) sum(stoneValue[k+1, ..., r]+dp[k+1][r]
            else sum(stoneValue[l, ..., k]+max(dp[l][k], dp[k+1][r])
            | k}
*/
int dp[MAXN][MAXN]; 
int ps[MAXN]; // "PrefixSum"

class Solution {
public:
    int stoneGameV(vector<int>& stoneValue) {
        memset(dp, 0, sizeof(dp));
        memset(ps, 0, sizeof(ps));
        
        int n = stoneValue.size();
        for (int i = 0; i < n; ++i) {
            ps[i] = (i > 0 ? ps[i-1]+stoneValue[i] : stoneValue[i]);
        }
        
        for (int len = 2; len <= n; ++len) {
            //printf("len: %d\n", len);
            for (int l = 0; l <= (n-len); ++l) {
                int r = l+len-1;
                for (int k = l; k <= r-1; ++k) {
                    int subarrSumL = ps[k] - (l > 0 ? ps[l-1] : 0);
                    int subarrSumR = ps[r] - ps[k];
                    
                    if (subarrSumL < subarrSumR) {
                        dp[l][r] = max(dp[l][r], subarrSumL+dp[l][k]);
                    } else if (subarrSumL > subarrSumR) {
                        dp[l][r] = max(dp[l][r], subarrSumR+dp[k+1][r]);
                    } else {
                        // subarrSumL == subarrSumR
                        dp[l][r] = max(dp[l][r], subarrSumL + max(dp[l][k], dp[k+1][r]));
                    }
                }
                //printf("dp[l:%d][r:%d] is figured out to %d\n", l, r, dp[l][r]);
            }
        }
        
        return dp[0][n-1];
    }
};
