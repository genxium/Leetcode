class Solution {
public:
    string reverseParentheses(string s) {
        string ans = "";
        int l = s.length(), anchor = 0;
        stack<char> stk;
        for (int i = 0; i < l; ++i) {
            if (s[i] == '(') {
                stk.push(s[i]);
            } else if (s[i] == ')') {
                char tt = stk.top(); stk.pop();
                string tmp = "";
                while (tt != '(') {
                    tmp.push_back(tt);
                    if (stk.empty()) break;
                    tt = stk.top(); stk.pop();
                }
                if (stk.empty()) {
                    ans.append(tmp);
                } else {
                    for (auto &ch : tmp) {
                        stk.push(ch);
                    }
                }
            } else {
                if (stk.empty()) {
                    ans.push_back(s[i]);                    
                } else {
                    stk.push(s[i]);
                }
            }
        }
        return ans;
    }
}; 
