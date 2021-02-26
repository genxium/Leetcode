bool debug = false;

int const MAXN = 100;
int const MAXK = MAXN;
double dp[MAXK+1][MAXN]; // "dp[c][i]" is the largest sum of averages if "A[0, ... , i]" is divided into "c+1" groups, and the "c-th" group ends EXACTLY at A[i] (all 0-indexed).

class Solution {
public:
    double largestSumOfAverages(vector<int>& A, int K) {
        
        int n = A.size();
        for (int c = 0; c < K; ++c) {
            for (int i = 0; i < n; ++i) {
                dp[c][i] = 0.0;
            }
        }
        
        // init
        vector<int> ps = {0}; // prefix sum
        for (int i = 0; i < n; ++i) ps.push_back(ps.back() + A[i]); 
            
        // loop
        for (int c = 0; c < K; ++c) {
            for (int i = 0; i < n; ++i) {
                if (c == 0) {
                    double newAvg = (double)ps[i+1]/(i+1);
                    dp[c][i] = newAvg;
                } else {
                    for (int j = 0; j < i; ++j) {
                        double newAvg = (double)(ps[i+1]-ps[j+1])/(i-j);
                        dp[c][i] = max(dp[c][i], dp[c-1][j]+newAvg);
                    }
                }
                if (debug) printf("dp[c:%d][i:%d] is updated to %.3lf\n", c, i, dp[c][i]);
            }
        }
        
        return dp[K-1][n-1];
    }
};
