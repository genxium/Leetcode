/*
test cases
"mississippi"
"mis*is*p*.."
"mississippi"
"mis*is*p*..*"
"abc"
"abc."
"abc"
"abc.*"
*/
class Solution {
public:
    bool isNormalCh(char const &ch) {
        return ch >= 'a' && ch <= 'z';    
    }

    bool isSingleMatch(char const &lhs, char const &rhs) {
        if (lhs == '\0' || rhs == '\0') {
            /*
             * It's important that we don't match '.' with '\0', for example
             * ```
             * p = abc.*\0
             *        ^
             * s = abc\0
             *        ^
             * ```
             * SHOULDN'T be a match, or our codes might need nasty termination checks.
             *
             * It's one of the reasons that "c_str" is used in this solution for convenience.
             */
            return false;
        }
        if (isNormalCh(lhs)) {
            return lhs == rhs;
        } else {
            return lhs == '.';
        }
    }

    bool isMatchExtra(char const *s, char const *p, bool &result) {
        if (result) return true;
        if (*s == '\0' && *p == '\0') return result = true;
        
        if (*p == '\0') return false; // "s" hasn't reached end
        if (*(p+1) == '*') {
            // can always try deeming "(p, p+1)" as empty
            if (isMatchExtra(s, p+2, result)) return result = true;
    
            if (isSingleMatch(*p, *s)) {
                // in this case "(p, p+1)" can match onwards without paying a move, and will recurse on "s" if "isSingleMatch(*p, *(s+1))" during the next recursion 
                if (isMatchExtra(s+1, p, result)) return result = true;
            }
        } else {
            if (isSingleMatch(*p, *s)) {
                if (isMatchExtra(s+1, p+1, result)) return result = true;
            }
        }
        return false;
    }
    
    bool isMatch(string s, string p) {
        bool ans = false;
        isMatchExtra(s.c_str(), p.c_str(), ans);
        return ans;
    }
};
