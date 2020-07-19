#define MAXN 100001
int rightmostContinuousDuplicatePos[MAXN];

class Solution {
public:
    int closestToTarget(vector<int>& arr, int target) {
      /*
      test case #1
      [5,89,79,44,45,79]
      965
      
      */
      int n = arr.size();
      for (int i = n-1; i >= 0; --i) {
        if (i == n-1) {
          rightmostContinuousDuplicatePos[i] = i;
          continue;
        }
        if (arr[i] != arr[i+1]) {
          rightmostContinuousDuplicatePos[i] = i;
        } else {
          rightmostContinuousDuplicatePos[i] = rightmostContinuousDuplicatePos[i+1];
        }
      }
      
      int ans = INT_MAX;
      for (int i = rightmostContinuousDuplicatePos[0]; i < n; i = rightmostContinuousDuplicatePos[i]+1) {
        if (arr[i] == target) {
          return 0;
        } else if (arr[i] < target) {
          int candidate = abs(arr[i] - target);
          if (candidate < ans) {
            ans = candidate;
          }
          // once got an "arr[i]" strictly less than "target", there's no need to move on along "i"
        } else {
          // arr[i] > target
          int tmp = arr[i];
          int candidate = abs(tmp - target);
          if (candidate < ans) {
            ans = candidate;
          }
          for (int j = i+1; j < n; j = rightmostContinuousDuplicatePos[j]+1) {
            tmp &= arr[j]; // each such step removes at least one bit "1" from tmp, thus the "j-iteration" takes O(log2(max{arr[i]})) time-complexity
            if (tmp == target) {
              return 0;
            }
            candidate = abs(tmp - target);
            if (candidate < ans) {
              ans = candidate;
            }
            if (tmp < target) {
              // once "tmp" becomes strictly less than "target", there's no need to move on along "j"
              break;
            }
          }
        }
      }
      
      return ans;
    }
};
