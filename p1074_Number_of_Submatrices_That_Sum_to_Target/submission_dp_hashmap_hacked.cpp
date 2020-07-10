#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,tune=native")
#define MAXN 300
int verticalSumsForRow[MAXN+1][MAXN]; // verticalSumsForRow[h][x]
static int partialSumCount[200'000'002];
const int OFFSET = 100'000'000; 

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
      
      //printf("yUpper == %d, xUpper == %d\n", yUpper, xUpper);
      unordered_map<int, int> partialSumCount;
      memset(verticalSumsForRow, 0, sizeof(verticalSumsForRow));

      for (int y = 0; y < yUpper; ++y) {
        // Deliberately traversing "h" in the reverse order.
        for (int h = y+1; h >= 1; --h) {
          partialSumCount[OFFSET] = 1;
          int partialSum = 0;
          for (int x = 0; x < xUpper; ++x) {
            verticalSumsForRow[h][x] = (verticalSumsForRow[h-1][x] + matrix[y][x]);
            partialSum += verticalSumsForRow[h][x];
            int counterTarget = partialSum - target;   
            ans += partialSumCount[counterTarget + OFFSET];
            ++partialSumCount[partialSum + OFFSET];
          }
          
          // reset "partialSumCount"
          partialSum = 0;
          for (int x = 0; x < xUpper; ++x) {
            verticalSumsForRow[h][x] = (verticalSumsForRow[h-1][x] + matrix[y][x]);
            partialSum += verticalSumsForRow[h][x];
            partialSumCount[partialSum + OFFSET] = 0;
          }
        }
      }  
      
      return ans;
    }
};

static const auto magic = []() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  return nullptr;
}();
