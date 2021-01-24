class Solution {
public:
    vector<int> decode(vector<int>& encoded) {
        int n = encoded.size()+1, totXor = 0;
        for (int i = 1; i <= n; ++i) totXor ^= i;
        
        int totXorExceptFirst = 0;
        for (int i = 1; i < encoded.size(); i += 2) {
            totXorExceptFirst ^= encoded[i];
        }
        
        int first = totXor^totXorExceptFirst;
        
        vector<int> ans = {first};
        for (int i = 0; i < encoded.size(); ++i) ans.push_back(ans.back()^encoded[i]);
        
        return ans;
    }
};
