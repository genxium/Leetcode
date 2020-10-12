#define MAXN 200
#define MAXSINGLE 100
#define MAXTOTAL MAXN*MAXSINGLE
bool dp[(MAXTOTAL >> 1) + 1]; // actually "dp[i][sum]", where "dp[i][sum] == true" means that using "nums[0, ..., i]" can compose "sum"

class Solution {
public:
    bool canPartition(vector<int>& nums) {
      int n = nums.size();
      if (0 == n) return false;
      
      int maxTotalInThisCase = 0;
      for (int i = 0; i < n; ++i) maxTotalInThisCase += nums[i]; 
      
      if ((maxTotalInThisCase & 1) > 0) return false;
      
      memset(dp, false, sizeof(dp));
      
      // init
      int half = (maxTotalInThisCase >> 1); 
      dp[0] = true;
      
      // loop 
      for (int i = 0; i < n; ++i) {
        for (int sum = half; sum >= nums[i]; --sum) {
          if (dp[sum]) continue;
          dp[sum] |= dp[sum-nums[i]];
        }
      }
      
      return dp[half];
    }
};
