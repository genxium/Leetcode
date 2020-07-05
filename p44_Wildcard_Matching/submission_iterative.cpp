#define MAXN 1500
bool dp[MAXN][MAXN];

class Solution {
public:
    bool isSingleMatch(string &s, string &p, int sIdx, int pIdx) {
        if (pIdx == p.length() && sIdx == s.length()) return true;
        if (sIdx == s.length()) return false;
        if (p[pIdx] == '?') return true;
        return (p[pIdx] == s[sIdx]);
    }

    bool isMatch(string s, string p) {
        int sl = s.length(), pl = p.length();
        memset(dp, false, sizeof(dp));
        dp[pl][sl] = true;
        for (int pIdx = pl-1; pIdx >= 0; --pIdx) {
            for (int sIdx = sl; sIdx >= 0; --sIdx) {
                if (p[pIdx] == '*') {
                    /*
                    By definition in this case

                    - dp[pIdx][sIdx] = OR{dp[pIdx+1][sIdx'] | sIdx' >= sIdx} --(1)
                    , then by math induction

                    - dp[pIdx][sIdx+1] = OR{dp[pIdx+1][sIdx''] | sIdx'' >= sIdx+1} --(2)
                    , therefore 

                    - dp[pIdx][sIdx] = OR{dp[pIdx][sIdx+1], dp[pIdx+1][sIdx]} --(3)
                    , where both elements of the right-hand-side of (3) are available by now.
                    */
                    dp[pIdx][sIdx] = (dp[pIdx][sIdx+1] || dp[pIdx+1][sIdx]); 
                } else {
                    dp[pIdx][sIdx] = isSingleMatch(s, p, sIdx, pIdx) && dp[pIdx+1][sIdx+1];
                }
            }
        } 
        return dp[0][0];
    }
};
