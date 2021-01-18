int const MAXN = 1000;
int const INVALID = -1;
int dp[MAXN][MAXN]; // "dp[l][r] == x" means that the first-hand facing "stones[l, ..., r]" can finally lead by at most "x" points, if both played optimally
int ps[MAXN+1]; // prefixSums
bool const debug = true;

/*
test cases
[5,3,1,4,2]
[7,90,5,1,100,10,10,2]
*/
class Solution {
public:
    int dfs(int l, int r) {
        if (INVALID != dp[l][r]) return dp[l][r];
        if (l == r) return dp[l][r] = 0;
        int cand1 = (ps[r+1] - ps[l+1]); // // remove stones[l], thus leading by "cand1" points
        cand1 -= dfs(l+1, r); // The second-hand would lead by "dp[l+1][r]" points in the remaining stones
        
        int cand2 = (ps[r] - ps[l]); // remove stones[r], thus leading by "cand2" points
        cand2 -= dfs(l, r-1);
        
        return dp[l][r] = max(cand1, cand2);
    }
    
    int stoneGameVII(vector<int>& stones) {
        memset(dp, INVALID, sizeof(dp));
        memset(ps, 0, sizeof(ps));
        
        int n = stones.size();
        for (int i = 0; i < n; ++i) {
            ps[i+1] = ps[i] + stones[i];
        }
        
        return dfs(0, n-1);
    }
};
