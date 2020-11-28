int const MAXL = 1000;

int dp[MAXL][MAXL]; // dp[i][j] is the longest common subsequence up to t1[i], t2[j], but NOT NECESSARILY INCLUDING either t1[i] or t2[j]

class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int l1 = text1.length(), l2 = text2.length();
        if (l1 <= 0 || l2 <= 0) return 0;
        
        // Init.

        int maxDpVal = 0;
        memset(dp, 0, sizeof(dp));
        dp[0][0] = (text1[0] == text2[0] ? 1 : 0);
        maxDpVal = max(dp[0][0], maxDpVal);
        
        for (int i = 1; i < l1; ++i) {
            dp[i][0] = (text1[i] == text2[0] ? 1 : dp[i-1][0]);
            maxDpVal = max(dp[i][0], maxDpVal);
        }
        for (int j = 1; j < l2; ++j) {
            dp[0][j] = (text1[0] == text2[j] ? 1 : dp[0][j-1]);
            maxDpVal = max(dp[0][j], maxDpVal);
        }
        
        // Loop.
        for (int i = 1; i < l1; ++i) {
            for (int j = 1; j < l2; ++j) {
                int candidate = 0;
                    
                candidate = dp[i - 1][j];    
                if (dp[i][j] < candidate) dp[i][j] = candidate;
             
                candidate = dp[i][j - 1];
                if (dp[i][j] < candidate) dp[i][j] = candidate;

                if (text1[i] == text2[j]) {
                    candidate = dp[i - 1][j - 1] + 1;
                    if (dp[i][j] < candidate) dp[i][j] = candidate;
                }
                // printf("dp[%d][%d] == %d\n", i, j, dp[i][j]);
                if (dp[i][j] > maxDpVal) {
                    maxDpVal = dp[i][j];
                }
            }
        }
        return maxDpVal;
    }
};
