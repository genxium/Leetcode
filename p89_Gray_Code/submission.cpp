class Solution {
public:
    void genGrayCode(int n, vector<int> &out) {
        if (0 == n) {
            out.push_back(0);
            return;
        }
        vector<int> prevOut;
        genGrayCode(n-1, prevOut);
        for (auto it = prevOut.begin(); it != prevOut.end(); ++it) {
            int v = *it;
            v <<= 1; 
            out.push_back(v);
        }
        
        for (auto it = prevOut.rbegin(); it != prevOut.rend(); ++it) {
            int v = *it;
            v <<= 1; 
            v |= 1;
            out.push_back(v);
        }
        return;
    }
    
    vector<int> grayCode(int n) {
        vector<int> ans;
        genGrayCode(n, ans);
        return ans;
    }
};
