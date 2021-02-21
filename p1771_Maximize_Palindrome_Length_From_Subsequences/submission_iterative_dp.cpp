bool debug = false;
int const MAXN = 2000;
// Just "dp[NOT_AT_BORDER | AT_BORDER][MAXN][MAXN]" as the traditional LCS approach
int notAtBorder[MAXN][MAXN]; 
int atBorder[MAXN][MAXN];

class Solution {
public:
    int longestPalindrome(string word1, string word2) {
        string s = word1 + word2;
        int n = s.length();
        if (debug) printf("s: %s\n", s.c_str());
        
        memset(notAtBorder, 0, sizeof(notAtBorder));
        memset(atBorder, 0, sizeof(atBorder));
        int ans = 0;

        // init
        for (int l = 0; l < n; ++l) {
            atBorder[l][l] = 1;
        }

        // loop, LCS dp
        for (int len = 2; len <= n; ++len) {
            for (int l = 0; l <= n-len; ++l) {
                int r = l+len-1;
                notAtBorder[l][r] = max(notAtBorder[l][r], notAtBorder[l+1][r]);
                notAtBorder[l][r] = max(notAtBorder[l][r], notAtBorder[l][r-1]);
                notAtBorder[l][r] = max(notAtBorder[l][r], atBorder[l+1][r]);
                notAtBorder[l][r] = max(notAtBorder[l][r], atBorder[l][r-1]);
                if (s[l] == s[r]) {
                    atBorder[l][r] = max(atBorder[l][r], 2 + atBorder[l+1][r-1]);
                    atBorder[l][r] = max(atBorder[l][r], 2 + notAtBorder[l+1][r-1]);
                }
                // The resulted palindrome must start at "word1" and end at "word2".
                if (atBorder[l][r] > 1 && l < word1.length() && r >= word1.length()) {
                    ans = max(ans, atBorder[l][r]);
                    if (debug) printf("ans updated to %d by atBorder[l:%d][r:%d]\n", ans, l, r);                        
                }
            }  
        }
        
        return ans;
    }
};
