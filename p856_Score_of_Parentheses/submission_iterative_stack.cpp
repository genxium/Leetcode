bool debug = false;

typedef pair<char, int> PCI;

/*
test cases
"()"
"(())"
"()()"
"(()(()))"
"()(())"
*/
class Solution {
public:
    int scoreOfParentheses(string S) {
        stack<PCI> stk;
        for (auto ch : S) {
            if ('(' == ch) {
                stk.push({ch, 0});
            } else {
                if (stk.top().first == '(') {
                    stk.pop();
                    stk.push({'\0', 1});
                    while (stk.size() >= 2 && stk.top().second > 0) {
                        auto v = stk.top(); stk.pop();
                        auto &u = stk.top();
                        if (u.first == '(') {
                            stk.push(v);
                            break;
                        } else {
                            u.second += v.second;                        
                        }
                    }
                } else {
                    while (stk.size() >= 2 && stk.top().second > 0) {
                        auto v = stk.top(); stk.pop();
                        auto &u = stk.top();
                        if (u.first == '(') {
                            u.first = '\0';
                            u.second = v.second*2;
                            break;
                        } else {
                            u.second += v.second;                        
                        }
                    }
                }
            }
            
            if (debug) printf("stk.top().second == %d\n", stk.top().second);
        }
        
        while (stk.size() >= 2) {
            auto v = stk.top(); stk.pop();
            auto &u = stk.top();
            u.second += v.second;
        }
        return stk.top().second;
    }
};
