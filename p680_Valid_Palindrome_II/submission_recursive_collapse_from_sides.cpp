bool const debug = false;

/*
test cases
"aba"
"abca"
"aaaaaaaac"
"abc"
"yd"
*/
class Solution {
public:
    bool dfs(int iFromLeft, int rightOffset, string& s) {
        if (iFromLeft >= (s.length()-1-(iFromLeft-rightOffset))) return true;
        
        if (s[iFromLeft] != s[s.length()-1-(iFromLeft-rightOffset)]) {
            if (rightOffset != 0) return false;
            // delete @iFromLeft, from now on, the right counterpart becomes "n-1-(i-1)"
            bool cand1 = dfs(iFromLeft+1, 1, s);
            // delete @(n-1-iFromLeft), from now on, the right counterpart becomes "n-1-(i+1)"
            bool cand2 = dfs(iFromLeft, -1, s);
            
            return (cand1 | cand2);
        }
        
        return dfs(iFromLeft+1, rightOffset, s);
    }

    bool validPalindrome(string s) {
        return dfs(0, 0, s);
    }
};
