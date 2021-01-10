bool const debug = true;

/*
test cases
"cdbcbbaaabab"
4
5
"aabbaaxybbaabb"
5
4
"baabb"
5
4
*/
class Solution {
public:
    int settle(stack<char> &stk, int x, int y) {
        int aCnt = 0, bCnt = 0;
        while (!stk.empty()) {
            'a' == stk.top() ? ++aCnt : ++bCnt;
            stk.pop();
        }
        
        return min(x, y)*min(aCnt, bCnt);
    }
    
    int maximumGain(string s, int x, int y) {
        /*
        The key idea is that any "ch not in ['a', 'b']" is a delimiter for scoring, thus we only have to score the segregated substrings patterned "a...ab....ba..ab...b", which should be scored greedily, e.g. if "x > y && count('a') < count('b')", try to consume each 'a' by "ab" until cannot do it.
        */
        stack<char> stk;
        int ans = 0;
        for (auto &ch : s) {
            switch (ch) {
                case 'a':
                    {
                        if (y > x && !stk.empty() && 'b' == stk.top()) {
                            stk.pop();
                            ans += y;
                        } else {
                            stk.push(ch);
                        }
                    }
                    break;
                case 'b':
                    {
                        if (x > y && !stk.empty() && 'a' == stk.top()) {
                            stk.pop();
                            ans += x;
                        } else {
                            stk.push(ch);
                        }
                    }
                    break;
                default:
                    {
                        ans += settle(stk, x, y);
                    }
                    break;
            }   
        }
        
        // wrap up
        ans += settle(stk, x, y);
        return ans;
    }
};
