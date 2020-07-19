class Solution {
public:
    int solveN2(vector<int>& nums) {
      int ans = 1;
      /*
      "dp[i] == x" means that the LIS ended exactly at nums[i] has length "x"
      
      dp[i] = max{ 1+dp[j] | j < i && nums[j] < nums[i] }
      */
      int n = nums.size();
      if (0 == n) return 0;
      
      vector<int> dp(n, 0);
      // init
      dp[0] = 1;
      
      // loop
      for (int i = 1; i < n; ++i) {
        int maxBefore = 0;
        for (int j = 0; j < i; ++j) {
          if (nums[j] < nums[i] && maxBefore < dp[j]) maxBefore = dp[j];
        }
        dp[i] = 1+maxBefore;
        if (ans < dp[i]) ans = dp[i];
      }
      return ans;
    }
  
    int solveNlogn(vector<int>& nums) {
      /*
      Consider that "nums[] == {100, 200, 300, 400, 1, 201, 3, 500}"
                                                    ^
      , our "sortedCache" grew to {100, 200, 300, 400} successfully, and would be updated to {1, 200, 300, 400}, then {1, 200, 201, 400}, and then {3, 200, 201, 400}, and finally {3, 200, 201, 400, 500}.
      */
      vector<int> sortedCache;
      for (auto &num : nums) {
        auto lower = lower_bound(sortedCache.begin(), sortedCache.end(), num);
        if (lower == sortedCache.end()) {
          sortedCache.push_back(num);
        } else {
          *lower = num;
        }
      }
      return sortedCache.size();
    }
  
    int lengthOfLIS(vector<int>& nums) {
      int ans = solveNlogn(nums);
      return ans;
    }
};
