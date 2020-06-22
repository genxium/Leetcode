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
            if (lhs == '.') {
                return true;
            }
            return false;
        }
    }

    bool isMatchExtra(char const *s, char const *p, bool &result) {
        if (result) {
            return true;
        }
        if (*s == '\0' && *p == '\0') {
            result = true;
            return true;
        }
        if (*p == '\0') {
            // "s" hasn't reached end
            return false;
        }
        if (*(p+1) == '*') {
            int magicOffset = 0;
            while (isSingleMatch(*p, *(s+magicOffset))) ++magicOffset;
            /*
            ////////////////////////////////////
            * p = "a.b*c"
            *        ^  
            *          _
            * s = "adbbbbbbbbc"
            *        ^
            *        _________
            *
            ////////////////////////////////////
            * p = "a.c*c"
            *        ^  
            *          _
            * s = "adbbbbbbbbc"
            *        ^
            *        _
            *
            */
            for (int i = 0; i <= magicOffset; ++i) {
                bool tmpResult = isMatchExtra(s+i, p+2, result);
                if (tmpResult == true) {
                    result = true;
                    return result;
                }
            }
        } else {
            if (isSingleMatch(*p, *s)) {
                bool tmpResult = isMatchExtra(s+1, p+1, result);
                if (tmpResult == true) {
                    result = true;
                    return result;
                }
            }
        }
        return false;
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
        */
        bool ans = false;
        isMatchExtra(s.c_str(), p.c_str(), ans);
        return ans;
    }
};
