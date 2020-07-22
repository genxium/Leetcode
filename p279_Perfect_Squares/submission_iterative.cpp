class Solution {
public:
  int numSquares(int n) {
    vector<int> dp; // "dp[i] == x" means that to form "i" we need add up at least "x" square numbers
    dp.assign(n+1, INT_MAX);
    // init
    dp[0] = 0; dp[1] = 1; 
    if (n >= 2) {
      dp[2] = 2; 
    }
    if (n >= 3) {
      dp[3] = 3;
    }
    // loop 
    for (int i = 4; i <= n; ++i) {
      int jUpper = floor(sqrt(i));
      for (int j = 1; j <= jUpper; ++j) {
        dp[i] = min(1+dp[i-j*j], dp[i]);
      }
    }
    return dp[n];
  }
};
