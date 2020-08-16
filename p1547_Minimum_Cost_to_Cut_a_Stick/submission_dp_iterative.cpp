#define MAXN 1000001
#define MAXCUTS 101
#define INVALID -1
int dp[MAXCUTS][MAXCUTS];
int costs[MAXCUTS];
int costsPrefixSum[MAXCUTS];
/*
- The "state" after "cuts[]" is irrelevant to the order of cutting.
- The min cost by cutting from "state1(fewer segs)" to "state2(more segs)" is identical to the min cost by merging from "state2" to "state1", assuming that "cost of merging is sum of both lengths".
- Therefore if we start from the "fully cut segments of costs[]", then the solution to this problem is analogous to that of https://github.com/genxium/Leetcode/tree/master/p312_Burst_Balloons.

"dp[l][r] == x" means that the min cost by merging "costs[l, ..., r]" is "x".

dp[l][r] = min { 
            sum(costs[l, ..., r]) + ((monopiece ? 0 : dp[l][k]) + (monopiece ? 0 : dp[k+1][r]))
            | l <= k < r 
           }

to initialize, "dp[l][l] = costs[l]" 
*/

class Solution {
public:
    int minCost(int n, vector<int>& cuts) {
        /*
        test case #1
        36
        [5,6,8,11,13,15]
        
        test case #2
        36
        [13,17,15,18,3,22,27,6,35,7,11,28,26,20,4,5,21,10,8]
        */
        sort(cuts.begin(), cuts.end());
        int cntCuts = cuts.size();
        int cntCosts = cntCuts+1;
        for (int i = 0; i < cntCosts; ++i) {
            costs[i] = (i == cntCuts ? n : cuts[i]) - (i > 0 ? cuts[i-1] : 0);
            costsPrefixSum[i] = (i > 0 ? costsPrefixSum[i-1] : 0) + costs[i];
            //printf("costs[i:%d] == %d\n", i, costs[i]);
        }

        // init        
        memset(dp, INVALID, sizeof(dp));
        for (int i = 0; i < cntCosts; ++i) {
            dp[i][i] = costs[i];
        }
        
        // loop
        for (int len = 2; len <= cntCosts; ++len) {
            int lUpper = cntCosts-len; // to guarantee that "r <= cntCosts-1"
            for (int l = 0; l <= lUpper; ++l) {
                int r = l + len - 1;
                int candidate = 0;
                //printf("checking len:%d, l:%d, r:%d\n", len, l, r);
                for (int k = l; k < r; ++k) {
                    candidate = costsPrefixSum[r] - (l > 0 ? costsPrefixSum[l-1] : 0);
                    if (k - l + 1 > 1) {
                        candidate += dp[l][k];
                    }
                    if (r - (k+1) + 1 > 1) {
                        candidate += dp[k+1][r];
                    }
                    //printf("\tchecking l:%d, k:%d, r:%d, candidate == %d\n", l, k, r, candidate);
                    if (INVALID == dp[l][r] || candidate < dp[l][r]) dp[l][r] = candidate;
                }
            }
        }

        return dp[0][cntCosts-1];
    }
};
