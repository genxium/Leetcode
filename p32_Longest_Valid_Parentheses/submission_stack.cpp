#define MAXL 15000
bool validMark[MAXL];
class Solution {
public:
    int longestValidParentheses(string s) {
        // test case #1 ")(()((())))))())"
        // test case #2 ")(()(((((((((((((()("
        int n = s.length(), ans = 0, lastTruePos = -1;
        vector<pair<char, int>> stk;
        stk.reserve(n); // To make use of continous memory.
        memset(validMark, false, sizeof(validMark));
        
        for (int i = 0; i < n; ++i) {
            char ch = s[i];
            switch (ch) {
                case '(':
                    stk.push_back({ch, i});
                break;
                case ')':
                    if (stk.empty()) {
                        break;
                    }
                    pair<char, int> counterpart = stk[stk.size() - 1];
                    stk.pop_back();
                    validMark[i] = true;
                    validMark[counterpart.second] = true;
                    lastTruePos = i+2;
                    //printf("Updated lastTruePos = %d by validMark[counterpart.second:%d]:%d, validMark[i:%d]:%d\n", lastTruePos, counterpart.second, validMark[counterpart.second], i, validMark[i]);
                break;
            }
        }
        int temp = 0;
        for (int i = 0; i < lastTruePos; ++i) {
            bool val = validMark[i];
            if (false == val) {
                if (temp > ans) ans = temp;
                temp = 0;
            } else {
                ++temp;
            }
        }
        // wrap up
        return ans;
    }
};
