int const MAXN = 1005;
int dp[MAXN][MAXN]; // "dp[i][j] == x" means that the longest common subsequence is of length "x" by using "s[0, ..., i]" and "rev_s[0, ..., j]" (but NOT NECESSARILY using 's[i]' or 'rev_s[j]')

class Solution {
public:
    int longestPalindromeSubseq(string s) {
        string rev_s = s;
        reverse(rev_s.begin(), rev_s.end());
        memset(dp, 0, sizeof dp);
        int n = s.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int p = (s[i] == rev_s[j]);
                int q = (i-1 >= 0 && j-1 >= 0 ? dp[i-1][j-1] : 0);
                dp[i][j] = p+q;
                if (i-1 >= 0) {
                    dp[i][j] = max(dp[i][j], dp[i-1][j]);
                }
                if (j-1 >= 0) {
                    dp[i][j] = max(dp[i][j], dp[i][j-1]);
                }
            }
        }
        
        return dp[n-1][n-1];
    }
};
