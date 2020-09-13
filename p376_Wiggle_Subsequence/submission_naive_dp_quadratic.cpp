#define MAXN 5000
#define PEAK +1
#define TROUGH 0
int dp[MAXN][PEAK+1]; // "dp[i][PEAK | TROUGH] == x" means that the maximum length of the WiggleSubsequence ending EXACTLY at "nums[i]" is "x"
/*
dp[i][PEAK] = max{1+dp[j][TROUGH] | j < i && nums[j] < nums[i]}
dp[i][TROUGH] = max{1+dp[j][PEAK] | j < i && nums[j] > nums[i]}
*/

class Solution {
public:
    int wiggleMaxLength(vector<int>& nums) {
        memset(dp, 0, sizeof(dp));
        int n = nums.size();
        if (0 == n) {
            return 0;
        }
        
        int ans = 1;
        
        // init
        dp[0][PEAK] = 1;
        dp[0][TROUGH] = 1;
        
        // loop
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    dp[i][PEAK] = max(dp[i][PEAK], 1+dp[j][TROUGH]);
                }
                if (nums[j] > nums[i]) {
                    dp[i][TROUGH] = max(dp[i][TROUGH], 1+dp[j][PEAK]);
                }
                ans = max(ans, dp[i][PEAK]);
                ans = max(ans, dp[i][TROUGH]);
            }
        }
        
        return ans;
    }
};
