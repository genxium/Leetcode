bool debug = false;

typedef uint64_t ULL;
ULL const MOD = 1000000007;
const int MAXN = 100, MAXL = 100;
ULL dp[MAXL+1][MAXN+1]; // dp[i][j] is the #ways to use EXACTLY "j" different songs to fill the first "i" in playlist

/*
test cases
3
3
1
2
3
0
2
3
1
*/
class Solution {
public:
    int numMusicPlaylists(int N, int L, int K) {
        memset(dp, 0, sizeof dp);
        // init
        dp[0][0] = 1;

        // loop
        for (int i = 1; i <= L; ++i) {
            for (int j = 1; j <= N; ++j) {
                ULL inc = dp[i-1][j-1]*(N-(j-1)); // Use a new color
                dp[i][j] += inc;
                dp[i][j] %= MOD;
                inc = dp[i-1][j]*max(j-K, 0); // Use an old color, except for those used in the last K slots, which must be all unique
                dp[i][j] += inc;
                dp[i][j] %= MOD;
            }
        }

        return dp[L][N];
    }
};
