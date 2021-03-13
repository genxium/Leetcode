bool debug = false;
typedef uint64_t ULL;
ULL const MOD = 1000000007; 

/*
test cases
5
3
[2,2]
[2,3]
10
5
[2,3,5]
[6,7,8]
*/
class Solution {
public:
    int profitableSchemes(int maxC, int minProfit, vector<int>& group, vector<int>& profit) {
        /* 
        That "dp[i][c][k] == x" means that there're "x" ways to commit the first "i" crimes using "c" people and get exactly "k" points. 
        */
        vector<unordered_map<int, ULL>> dp(maxC+1); // This is only "dp[c][k]", the first dimension "i" is omitted to save space.
        
        // init
        dp[0][0] = 1; // can always commit 0 crimes by no one (now i == -1)
        
        int m = group.size();
        for (int i = 0; i < m; ++i) {
            int oldCLower = 0, oldCUpper = maxC-group[i];
            for (int oldC = oldCUpper; oldC >= oldCLower; --oldC) {
                for (auto &[oldK, oldX] : dp[oldC]) {
                    int newC = oldC+group[i]; // newC > oldC && oldC traversed in reverse-order to avoid data contamination
                    int newK = oldK+profit[i];
                    dp[newC][newK] += oldX;
                    dp[newC][newK] %= MOD;
                }
            }
        }

        ULL ans = 0;
        for (int c = 0; c <= maxC; ++c) {
            for (auto &[k, x] : dp[c]) {
                if (debug) printf("There're x:%d ways to use %d people to get %d points\n", x, c, k);
                if (k >= minProfit) {
                    ans += x;
                    ans %= MOD;
                }
            }
        }

        return ans;
    }
};
