int const MAXN = 100000;
int const INVALID = -1;
bool const debug = true;
int dp[MAXN];

class Solution {
public:
    int maxResult(vector<int>& nums, int k) {
        memset(dp, INVALID, sizeof(dp));
        int n = nums.size();
        map<int, int> dpOrdered;
            
        // init 
        dp[0] = nums[0];
        dpOrdered[dp[0]] = 1;
        int l = 0, r = 0;

        // loop 
        for (int i = 1; i < n; ++i) {
            while (r-l+1 > k) {
                --dpOrdered[dp[l]];
                if (0 == dpOrdered[dp[l]]) dpOrdered.erase(dp[l]);
                ++l;
            }
            
            auto it = dpOrdered.rbegin();
            dp[i] = nums[i] + it->first;
            
            if (dpOrdered.find(dp[i]) != dpOrdered.end()) ++dpOrdered[dp[i]];
            else dpOrdered[dp[i]] = 1;
            ++r;
        }
        
        return dp[n-1];
    }
};
