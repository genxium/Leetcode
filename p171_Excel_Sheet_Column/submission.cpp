#define BASE 26
class Solution {
public:
    int titleToNumber(string s) {
        int n = s.length();
        int exp = 1, ans = 0;
        for (int i = n-1; i >= 0; --i) {
            int val = (s[i] - 'A') + 1;
            ans += val*exp;
            if (INT_MAX/BASE < exp) {
                break;
            } else {
                exp *= BASE;                
            }
        }
        return ans;
    }
};
