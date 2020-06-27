#define MAXN 1000
int dp[MAXN]; // "dp[n] = x" means that there're "x" different "BST"s for "n"

class Solution {
public:
    int numTrees(int n) {
        if (0 == n) {
            return 0;
        }
        
        memset(dp, 0, sizeof(dp));
        
        // init 
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i <= n; ++i) {
            /*
            - i being root: dp[i-1] on the left subtree
            */
            dp[i] = dp[i-1];

            for (int j = 1; j < i; ++j) {
                /*
                - any j < i being root 
                    -- [1, j-1] on the left subtree, dp[j-1]
                    -- [j-1, i] on the right subtree, dp[i-j]
                */
                dp[i] += dp[j-1]*dp[i-j];
            }
        }
        return dp[n];
    }
};
