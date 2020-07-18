class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
      int yUpper = matrix.size();
      if (0 == yUpper) return false;
    
      int xUpper = matrix[0].size();
      if (0 == xUpper) return false;
      
      int startY = 0, startX = xUpper-1;
      for (int y = startY, x = startX; x >= 0 && y < yUpper; ) {
        if (matrix[y][x] == target) return true;
        if (matrix[y][x] > target) {
          --x;
        } else {
          // matrix[y][x] < target
          ++y;
        }
      }
          
      return false;
    }
};
