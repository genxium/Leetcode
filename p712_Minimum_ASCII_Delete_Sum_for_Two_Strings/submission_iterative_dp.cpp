bool debug = false;
int const MAXN = 1000;
int const INVALID = -1;
int dp[MAXN+1][MAXN+1]; // "dp[i][j] == x" means that it takes "x" cost to make "s1[0,...,i-1] == s2[0,...,j-1]" by deletion

/*
test cases
"sea"
"eat"
"delete"
"leet"
"abcde"
"fgh"
*/
class Solution {
public:
    int minimumDeleteSum(string s1, string s2) {
        memset(dp, INVALID, sizeof dp);
        
        int s1Len = s1.length(), s2Len = s2.length();
        
        // init
        dp[0][0] = 0;
        dp[1][0] = (int)s1[0];
        dp[0][1] = (int)s2[0];
        for (int i = 1; i <= s1Len; ++i) {
            dp[i][0] = dp[i-1][0] + s1[i-1];
        }
        for (int j = 1; j <= s2Len; ++j) {
            dp[0][j] = dp[0][j-1] + s2[j-1];
        }
        
        // loop
        for (int i = 1; i <= s1Len; ++i) {
            for (int j = 1; j <= s2Len; ++j) {
                int cand1 = dp[i-1][j-1] + (
                    s1[i-1] == s2[j-1] 
                    ? 
                    0 // no deletion
                    : 
                    (s1[i-1] + s2[j-1]) // delete both
                );
                if (INVALID == dp[i][j] || dp[i][j] > cand1) {
                    dp[i][j] = cand1;
                    if (debug) printf("#1, dp[i:%d][j:%d] is updated to %d\n", i, j, dp[i][j]);
                }
                
                int cand2 = dp[i-1][j] + s1[i-1]; // delete only "s1[i-1]"
                if (INVALID == dp[i][j] || dp[i][j] > cand2) {
                    dp[i][j] = cand2;
                    if (debug) printf("#2, dp[i:%d][j:%d] is updated to %d\n", i, j, dp[i][j]);
                }
                
                int cand3 = dp[i][j-1] + s2[j-1]; // delete only "s2[j-1]"
                if (INVALID == dp[i][j] || dp[i][j] > cand3) {
                    dp[i][j] = cand3;
                    if (debug) printf("#3, dp[i:%d][j:%d] is updated to %d\n", i, j, dp[i][j]);
                }
            }    
        }
        
        return dp[s1Len][s2Len];
    }
};
