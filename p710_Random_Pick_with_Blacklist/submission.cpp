class Solution {
private:
    vector<pair<int, int>> whitelist;
    vector<int> whitelistLengthPrefixSums;
public:
    Solution(int N, vector<int>& blacklist) {
        sort(blacklist.begin(), blacklist.end());
        int l = 0, r = 0;
        for (int i = 0; i < blacklist.size(); ++i) {
            r = blacklist[i];
            if (l < r) {
                whitelist.push_back({l, r});
                int effLength = r-l;
                whitelistLengthPrefixSums.push_back(effLength + (whitelistLengthPrefixSums.empty() ? 0 : whitelistLengthPrefixSums.back()));
            }
            l = blacklist[i]+1;
        }
        
        // wrap up 
        r = N;
        if (l < r) {
            whitelist.push_back({l, r});
            int effLength = r-l;
            whitelistLengthPrefixSums.push_back(effLength + (whitelistLengthPrefixSums.empty() ? 0 : whitelistLengthPrefixSums.back()));
        }
    }
    
    int pick() {
        auto it = upper_bound(whitelistLengthPrefixSums.begin(), whitelistLengthPrefixSums.end(), rand()%whitelistLengthPrefixSums.back());
        auto group = whitelist[(int)(it-whitelistLengthPrefixSums.begin())];
        int l = group.first, r = group.second; // r is open-end
        return l+rand()%(r-l);
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(N, blacklist);
 * int param_1 = obj->pick();
 */
