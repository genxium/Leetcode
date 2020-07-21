#define MAXM 100
#define MAXG 101
#define MAXN 21
#define INF INT_MAX
#define INVALID -1
/*
"dp[houseId][groupCountByFar][colorId] == x" means that the minimum total cost to paint "houseId" to "colorId" such that it joins/forms group "groupCountByFar", is "x"
*/
int dp[MAXM][MAXG][MAXN];

class Solution {
public:
    int minCost(vector<int>& origColors, vector<vector<int>>& cost, int m, int n, int targetGroupCount) {
      int ans = INF;
      
      for (int houseId = 0; houseId < m; ++houseId) {
        for (int groupCountByFar = 1; groupCountByFar <= targetGroupCount; ++groupCountByFar) {
          for (int currentColorId = 1; currentColorId <= n; ++currentColorId) {
            dp[houseId][groupCountByFar][currentColorId] = INF;
          }
        }
      }
      
      // init
      for (int targetColorId = 1; targetColorId <= n; ++targetColorId) {
        if (0 < origColors[0]) {
          if (origColors[0] != targetColorId) {
            dp[0][1][targetColorId] = INF;
          } else {
            dp[0][1][targetColorId] = 0;
          }
        } else {
          dp[0][1][targetColorId] = cost[0][targetColorId-1];
        }
      }
      
      // loop 
      for (int houseId = 1; houseId < m; ++houseId) {
        for (int groupCountByFar = 1; groupCountByFar <= targetGroupCount; ++groupCountByFar) {
          for (int currentColorId = 1; currentColorId <= n; ++currentColorId) {
            for (int prevColorId = 1; prevColorId <= n; ++prevColorId) {
              int prevHouseId = houseId-1;
              int prevGroupCountByFar = (currentColorId == prevColorId ? groupCountByFar : (groupCountByFar-1));
              if (0 >= prevGroupCountByFar) {
                // no need to consider
                continue;
              }
              if (INF == dp[prevHouseId][prevGroupCountByFar][prevColorId]) {
                // no need to consider
                continue;
              }
              //printf("Deriving dp[houseId:%d][groupCountByFar:%d][currentColorId:%d] from dp[prevHouseId:%d][prevGroupCountByFar:%d][prevColorId:%d]:%d\n", houseId, groupCountByFar, currentColorId, prevHouseId, prevGroupCountByFar, prevColorId, dp[prevHouseId][prevGroupCountByFar][prevColorId]);
              if (0 < origColors[houseId]) {
                if (origColors[houseId] != currentColorId) {
                  dp[houseId][groupCountByFar][currentColorId] = INF;
                } else {
                  dp[houseId][groupCountByFar][currentColorId] = min(dp[houseId][groupCountByFar][currentColorId], dp[prevHouseId][prevGroupCountByFar][prevColorId]);
                }
              } else {
                // 0 == origColors[nextHouseId]
                dp[houseId][groupCountByFar][currentColorId] = min(dp[houseId][groupCountByFar][currentColorId], dp[prevHouseId][prevGroupCountByFar][prevColorId] + cost[houseId][currentColorId-1]);
              }
            }
          }
        }
      }
      
      for (int lastHouseColorId = 1; lastHouseColorId <= n; ++lastHouseColorId) {
        ans = min(ans, dp[m-1][targetGroupCount][lastHouseColorId]);
      }
      
      if (INF == ans) {
        return INVALID;
      }
      return ans;
    }
};
