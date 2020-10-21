#define MAXK 500
#define MAXN 10000
#define MAXC 26
#define larger(x, y) x > y ? x : y
#define smaller(x, y) x < y ? x : y
/*
"dp[p][i][ch] == x" means that using "p" steps, the longest repeating substring of all the same 'ch' ended at "s[i]" has length "x"
*/
int dp[MAXK+1][MAXN][MAXC];

/*
test case#0
""
0

test case#1
"ABAB"
2

test case#2
"AABABBA"
1

test case#3 (this is a case where some greedy strategy will fail, if the middle substring "XAAX" was chosen to be replaced with 'B' for the first step)
"AAAABBBBXAAXBBBBAAAA"
10

test case#4
"ABBB"
2
*/
class Solution {
public:
    int characterReplacement(string s, int k) {
        int n = s.length(), ans = 0;
        memset(dp, 0, sizeof(dp));
        // init
        for (int ch = 0; ch < MAXC; ++ch) {
            for (int i = 0; i < n; ++i) {
                dp[0][i][ch] = (
                    s[i] == 'A'+ch 
                    ? 
                    1+(i > 0 ? dp[0][i-1][ch]: 0) 
                    : 
                    0
                );
                //printf("dp[p:0][i:%d][ch:%c] == %d\n", i, 'A'+ch, dp[0][i][ch]);
                ans = max(ans, dp[0][i][ch]);
            }
        }

        // loop
        for (int p = 1; p <= k; ++p) {
            for (int i = 0; i < n; ++i) {
                for (int ch = 0; ch < MAXC; ++ch) {
                    if (s[i] == 'A'+ch) {
                        int candidate = larger((i > 0 ? dp[p][i-1][ch]+1 : 1), dp[p-1][i][ch]);
                        dp[p][i][ch] = larger(dp[p][i][ch], candidate);
                    } else {
                        // s[i] != ('A'+ch)
                        int candidate = larger((i > 0 ? dp[p-1][i-1][ch]+1 : 1), dp[p-1][i][ch]);
                        dp[p][i][ch] = larger(dp[p][i][ch], candidate);
                    }
                    //printf("dp[p:%d][i:%d][ch:%c] == %d\n", p, i, 'A'+ch, dp[p][i][ch]);
                    ans = larger(ans, dp[p][i][ch]);
                }
            }
        }
        
        return ans;
    }
};
