/*
test cases
[0,1,2,3,0]
[0,1,1,3,3,0]
[0,2,1,0,3,0]
*/
class Solution {
public:
    int minSideJumps(vector<int>& obstacles) {
        int n = obstacles.size()-1;
        vector<vector<int>> dp(n+1, vector<int>(3, INT_MAX)); // dp[i] is the min #side-jumps when reached point "i" at lane j, note that j is 0-based
        
        // init
        dp[0][1] = 0; 
        dp[0][0] = dp[0][2] = 1; // jumps from the middle lane
        
        // loop 
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (obstacles[i] == j+1) {
                    // Again note that j is 0-based.
                    continue;
                }
                for (int k = 0; k < 3; ++k) {
                    if (INT_MAX == dp[i-1][k]) continue;
                    if (obstacles[i-1] == j+1 && obstacles[i] == k+1) continue; // couldn't side-jump
                    dp[i][j] = min(dp[i-1][k] + (j == k ? 0 : 1), dp[i][j]);
                }
            }
        }
        
        int ans = min(dp[n][0], dp[n][1]);
        ans = min(ans, dp[n][2]);
        return ans;
    }
};
