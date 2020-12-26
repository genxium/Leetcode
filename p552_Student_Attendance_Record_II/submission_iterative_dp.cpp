typedef int64_t LL;

int const END_WITH_A = 0;
int const END_WITH_P = 1;
int const END_WITH_SINGLE_L = 2;
int const END_WITH_DOUBLE_L = 3;
int const MAX_VALID_A = 1;

LL const INVALID = -1;
int const MAXN = 100000;
LL const MOD = 1000000007;
bool const debug = false;

LL dp[MAXN][END_WITH_DOUBLE_L+1][MAX_VALID_A+1]; // "dp[i][*][*]" represents number of rewardable records using the first [0, ..., i] letters

/*
test cases
1
2
3
10
1000
*/
class Solution {
public:
    int checkRecord(int n) {
        memset(dp, 0, sizeof dp);
        // init
        dp[0][END_WITH_A][1] = 1;
        dp[0][END_WITH_P][0] = 1;
        dp[0][END_WITH_SINGLE_L][0] = 1;

        // loop
        for (int i = 1; i < n; ++i) {
            // append 'A'
            dp[i][END_WITH_A][1] += dp[i-1][END_WITH_P][0]; dp[i][END_WITH_A][1] %= MOD;
            dp[i][END_WITH_A][1] += dp[i-1][END_WITH_SINGLE_L][0]; dp[i][END_WITH_A][1] %= MOD;
            dp[i][END_WITH_A][1] += dp[i-1][END_WITH_DOUBLE_L][0]; dp[i][END_WITH_A][1] %= MOD;
            
            // append 'P'
            for (int countOfA = 0; countOfA < 2; ++countOfA) {
                dp[i][END_WITH_P][countOfA] += dp[i-1][END_WITH_A][countOfA]; dp[i][END_WITH_P][countOfA] %= MOD;
                dp[i][END_WITH_P][countOfA] += dp[i-1][END_WITH_P][countOfA]; dp[i][END_WITH_P][countOfA] %= MOD;
                dp[i][END_WITH_P][countOfA] += dp[i-1][END_WITH_SINGLE_L][countOfA]; dp[i][END_WITH_P][countOfA] %= MOD;
                dp[i][END_WITH_P][countOfA] += dp[i-1][END_WITH_DOUBLE_L][countOfA]; dp[i][END_WITH_P][countOfA] %= MOD;
            }

            // append 'L'
            for (int countOfA = 0; countOfA < 2; ++countOfA) {
                dp[i][END_WITH_SINGLE_L][countOfA] += dp[i-1][END_WITH_A][countOfA]; dp[i][END_WITH_SINGLE_L][countOfA] %= MOD;
                dp[i][END_WITH_SINGLE_L][countOfA] += dp[i-1][END_WITH_P][countOfA]; dp[i][END_WITH_SINGLE_L][countOfA] %= MOD;

                dp[i][END_WITH_DOUBLE_L][countOfA] += dp[i-1][END_WITH_SINGLE_L][countOfA]; dp[i][END_WITH_DOUBLE_L][countOfA] %= MOD;
            }
        }

        LL ans = 0;
        for (int countOfA = 0; countOfA < 2; ++countOfA) {
            ans += dp[n-1][END_WITH_A][countOfA]; ans %= MOD;
            ans += dp[n-1][END_WITH_P][countOfA]; ans %= MOD;
            ans += dp[n-1][END_WITH_SINGLE_L][countOfA]; ans %= MOD;
            ans += dp[n-1][END_WITH_DOUBLE_L][countOfA]; ans %= MOD;
        }

        return ans;
    }
};
