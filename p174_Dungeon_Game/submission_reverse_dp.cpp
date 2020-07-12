#define MAXN 1001
#define INVALID 0
int dp[MAXN][MAXN]; // Re-purpose "dp[y][x] == h" means that to reach (yUpper-1, xUpper-1) from (y, x), the minimum initial health needed at (y, x) is "h"

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
      int ans = 1;
      int yUpper = dungeon.size(), xUpper = dungeon[0].size();
      // init  
      dp[yUpper-1][xUpper - 1] = 1 - dungeon[yUpper-1][xUpper-1];
      if (dp[yUpper-1][xUpper-1] < 1) {
        dp[yUpper-1][xUpper-1] = 1;
      }
      for (int y = yUpper-1; y >= 0; --y) {
        for (int x = xUpper-1; x >= 0; --x) {
            if (y == yUpper-1 && x == xUpper-1) continue;
            dp[y][x] = INT_MAX;
            if (y+1 < yUpper) {
                if (dp[y][x] > dp[y+1][x] - dungeon[y][x]) {
                    dp[y][x] = dp[y+1][x] - dungeon[y][x];
                }
            }
            if (x+1 < xUpper) {
                if (dp[y][x] > dp[y][x+1] - dungeon[y][x]) {
                    dp[y][x] = dp[y][x+1] - dungeon[y][x];
                }
            }
            if (dp[y][x] < 1) {
                dp[y][x] = 1;
            }
            //printf("dp[y:%d][x:%d] is updated to %d\n", y, x, dp[y][x]);
        }
      }
      return dp[0][0];
    }
};
