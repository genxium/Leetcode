/*
See https://github.com/genxium/Leetcode/tree/master/p1074_Number_of_Submatrices_That_Sum_to_Target for the origin of the solution used here.
*/

#define MAXN 5000
int verticalSumsForRow[MAXN][MAXN]; // verticalSumsForRow[h][x]
int horizontalSumsForCol[MAXN][MAXN]; // horizontalSumsForCol[w][y]

class Solution {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
      int yUpper = matrix.size();
      if (0 >= yUpper) return 0;
      int xUpper = matrix[0].size();
      
      int ans = INT_MIN;
      //printf("yUpper: %d, xUpper: %d\n", yUpper, xUpper);
      
      if (yUpper < xUpper) {
        memset(verticalSumsForRow, 0, sizeof(verticalSumsForRow));
        for (int y = 0; y < yUpper; ++y) {
          // Deliberately traversing "h" in the reverse order to guarantee that we derive "verticalSumsForRow[h][x] of `currentRound`" from "verticalSumsForRow[h-1][x] of `previousRound`".
          for (int h = y+1; h >= 1; --h) {
            set<int> partialSumSet;
            // init
            int partialSum = 0;
            partialSumSet.insert(partialSum);
            
            // loop
            for (int x = 0; x < xUpper; ++x) {
              verticalSumsForRow[h][x] = (verticalSumsForRow[h-1][x] + matrix[y][x]);
              partialSum += verticalSumsForRow[h][x];
              int lowerTarget = partialSum - k;   
              // We seek a "previous & minimum partialSum >= lowerTarget".
              
              auto lb = partialSumSet.lower_bound(lowerTarget);
              if (lb != partialSumSet.end()) {
                ans = max(ans, partialSum-(*lb));
              }
              partialSumSet.insert(partialSum);
            }
          }
        }
      } else {
        memset(horizontalSumsForCol, 0, sizeof(horizontalSumsForCol));
        for (int x = 0; x < xUpper; ++x) {
          // Deliberately traversing "w" in the reverse order to guarantee that we derive "horizontalSumsForCol[w][y] of `currentRound`" from "horizontalSumsForCol[w-1][y] of `previousRound`".
          for (int w = x+1; w >= 1; --w) {
            set<int> partialSumSet;
            // init
            int partialSum = 0;
            partialSumSet.insert(partialSum);
            
            // loop
            for (int y = 0; y < yUpper; ++y) {
              horizontalSumsForCol[w][y] = (horizontalSumsForCol[w-1][y] + matrix[y][x]);
              partialSum += horizontalSumsForCol[w][y];
              int lowerTarget = partialSum - k;   
              // We seek a "previous & minimum partialSum >= lowerTarget".
              
              auto lb = partialSumSet.lower_bound(lowerTarget);
              if (lb != partialSumSet.end()) {
                ans = max(ans, partialSum-(*lb));
              }
              partialSumSet.insert(partialSum);
            }
          }
        }
      }
      
      return ans;
    }
};
