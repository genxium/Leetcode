/*
According to https://github.com/genxium/Leetcode/tree/master/p560_Subarray_Sum_Equals_K, the 1-d version of this problem can be solved in O(n) time-complexity.

Assume that for the y-th row, we have 
```
verticalSum[y][x][h] = sum{matrix[y'][x] | y-(h-1) <= y' <= y} = verticalSum[y-1][x][h-1] + matrix[y][x] 
```
, then we can figure out the answer to this problem by traversing dimensions [y][h].

*/
#define MAXN 300
int partialSum[MAXN];
int PRE = 0, CUR = 1;
int verticalSumsForRow[2][MAXN][MAXN]; // verticalSumsForRow[PRE|CUR][h][x]

class Solution {
public:
  int subarraySum(int nums[], int xUpper, int k) {
        int res = 0;
        map<int, int> partialSumCount;
    
        int idx = 0;
        partialSum[idx++] = nums[0];
        partialSumCount[partialSum[idx-1]] = 1;
        if (nums[0] == k) {
            ++res;
        }
        
        for (int i = 1; i < xUpper; ++i) {
            int newPartialSum = partialSum[i-1] + nums[i];
            if (newPartialSum == k) {
                ++res;
            }
            int target = newPartialSum - k;   
            //printf("For partialSum[%d] == %d, target == %d\n", i, newPartialSum, target);
            
            // By far, each element in "partialSumCount" has a position "j < i". 
            map<int, int>::iterator counterpart = partialSumCount.find(target);
            if (counterpart != partialSumCount.end()) {
                res += counterpart->second;
            }
            
            partialSum[idx++] = (newPartialSum); 
            if (partialSumCount.find(newPartialSum) != partialSumCount.end()) {
                partialSumCount[newPartialSum] += 1;
            } else {
                partialSumCount[newPartialSum] = 1;
            }
        }
       
        return res;
    }
  
    int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
      int yUpper = matrix.size();
      if (0 >= yUpper) return 0;
      memset(verticalSumsForRow, 0, sizeof(verticalSumsForRow));
      int ans = 0;
      int xUpper = matrix[0].size();
      for (int y = 0; y < yUpper; ++y) {
        for (int h = 1; h <= y+1; ++h) {
          if (1 == h) {
            for (int x = 0; x < xUpper; ++x) {
              verticalSumsForRow[CUR][h][x] = (matrix[y][x]);
            }
          } else {
            for (int x = 0; x < xUpper; ++x) {
              verticalSumsForRow[CUR][h][x] = (verticalSumsForRow[PRE][h-1][x] + matrix[y][x]);
            }   
          }
          /*
          printf("At y:%d, matching target:%d to verticalSumsForRow[CUR][h:%d]:\n", y, target, h);
          for (int x = 0; x < xUpper; ++x) {
            printf("%d ", verticalSumsForRow[CUR][h][x]);
          }
          printf("\n");
          */
          int matchedCount = subarraySum(&(verticalSumsForRow[CUR][h][0]), xUpper, target);
          ans += matchedCount;
        }
        swap(PRE, CUR);
      }
      return ans;
    }
};
