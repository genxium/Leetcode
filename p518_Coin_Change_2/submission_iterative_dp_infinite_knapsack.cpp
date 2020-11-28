int const MAXN = 500;
int const MAXAMOUNT = 5000;

int dp[MAXN][MAXAMOUNT+1]; // "dp[i][amount] == x" means that there're "x" ways to make up "amount" by "coins[0, ..., i]"

int dp1DSpace[MAXAMOUNT+1]; 

/*
test cases
5
[1,2]
3
[2]
10
[10]
998
[1,2,5,10,20,33]
0
[]
*/
class Solution {
public:
    int solve2DSpace(int amount, vector<int>& coins) {
      if (0 == amount) return 1;
      int n = coins.size();
      if (0 == n) return 0;
      
      // init
      memset(dp, 0, sizeof(dp));
      for (int j = 0; j*coins[0] <= amount; ++j) {
        dp[0][j*coins[0]] = 1;
      }
      
      // loop
      for (int i = 1; i < n; ++i) {
        dp[i][0] = 1;
        // Deliberately traversing "p" in the direction of "coins[i] -> amount" BUT NOT REVERSELY due to the constraint that there're infinite number of each coin!
        for (int p = 0; p <= amount; ++p) {
          dp[i][p] = dp[i-1][p];
          if (p-coins[i] >= 0) {
            dp[i][p] += dp[i][p-coins[i]];  
          }
        }
      }
      
      return dp[n-1][amount];
    }
  
    int solve1DSpace(int amount, vector<int>& coins) {
      if (0 == amount) return 1;
      int n = coins.size();
      if (0 == n) return 0;
      
      // init
      memset(dp1DSpace, 0, sizeof(dp1DSpace));
      dp1DSpace[0] = 1;
      
      // loop
      for (int i = 0; i < n; ++i) {
        // Deliberately traversing "p" in the direction of "coins[i] -> amount" BUT NOT REVERSELY due to the constraint that there're infinite number of each coin!
        for (int p = coins[i]; p <= amount; ++p) {
          dp1DSpace[p] += dp1DSpace[p-coins[i]];  
        }
      }
      
      return dp1DSpace[amount];
    }
  
    int change(int amount, vector<int>& coins) {
      // return solve2DSpace(amount, coins);
      return solve1DSpace(amount, coins);
    }
};
