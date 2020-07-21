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
      for (int houseId = 0; houseId < (m-1); ++houseId) {
        for (int groupCountByFar = 1; groupCountByFar <= targetGroupCount; ++groupCountByFar) {
          for (int currentColorId = 1; currentColorId <= n; ++currentColorId) {
            for (int nextColorId = 1; nextColorId <= n; ++nextColorId) {
              /*
              case #a, impossible to propagate
              */
              if (INF == dp[houseId][groupCountByFar][currentColorId]) {
                continue;
              }
              
              /*
              case #b
              */
              int nextHouseId = houseId+1;
              int nextGroupCountByFar = (currentColorId == nextColorId ? groupCountByFar : (groupCountByFar+1));
              if (0 < origColors[nextHouseId]) {
                if (origColors[nextHouseId] != nextColorId) {
                  dp[nextHouseId][nextGroupCountByFar][nextColorId] = INF;
                } else {
                  dp[nextHouseId][nextGroupCountByFar][nextColorId] = min(dp[nextHouseId][nextGroupCountByFar][nextColorId], dp[houseId][groupCountByFar][currentColorId]);
                }
              } else {
                // 0 == origColors[nextHouseId]
                dp[nextHouseId][nextGroupCountByFar][nextColorId] = min(dp[nextHouseId][nextGroupCountByFar][nextColorId], dp[houseId][groupCountByFar][currentColorId] + cost[nextHouseId][nextColorId-1]);
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
