#define INVALID -1 
#define MAXN 10000
int dp[MAXN]; // 
/*
"dp[i] == x" means that selling exactly at i-th day can reach max profix "x"

dp[i] = prices[i]-min(0 <= t <= i-1) or max{dp[j] + prices[i]-min(t) | j+2 <= i-1, j+2 <= t <= i-1}
*/

class Solution {
public:
    int maxProfit(vector<int>& prices) {
      /*
      test case #0
      []
      
      test case #1
      [1,2,3,0,2]
      
      test case #2
      [2,4,1,7,11]
      */
      memset(dp, 0, sizeof(dp));
      int n = prices.size(), ans = 0;
      // "i" starts from 1, because it's impossible to sell at 0-th day
      for (int i = 1; i < n; ++i) {
        int revMin = prices[i-1];
        dp[i] = prices[i] - revMin;
        for (int j = i-3; j >= 1; --j) {
          if (revMin > prices[j+2]) {
            revMin = prices[j+2];
          }
          int candidate = dp[j] + (prices[i] - revMin); 
          if (candidate > dp[i]) {
            dp[i] = candidate;
            //printf("dp[i:%d] is updated to %d by dp[j:%d]:%d\n", i, dp[i], j, dp[j]);
          }
        }
        int tUpper = (i < 3 ? i : 3);
        for (int t = tUpper; t >= 0; --t) {
          if (revMin > prices[t]) {
            revMin = prices[t];
          }
        }
        int candidate = (prices[i] - revMin);
        if (candidate > dp[i]) {
          dp[i] = candidate;
          //printf("dp[i:%d] is updated to %d by revMin:%d\n", i, dp[i], revMin);
        }
        
        if (ans < dp[i]) {
          ans = dp[i];
        }
      }
      return ans;
    }
};
