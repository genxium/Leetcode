#define MAXN 10000
/*
* "dp[i][ROBBED | NOT_ROBBED] == x" means that ending the rob at the i-th house can reach at most "x" 
*/
int ROBBED = 0, NOT_ROBBED = 1;
int dp[MAXN][2]; 

class Solution {
public:
    int rob(vector<int>& nums) {
      /*
      test case #1
      [1]
      
      test case #2
      [2,3,2,3]
      
      test case #3
      [2,7,9,3,1]
      
      test case #4
      [1,3,1,3,100]
      
      test case #5
      [1,1]
      */
      int n = nums.size();
      if (0 == n) return 0;
      if (1 == n) return nums[0];
      
      int ans = 0;
      
      // As if the last house didn't exist.
      memset(dp, 0, sizeof(dp));
      dp[0][ROBBED] = nums[0]; dp[0][NOT_ROBBED] = 0;
      if (ans < dp[0][ROBBED]) {
        ans = dp[0][ROBBED];
      }
      for (int i = 1; i < n-1; ++i) {
        dp[i][NOT_ROBBED] = max(dp[i-1][ROBBED], dp[i-1][NOT_ROBBED]); // Note that you might have to keep consecutive "NOT_ROBBED"s to maximize it. Consider "[1,3,1,3,100]", the value of "dp[3][NOT_ROBBED]" should be drived from "dp[2][NOT_ROBBED] == dp[1][ROBBED] == 3".
        dp[i][ROBBED] = nums[i] + dp[i-1][NOT_ROBBED];
        
        if (ans < dp[i][NOT_ROBBED]) {
          ans = dp[i][NOT_ROBBED];
        }
        if (ans < dp[i][ROBBED]) {
          ans = dp[i][ROBBED];
        }
      }
      
      // As if the first house didn't exist.
      memset(dp, 0, sizeof(dp));
      dp[1][ROBBED] = nums[1]; dp[1][NOT_ROBBED] = 0;
      if (ans < dp[1][ROBBED]) {
        ans = dp[1][ROBBED];
      }
      for (int i = 2; i < n; ++i) {
        dp[i][NOT_ROBBED] = max(dp[i-1][ROBBED], dp[i-1][NOT_ROBBED]);
        dp[i][ROBBED] = nums[i] + dp[i-1][NOT_ROBBED];
        
        if (ans < dp[i][NOT_ROBBED]) {
          ans = dp[i][NOT_ROBBED];
        }
        if (ans < dp[i][ROBBED]) {
          ans = dp[i][ROBBED];
        }
      }
      
      return ans;
    }
};
