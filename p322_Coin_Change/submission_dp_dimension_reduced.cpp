#define MAXAMOUNT 1000000
#define INVALID -1
int dp[MAXAMOUNT+1]; // "dp[i][x] == c" means that the min number of coins to use within "coins[0, ..., i]" to compose value "x" is "c"

class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        memset(dp, INVALID, sizeof(dp));
        int n = coins.size();
        int xUpper = min(MAXAMOUNT, amount);

        // init
        dp[0] = 0;
        
        // loop
        for (int i = 0 ; i < n; ++i) {
            for (int x = coins[i]; x <= xUpper; ++x) {
                if (INVALID == dp[x-coins[i]]) continue;
                int candidate = dp[x-coins[i]]+1;
                if (INVALID == dp[x]) dp[x] = candidate;
                else dp[x] = min(dp[x], candidate);
                // printf("dp[x:%d] == %d\n", x, dp[x]);
            }
        }
        
        return dp[amount];
    }
};
