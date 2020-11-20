int const MAXN = 20;

class Solution {  
public:
  int findTargetSumWays(vector<int>& nums, int S) {
    int n = nums.size();
    unordered_map<int, int> dp[MAXN]; // "dp[i][BIAS+sum] == x" means that there're "x" ways to make that "nums[i, ... n)" result in "sum" 

    // init
    ++dp[n-1][nums[n-1]];
    ++dp[n-1][-nums[n-1]];

    // loop
    for (int len = 2; len <= n; ++len) {
      int l = n-len;
      if (l < 0) break;
      // printf("l:%d\n", l);
      for (auto it : dp[l+1]) {
        int existingSum = it.first;
        int existingSumCount = it.second;
        // printf("\texistingSum:%d, existingSumCount:%d\n", existingSum, existingSumCount);
        dp[l][+nums[l]+existingSum] += existingSumCount;
        dp[l][-nums[l]+existingSum] += existingSumCount;
        // printf("\tupdated dp[l:%d][sum:%d] == %d\n", l, +nums[l]+existingSum, dp[l][+nums[l]+existingSum]);
        // printf("\tupdated dp[l:%d][sum:%d] == %d\n", l, -nums[l]+existingSum, dp[l][-nums[l]+existingSum]);
      }
    }

    int ans = dp[0][S];
    return ans;
  }
};
