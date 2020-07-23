#define MAXN 100001
// "dp[i] == true" means that the one facing "i" stones can win if played optimally, otherwise means that one facing "i" stones definitely loose regardless of the move

bool dp[MAXN]; 

class Solution {
public:
    bool winnerSquareGame(int n) {
      memset(dp, false, sizeof(dp));
      // init
      dp[1] = true; dp[3] = true; dp[4] = true;
      
      if (n <= 4) return dp[n];
      
      for (int i = 5; i <= n; ++i) {
        for (int j = 1; j*j <= i; ++j) {
          if (dp[i-j*j] == false) {
            dp[i] = true;
            //printf("dp[i:%d] = %d by dp[i-j*j] == false @j:%d\n", i, dp[i], j);
            // once you can win here the loosing options are meaningless
            continue;
          }
        }
      }
      
      return dp[n];
    }
};
