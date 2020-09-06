#define INVALID -1

class Solution {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> dp; // dp[i] records the largest DivisibleSubset ended at "nums[i]"
        int n = nums.size();
        // init
        for (int i = 0; i < n; ++i) {
            dp.push_back({nums[i]});
        }
        
        int maxSize = 1;
        
        // loop
        for (int i = 0; i < n; ++i) {
            int targetJ = INVALID, maxSizeThisRound = dp[i].size();
            for (int j = 0; j < i; ++j) {
                if ((nums[i] % nums[j]) != 0) continue;
                // now that "(nums[i] % nums[j]) == 0", it's implied that "(nums[i] % (any in dp[j])) == 0", because "(nums[j] % (any in dp[j])) == 0" by definition
                if (dp[j].size() + 1 <= maxSizeThisRound) {
                    // not expanding the size of dp[i]
                    continue;
                }
                maxSizeThisRound = dp[j].size() + 1;
                targetJ = j;
            }
            //printf("For i:%d, targetJ == %d, maxSizeThisRound == %d\n", i, targetJ, maxSizeThisRound);
            if (INVALID != targetJ) {
                dp[i].clear();
                dp[i].insert(dp[i].end(), dp[targetJ].begin(), dp[targetJ].end());
                dp[i].push_back(nums[i]);
                if (dp[i].size() > maxSize) {
                    maxSize = dp[i].size();
                }
            }
        }
        
        for (int i = 0; i < n; ++i) {
            if (maxSize == dp[i].size()) {
                return dp[i];
            }
        }
        
        return {};
    }
};
