#define MAXN 101
bool dp[MAXN][MAXN]; // "dp[i][cnt] == true" means that at exactly "s[i]" we can have accumulated "cnt" by counting '{' as +1 and '}' as -1

class Solution {
public:
    bool checkValidString(string s) {
        memset(dp, false, sizeof(dp));
        int l = s.length();
        if (0 == l) return true;
        
        // init
        if (s[0] == ')') {
            return false;
        } 
        dp[0][1] = true;
        if (s[0] == '*') {
            dp[0][0] = true;
        }
        
        // loop
        for (int i = 1; i < l; ++i) {
            for (int cnt = 0; cnt < (MAXN-1); ++cnt) {
                if (s[i] == ')') {
                    dp[i][cnt] = dp[i-1][cnt+1];   
                } else if (s[i] == '(') {
                    if (cnt > 0) {
                        dp[i][cnt] = dp[i-1][cnt-1];                        
                    }
                } else {
                    // s[i] == '*'
                    dp[i][cnt] |= dp[i-1][cnt]; // as empty string
                    dp[i][cnt] |= dp[i-1][cnt+1]; // as '}'
                    if (cnt > 0) {
                        dp[i][cnt] |= dp[i-1][cnt-1];
                    }
                }
            }
        }
        
        return dp[l-1][0];
    }
};
