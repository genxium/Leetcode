int const MAXN = 1000;
int dp[MAXN][MAXN]; // "dp[l][r] == x" means that the longest "PalindromicSubsequence" in "s[l, ..., r]" is of length "x"

/*
test cases
"bbbab"
"cbbd"
"bbbabaaajiasdufoiwandojjofjadbbdbaojqijfao"
"cbbdcccdbccbbcbdcbdbbcbcbcbbdbcdb"
*/
class Solution {
public:
    int longestPalindromeSubseq(string s) {
      int n = s.length();
      memset(dp, 0, sizeof(dp));
      int ans = 1;
      
      // init
      for (int l = 0; l < n; ++l) {
        dp[l][l] = 1;
      }
      
      // loop 
      for (int len = 2; len <= n; ++len) {
        for (int l = 0; l <= n-len; ++l) {
          int r = l+len-1;
          dp[l][r] = max(dp[l+1][r], dp[l][r-1]);
          if (s[l] == s[r]) {
            dp[l][r] = max(dp[l][r], 2 + dp[l+1][r-1]);
            ans = max(ans, dp[l][r]);  
          }
        }  
      }
      return ans;
    }
};
