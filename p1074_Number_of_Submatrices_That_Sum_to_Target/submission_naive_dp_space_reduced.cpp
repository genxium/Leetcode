#define MAXN 300
int verticalSumsForRow[MAXN][MAXN]; // verticalSumsForRow[h][x]
int horizontalSumsForCol[MAXN][MAXN]; // horizontalSumsForCol[w][y]

class Solution {
  public:
    int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
      /*
         test case #1
         [[0,1,0],[1,1,1],[0,1,0],[0,1,0]]
         0

         test case #2
         [[0,1,0,1],[1,1,1,-1],[0,1,0,0]]
         0
       */
      int yUpper = matrix.size();
      if (0 >= yUpper) return 0;
      int ans = 0;
      int xUpper = matrix[0].size();

      if (yUpper < xUpper) {
        memset(verticalSumsForRow, 0, sizeof(verticalSumsForRow));
        for (int y = 0; y < yUpper; ++y) {
          // Deliberately traversing "h" in the reverse order.
          for (int h = y+1; h >= 1; --h) {
            unordered_map<int, int> partialSumCount;
            int partialSum = 0;
            for (int x = 0; x < xUpper; ++x) {
              verticalSumsForRow[h][x] = (verticalSumsForRow[h-1][x] + matrix[y][x]);
              partialSum += verticalSumsForRow[h][x];
              if (partialSum == target) {
                ++ans;
              }
              int counterTarget = partialSum - target;   
              ans += partialSumCount[counterTarget];
              ++partialSumCount[partialSum];
            }
          }
        }  
      } else {
        memset(horizontalSumsForCol, 0, sizeof(horizontalSumsForCol));
        for (int x = 0; x < xUpper; ++x) {
          // Deliberately traversing "w" in the reverse order.
          for (int w = x+1; w >= 1; --w) {
            unordered_map<int, int> partialSumCount;
            int partialSum = 0;
            for (int y = 0; y < yUpper; ++y) {
              horizontalSumsForCol[w][y] = (horizontalSumsForCol[w-1][y] + matrix[y][x]);
              partialSum += horizontalSumsForCol[w][y];
              if (partialSum == target) {
                ++ans;
              }
              int counterTarget = partialSum - target;   
              ans += partialSumCount[counterTarget];
              ++partialSumCount[partialSum];
            }
          }
        }
      }

      return ans;
    }
};
