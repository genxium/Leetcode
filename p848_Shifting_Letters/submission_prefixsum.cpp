bool debug = false;
typedef int64_t LL;

class Solution {
public:
    string shiftingLetters(string S, vector<int>& shifts) {
        int n = shifts.size();
        vector<int> rps(n, 0); // reverse prefixsum
        
        rps[n-1] = shifts[n-1];
        for (int i = n-2; i >= 0; --i) {
            rps[i] = rps[i+1] + shifts[i];
            rps[i] %= 26;
        }
        
        for (int i = 0; i < n; ++i) {
            int chIdx = S[i]-'a';
            chIdx = (chIdx+rps[i])%26;
            if (debug) printf("rps[i:%d] == %d\n", i, rps[i]);
            S[i] = 'a'+chIdx;
        }
        
        return S;
    }
};
