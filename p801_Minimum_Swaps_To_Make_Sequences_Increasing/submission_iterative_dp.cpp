bool debug = true;
int const MAXN = 1000;
int const INVALID = -1;
int const KEEP = 0, SWAP = 1;
int dp[MAXN][2]; // Minimum number of swaps to make both "A[0, ..., i] & B[0, ..., i]" strictly increasing if "A[i], B[i]" are either kept or swapped.

class Solution {
public:
    int minSwap(vector<int>& A, vector<int>& B) {
        memset(dp, INVALID, sizeof dp);
        int n = A.size();
        
        // init
        dp[0][KEEP] = 0; dp[0][SWAP] = 1;
        
        // loop
        for (int i = 1; i < n; ++i) {
            if (INVALID != dp[i-1][KEEP]) {
                if (A[i-1] < A[i] && B[i-1] < B[i]) {
                    // was KEEP, try to KEEP
                    if (INVALID == dp[i][KEEP]) dp[i][KEEP] = dp[i-1][KEEP];
                    else dp[i][KEEP] = min(dp[i][KEEP], dp[i-1][KEEP]);
                }
                
                if (A[i-1] < B[i] && B[i-1] < A[i]) {
                   // was KEEP, try to SWAP 
                    if (INVALID == dp[i][SWAP]) dp[i][SWAP] = 1+dp[i-1][KEEP];
                    else dp[i][SWAP] = min(dp[i][SWAP], 1+dp[i-1][KEEP]);
                }
            }
            
            if (INVALID != dp[i-1][SWAP]) {
                if (A[i-1] < B[i] && B[i-1] < A[i]) {
                   // was SWAP, try to KEEP 
                    if (INVALID == dp[i][KEEP]) dp[i][KEEP] = dp[i-1][SWAP];
                    else dp[i][KEEP] = min(dp[i][KEEP], dp[i-1][SWAP]);
                }
                
                if (A[i-1] < A[i] && B[i-1] < B[i]) {
                    // was SWAP, try to SWAP
                    if (INVALID == dp[i][SWAP]) dp[i][SWAP] = 1+dp[i-1][SWAP];
                    else dp[i][SWAP] = min(dp[i][SWAP], 1+dp[i-1][SWAP]);
                }
            }
        }
        
        return min(dp[n-1][KEEP], dp[n-1][SWAP]);
    }
};
