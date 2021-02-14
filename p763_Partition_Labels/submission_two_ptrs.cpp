class Solution {
public:
    vector<int> partitionLabels(string S) {
        unordered_map<char, int> rightmostIdx;
        for (int i = 0; i < S.length(); ++i) {
            rightmostIdx[S[i]] = i;
        }
        
        vector<int> ans;
        
        int maxRightmostOfChInRange = rightmostIdx[S[0]];
        int l = 0, r = 1;
        while (r < S.length()) {
            while (r <= maxRightmostOfChInRange) {
                maxRightmostOfChInRange = max(maxRightmostOfChInRange, rightmostIdx[S[r]]);
                ++r;
            } 
            // now that "r > maxRightmostOfChInRange"
            ans.push_back(r-l);
            l = r;
            if (l < S.length()) {
                maxRightmostOfChInRange = rightmostIdx[S[l]];
            }
        }
        
        return ans;
    }
};
