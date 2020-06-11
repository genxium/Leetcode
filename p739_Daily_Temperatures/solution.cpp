#include <stdio.h>
#include <vector>
using namespace std;

class Solution {
  public:
    vector<int> dailyTemperatures(vector<int>& T) {
      int n = T.size();
      vector<int> toRet(n, 0);
      if (1 == n) {
        return toRet;
      }

      vector<int> dp(n, n); // In this solution, "dp[i] == j" means that "j = smallest{j' | T[j'] > T[i]} && j' > i}".

      // Init.
      dp[n-1] = n;

      // Loop.
      for (int i = n-2; i >= 0; --i) {
        if (T[i+1] > T[i]) {
          // The trivial case.
          dp[i] = i+1;
        } else {
          // T[i+1] <= T[i].
          int k = dp[i+1];
          while (k < n && T[k] <= T[i]) {
            k = dp[k];
          }

          dp[i] = k;
        }
      }

      for (int i = 0; i < n; ++i) {
        if (dp[i] != n) {
          toRet[i] = dp[i] - i;
        }
      }

      return toRet;
    }
};

int main() {
  int inputArr[] = {73, 74, 75, 71, 69, 72, 76, 73};
  vector<int> inputVec(inputArr, inputArr+sizeof(inputArr)/sizeof(int));
  Solution s;
  vector<int> res = s.dailyTemperatures(inputVec);
  for (int i = 0; i < res.size(); ++i) {
    printf("%d ", res[i]);
  }
  printf("\n");
  return 0;
}
