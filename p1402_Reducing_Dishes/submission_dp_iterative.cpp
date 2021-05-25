int const MAXN = 500;
bool const debug = false;

/*
test cases
[-1,-8,0,5,-7]
[4,3,2]
[-1,-4,-5]
[-2,5,-1,0,3,-3]
*/
class Solution {
public:
    int maxSatisfaction(vector<int>& satisfaction) {
        sort(satisfaction.begin(), satisfaction.end());
        
        int n = satisfaction.size();
        vector<vector<int>> dp(n, vector<int>(n+1, 0)); // dp[i][m] is the max score by choosing EXACTLY "m" of "satisfaction[0, ..., i]"
        
        // init
        dp[0][1] = satisfaction[0];
        
        // loop
        for (int i = 1; i < n; ++i) {
            for (int m = 1; m <= min(i+1, n); ++m) {
                dp[i][m] = dp[i-1][m-1] + m*satisfaction[i];
                
                if (m <= i) {
                    dp[i][m] = max(dp[i][m], dp[i-1][m]);    
                }
                
                if (debug) printf("dp[i:%d][m:%d] == %d\n", i, m, dp[i][m]);
            }
        }
        
        int ans = 0;
        for (int m = 0; m <= n; ++m) {
            ans = max(ans, dp[n-1][m]);
        }
        
        return ans;
    }
};
