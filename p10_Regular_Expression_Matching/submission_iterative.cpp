#define MAXN 2000
bool dp[MAXN][MAXN]; // "dp[pIdx][sIdx] == true" means that "p[pIdx, ..., p.length())" matches "s[sIdx, ..., s.length())"

class Solution {
public:
    bool isNormalCh(char const &ch) {
        return ch >= 'a' && ch <= 'z';    
    }

    bool isSingleMatch(string &p, int &pIdx, string &s, int &sIdx) {
        if (pIdx == p.length()) return false;
        if (sIdx == s.length()) return false;
        if (isNormalCh(p[pIdx])) {
            return p[pIdx] == s[sIdx];
        } else {
            if (p[pIdx] == '.') {
                return true;
            }
            return false;
        }
    }
    
    bool isMatch(string s, string p) {
        /*
        test case #1
        "mississippi"
        "mis*is*p*.."
        false
        
        test case #2
        "mississippi"
        "mis*is*p*..*"
        true
        
        test case #3
        "abc"
        "abc."
        false
        
        test case #4
        "abc"
        "abc.*"
        true
        
        test case #5
        "abc"
        "abc.*."
        true
        
        test case #6
        "abc"
        "abc.*.."
        false
        
        test case #7
        "abc"
        "abc..*."
        false
        */

        memset(dp, 0, sizeof(dp));
        dp[p.length()][s.length()] = true;

        for (int pIdx = p.length(); pIdx >= 0; --pIdx) {
          if (p[pIdx] == '*') continue;
          for (int sIdx = s.length(); sIdx >= 0; --sIdx) {
            if (pIdx == p.length() && sIdx == s.length()) {
                continue;
            }
            bool singleMatched = isSingleMatch(p, pIdx, s, sIdx);
            if (pIdx+1 < p.length() && p[pIdx+1] == '*') {
              if (singleMatched) {
                dp[pIdx][sIdx] = (dp[pIdx][sIdx+1] || dp[pIdx+2][sIdx]);
                /*
                * A little tricky thought here. Assume that "sIdxUpper" suffices "true == isSingleMatch(p[Idx], s[sIdx, ..., sIdxUpper)) && false == isSingleMatch(p[Idx], s[sIdxUpper])".
                *
                ////////////////////////////////////
                * p = "a.b*c"
                *        ^  
                *          _
                * s = "adbbbbbbbbc"
                *        ^
                *        _________
                *
                * Intuitively, 
                *   ```
                *   dp[pIdx][sIdx] = OR{dp[pIdx+2][sIdxPrime] | sIdx <= sIdxPrime <= sIdxUpper}
                *   ```
                * which seems to require a for-loop.  
                *
                * Notice that by mathematical induction, we have 
                *   ```
                *   dp[pIdx][sIdx] = OR{dp[pIdx+2][sIdx], dp[pIdx][sIdx+1]}
                *   ```
                * due to that by the definition here 
                *   ```
                *   dp[pIdx][sIdx+1] = OR{dp[pIdx+2][sIdxPrime] | sIdx+1 <= sIdxPrime <= sIdxUpper}
                *   ```
                * , thus a no-for-loop way to calculate "dp[pIdx][sIdx]" at current closure is found.
                *
                */   
              } else {
                /*
                ////////////////////////////////////
                * p = "a.c*c"
                *        ^  
                *          _
                * s = "adbbbbbbbbc"
                *        ^
                *        _
                *
                */
                dp[pIdx][sIdx] = dp[pIdx+2][sIdx];
              }
            } else {
              dp[pIdx][sIdx] = singleMatched && dp[pIdx+1][sIdx+1];
            }
          }
        }

        return dp[0][0];
    }
};

