#define MAXN 1000
int dp[MAXN][MAXN]; // "dp[y][x] == h" means that by reaching (y, x) the maximum health remained is "h"

class Solution {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
      /*
      test case #1
      [[100]]
      
      test case #2
      [[-1]]
      
      test case #3
      [[-2,-3,3],[-5,-10,1],[10,30,-5]]
      */
      int maxTotalDeduction = 1;
      for (auto &row : dungeon) {
        for (auto &val : row) {
          if (val >= 0) continue;
          maxTotalDeduction += -val;
        }
      }
      int yUpper = dungeon.size();
      int xUpper = dungeon[0].size();
      int l = 1, r = maxTotalDeduction+1;
      int ans = INT_MAX;
      
      while (l < r) {
        memset(dp, 0, sizeof(dp));
        int initH = ((l + r) >> 1);
        dp[0][0] = initH + dungeon[0][0];
        for (int y = 0; y < yUpper; ++y) {
          for (int x = 0; x < xUpper; ++x) {
            if (0 == y && 0 == x) continue;
            int candidate = INT_MIN;
            if (y-1 >= 0) {
              if (dp[y-1][x] > 0 && dp[y-1][x] > candidate) candidate = dp[y-1][x];
            }
            if (x-1 >= 0) {
              if (dp[y][x-1] > 0 && dp[y][x-1] > candidate) candidate = dp[y][x-1];
            }
            if (INT_MIN == candidate) {
              dp[y][x] = candidate; // that none of the predecessors is ">= 0"
            } else {
              dp[y][x] = candidate+dungeon[y][x];
            }
            //printf("dp[y:%d][x:%d] is updated to %d\n", y, x, candidate);
          }
        }
        if (dp[yUpper-1][xUpper-1] > 0) {
          //printf("\tinitH:%d works\n", initH);
          ans = initH;
          r = initH;
        } else {
          //printf("\tinitH:%d doesn't work\n", initH);
          l = initH+1;
        }
      }
      return ans;
    }
};
