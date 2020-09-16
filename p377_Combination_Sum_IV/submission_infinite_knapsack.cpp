#define LL uint64_t

class Solution {
public:
    int combinationSum4(vector<int>& nums, int target) {
        vector<LL> dp(target+1, 0u); // "dp[sum] == x" means that there're "x" ways to make up "sum"
        int n = nums.size();
        // init
        dp[0] = 1u;
        
        // loop
        for (int sum = 1; sum <= target; ++sum) {
            for (int i = 0; i < n; ++i) {
                if (sum < nums[i]) continue;
                dp[sum] += dp[sum-nums[i]];              
            }
        }
        return dp[target];
    }
};
