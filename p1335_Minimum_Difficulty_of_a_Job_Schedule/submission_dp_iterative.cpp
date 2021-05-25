bool const debug = false;

/*
test cases
[6,5,4,3,2,1]
2
[9,9,9]
4
[1,1,1]
3
[7,1,7,1,7,1]
3
[11,111,22,222,33]
3
[11,111,22,222,33,333,44,444]
6
*/
class Solution {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {

        // We only need consider putting the latest job into the existing last day or to a new day, due to the dependency assumption.

        int n = jobDifficulty.size();
        
        if (n < d) return -1;
        
        vector<vector<map<int, int>>> dp(d, vector<map<int, int>>(n)); // dp[k][i][s] = min sum difficulty that can be obtained, if using all of "jobDifficulty[0, ..., i]" in EXACTLY the first "k" days where the difficulty of the last day is "s". All 0-based.
        
        /*
        It's implied that dp[k][i][*] for "n-i < d-k" should be -1.
        */
        // init 0th-day
        int maxByFar = INT_MIN;
        for (int i = 0; i <= n-d; ++i) {
            maxByFar = max(maxByFar, jobDifficulty[i]);
            dp[0][i][maxByFar] = maxByFar;
            if (debug) printf("dp[0][i:%d][s:%d] == %d\n", i, maxByFar, dp[0][i][maxByFar]);
        }
        
        for (int k = 1; k < d; ++k) {
            int iUpper = n-d+k;
            for (int i = k; i <= iUpper; ++i) {
                /*
                Now that we try to put "jobDifficulty[i]" into the last day, there're 2 choices:
                - add it as a new day, or
                - add it to the existing last day
                */
                // valid propagations
                int t = jobDifficulty[i], minForKMinus1 = INT_MAX;
                for (auto &[s, val] : dp[k][i-1]) {
                    if (t < s) {
                        // Putting "t" into existing last day.
                        dp[k][i][s] = val;
                    }
                }
                
                // new day
                for (auto &[s, val] : dp[k-1][i-1]) {
                    minForKMinus1 = min(minForKMinus1, val);
                }
                if (dp[k][i].count(t)) {
                    dp[k][i][t] = min(dp[k][i][t], t + minForKMinus1); 
                } else {
                    dp[k][i][t] = t + minForKMinus1;
                }
                
                // relaxation
                for (auto &[s, val] : dp[k][i-1]) {
                    if (t < s) continue; // In this case the value of "dp[k][i][t]" remains (if existing).
                    dp[k][i][t] = min(dp[k][i][t], val + (t-s));
                }
                
                if (debug) printf("dp[k:%d][i:%d][t:%d] == %d\n", k, i, t, dp[k][i][t]);
            }
        }
        
        int ans = INT_MAX;
        for (auto &[s, val] : dp[d-1][n-1]) {
            ans = min(ans, val);
        }
        
        return ans;
    }
};
