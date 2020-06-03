#include <stdio.h>
#include <string.h>
#include <vector>
#define MAXL 1000
#define MAX 9999999
using namespace std;

int dp[MAXL][MAXL]; // dp[i][j] = "min path sum that ends exactly at grid[i][j]"

class Solution {
public:
    int minPathSum(vector< vector<int> >& grid) {         
      /*
      * Why not just go toward "min{grid[i+1][j], grid[i][j+1]}" when currently standing at grid[i][j]?
      *
      * Consider the following case.
      * 1  1  1  1  99
      * 3  2  2  2  99
      * 1  2  2  97 99
      * 1  2  2  97 0
      * 1  1  1  1  1
      *
      * The greedy path is as follows.
      * _  _  _  _ #  
      * #  #  #  | #
      * #  #  #  | #
      * #  #  #  | _
      * #  #  #  # |
      *
      * The actual optimal path is as follows.
      * |  #  #  # #  
      * |  #  #  # #
      * |  #  #  # #
      * |  #  #  # #
      * _  _  _  _ _
      *
      */
      int rowsCount = grid.size();
      if (0 >= rowsCount) {
        return 0; 
      }
      int indentCount = 4;
      memset(dp, MAX, sizeof(dp));     
      dp[0][0] = grid[0][0];
      int colsCount;
      for (int i = 0; i < rowsCount; ++i) {
        colsCount = grid[i].size();
        for (int j = 0; j < colsCount; ++j) {
          // From the problem description.
          // ```
          // You can only move either down or right at any point in time.
          // ```
          //printf("minPathSum\n%*sat grid[%d][%d]\n", indentCount, "", i, j);
          if (i >= 1 && dp[i-1][j] != MAX) {
            int candidate = dp[i-1][j] + grid[i][j];
            if (candidate < dp[i][j]) {
                dp[i][j] = candidate;
            }
          }
          if (j >= 1 && dp[i][j-1] != MAX) {
            int candidate = dp[i][j-1] + grid[i][j];
            if (candidate < dp[i][j]) {
                dp[i][j] = candidate;
            }
          }
          // printf("%*sdp[%d][%d] = %d\n", indentCount, "", i, j, dp[i][j]);
        }
      }
      return dp[rowsCount-1][colsCount-1];
    }
};

int main() {
  vector< vector<int> > grid;
  int row1Arr[] = {1, 1, 1, 1, 99};
  vector<int> row1(row1Arr, row1Arr+sizeof(row1Arr)/sizeof(int));
  int row2Arr[] = {3, 2, 2, 2, 99};
  vector<int> row2(row2Arr, row2Arr+sizeof(row2Arr)/sizeof(int));
  int row3Arr[] = {1, 2, 2, 97, 99};
  vector<int> row3(row3Arr, row3Arr+sizeof(row3Arr)/sizeof(int));
  int row4Arr[] = {1, 2, 2, 97, 0};
  vector<int> row4(row4Arr, row4Arr+sizeof(row4Arr)/sizeof(int));
  int row5Arr[] = {1, 1, 1, 1, 1};
  vector<int> row5(row5Arr, row5Arr+sizeof(row5Arr)/sizeof(int));
  grid.push_back(row1);
  grid.push_back(row2);
  grid.push_back(row3);
  grid.push_back(row4);
  grid.push_back(row5);

  Solution s;
  int res = s.minPathSum(grid);
  printf("%d\n", res);
  return 0;
}
