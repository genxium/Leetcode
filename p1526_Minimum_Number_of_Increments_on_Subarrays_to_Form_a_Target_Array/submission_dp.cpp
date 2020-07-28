#define MAXN 100000
/*
"dp[i] == x" means that to match "compressedTarget[0, ..., i]" we need at least "x" steps
*/
int dp[MAXN], compressedTarget[MAXN];

class Solution {
public:
    int minNumberOperations(vector<int>& target) {
      int n = target.size();

      int currentH = target[0], compressedTargetCnt = 0;
      for (int i = 1; i < n; ++i) {
        if (currentH != target[i]) {
          compressedTarget[compressedTargetCnt++] = currentH;
          currentH = target[i];
        }
      }
      // wrap up
      compressedTarget[compressedTargetCnt++] = currentH;
      
      // init
      dp[0] = compressedTarget[0];
      
      // loop 
      for (int i = 1; i < compressedTargetCnt; ++i) {
        if (compressedTarget[i-1] >= compressedTarget[i]) {
          // In this case we can ALWAYS increment the "OriginalInterval that should match compressedTarget[i]" together with the "OriginalInterval that should match compressedTarget[i-1]".
          dp[i] = dp[i-1];
        } else {
          dp[i] = dp[i-1] + (compressedTarget[i]-compressedTarget[i-1]);
        }
      }
      return dp[compressedTargetCnt-1];
    }
};
