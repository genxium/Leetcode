#include <string.h>
#include <stdio.h>
#include <vector>
#define MAXL 1000
using namespace std;

int dp[MAXL][MAXL]; // dp[i][j] is the longest common subarray/substring width ENDED EXACTLY at t1[i] & t2[j] respectively.

class Solution {
public:
    int findLength(vector<int>& t1, vector<int>& t2) {
        int l1 = t1.size(), l2 = t2.size();
        if (l1 <= 0 || l2 <= 0) return 0;
        
        // Init.
        memset(dp, 0, sizeof(dp));
        
        // Loop. 
        int maxDpVal = 0;
        for (int i = 0; i < l1; ++i) {
            for (int j = 0; j < l2; ++j) {
                if (t1[i] != t2[j]) {
                    dp[i][j] = 0;
                } else {
                    if (0 < i && 0 < j) {
                        dp[i][j] = dp[i-1][j-1] + 1; 
                    } else {
                        dp[i][j] = 1;
                    }
                }
                if (dp[i][j] > maxDpVal) {
                    maxDpVal = dp[i][j];
                }
            }
        }
        
        return maxDpVal;
    }
};

int main() {
  int t1Arr[] = {1, 2, 3, 2, 1};
  int t2Arr[] = {3, 2, 1, 4, 7};
  vector<int> t1(t1Arr, t1Arr + sizeof(t1Arr)/sizeof(int));
  vector<int> t2(t2Arr, t2Arr + sizeof(t2Arr)/sizeof(int));
  Solution s;
  int res = s.findLength(t1, t2);
  printf("%d\n", res);
  return 0;
}
