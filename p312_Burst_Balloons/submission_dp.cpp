#define MAXN 502
/*
"dp[l][r] == x" means that the max profit by bursting "nums[l+1, ..., r-1]" is "x", note that the range excludes "l" & "r"

dp[l][r] = max{nums[l]*nums[k]*nums[r] + dp[l][k] + dp[k][r] | l < k < r where nums[k] is the last among [l+1, r-1] to burst}

to initialize, "dp[l][l] = 0 && dp[l][l+1] = 0" because there's no valid "k" 
*/
int dp[MAXN][MAXN];

class Solution {
public:
    int maxCoins(vector<int>& nums) {
      memset(dp, 0, sizeof(dp));
      nums.insert(nums.begin(), 1);
      nums.push_back(1);
      int n = nums.size();
      
      // as defined above, "len == 0" and "len == 1" are meaningless for "dp[l][r]"
      for (int len = 2; len <= n; ++len) {
        int lUpper = n-len; // to guarantee that "r <= n-1"
        for (int l = 0; l <= lUpper; ++l) {
          int r = l + len - 1;
          int candidate = 0;
          //printf("checking len:%d, l:%d, r:%d\n", len, l, r);
          for (int k = l+1; k <= r-1; ++k) {
            candidate = nums[l]*nums[k]*nums[r] + dp[l][k] + dp[k][r];
            //printf("checking l:%d, k:%d, r:%d, candidate == %d\n", l, k, r, candidate);
            if (candidate > dp[l][r]) dp[l][r] = candidate;
          }
        }
      }
      return dp[0][n-1];
    }
};
