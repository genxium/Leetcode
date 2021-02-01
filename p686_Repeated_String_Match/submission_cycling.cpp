bool const debug = false;
int const INVALID = -1;

/*
test cases
"abcd"
"cdabcdab"
"a"
"aa"
"a"
"a"
"abc"
"wxyz"
"abcd"
"cdabcdabcdabcdabcdabcdabcdabcda"
*/
class Solution {
public:
    int extraCount(string &a, string &b, string &pattern) {
        // Guaranteed that "a.length() >= b.length()" and that "a" is a repetition of "pattern".
        char const *p1 = strstr(a.c_str(), b.c_str());
        if (NULL != p1) return 0;
        a += pattern;
        char const *p2 = strstr(a.c_str(), b.c_str());
        return (NULL != p2 ? 1 : INVALID);
    }
    
    int repeatedStringMatch(string a, string b) {
        string pattern = a;
        int baseCnt = 1;
        while (a.length() < b.length()) {
            a += pattern;
            ++baseCnt;
        }
        
        int exCnt = extraCount(a, b, pattern);
        if (INVALID == exCnt) return INVALID;
        return baseCnt+exCnt;
    }
};
