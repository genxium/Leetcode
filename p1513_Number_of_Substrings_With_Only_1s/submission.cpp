#define MAXN 100001
#define MOD 1000000007u
class Solution {
public:
    int numSub(string s) {
        unsigned int consecLen = 0u;
        unsigned int ans = 0u;
        for (auto &c : s) {
            if (c == '0') {
                if ((consecLen & 1u)) {
                    unsigned int temp = ((consecLen+1u) >> 1u);
                    ans += (consecLen*temp)%MOD;
                    ans %= MOD;
                } else {
                    unsigned int temp = (consecLen >> 1u);
                    ans += (temp*(consecLen+1u))%MOD;
                    ans %= MOD;
                }
                consecLen = 0u;
            } else {
                ++consecLen;
            }
        }
        if (consecLen > 0u) {
            if ((consecLen & 1u)) {
                unsigned int temp = ((consecLen+1u) >> 1u);
                ans += (consecLen*temp)%MOD;
                ans %= MOD;
            } else {
                unsigned int temp = (consecLen >> 1u);
                ans += (temp*(consecLen+1u))%MOD;
                ans %= MOD;
            }
        }
        return (int)ans;
    }
};
