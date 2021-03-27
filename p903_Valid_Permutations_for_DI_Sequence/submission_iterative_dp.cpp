int const MAXN = 200, MAXK = 200;
typedef uint64_t ULL;
typedef int64_t LL;
LL const INVALID = -1, MOD = 1000000007;
/*
The number of solutions to fulfill "s[0, ..., i]" with "i+2" nums, i.e. [0, ..., i+1], where the last num is the "k-th smallest". Note that the "i+2" nums need not be continuous by this definition. All is 0-based.

If we pick the "k-th smallest" of the "i+2" elements to be the last of the new sequence, and if "S[i-1] == D", then the last element of the rest "i+1" elements to form the prefix sequence is limited, i.e. the "t-th" in the diagram below is limited.

```
0,1,.........,i-1,        i
.................,       'D'
                   t-th of "i+1" elements, k-th of "i+2" elements
```

dp[i][k] = S[i] == 'D' ? SUM{dp[i-1][t >= k]} : SUM{dp[i-1][t < k]}
*/
LL dp[MAXN+1][MAXK+1];

/*
test cases
"D"
"DD"
"DID"
"I"
*/
class Solution {
public:
    int numPermsDISequence(string S) {
        memset(dp, 0, sizeof dp);
        int n = S.length();
        
        if (S[0] == 'D') {
            dp[0][0] = 1; // [1, 0], the last element is the 0-th
        } else {
            dp[0][1] = 1; // [0, 1], the last element is the 1-st
        }
                
        for (int i = 1; i < n; ++i) {
            // Recall that for "S[0, ..., i]", there're "i+2" nums "[0, ..., i, i+1]" to permute. 
            for (int k = 0; k <= i+1; ++k) {
                if (S[i] == 'D') {
                    for (int t = k; t <= i; ++t) {
                        dp[i][k] += dp[i-1][t];
                        dp[i][k] %= MOD;
                    }
                } else {
                    for (int t = 0; t < k; ++t) {
                        dp[i][k] += dp[i-1][t];
                        dp[i][k] %= MOD;
                    }
                }
            }
        }
        
        LL ans = 0;
        for (int k = 0; k <= n; ++k) {
            ans += dp[n-1][k];
            ans %= MOD;
        }
        
        return ans;
    }
};
