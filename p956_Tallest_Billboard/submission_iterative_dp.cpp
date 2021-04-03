bool debug = false;

class Solution {
public:
    int tallestBillboard(vector<int>& rods) {   
        /*
        Assume that we put each rod 
        - into the "PositiveBucket", or
        - into the "NegativeBucket", or
        - into no bucket, i.e. discard it.

        Denote "dp[i][d]" as the largest "PositiveBucketSum" using "rods[0, ..., i]" and yielding "d == PositiveBucketSum-NegativeBucketSum", where "d" could be negative.
        */
        int n = rods.size();
        vector<unordered_map<int, int>> dp(n);

        // init
        dp[0][0] = 0; // discards
        dp[0][rods[0]] = rods[0]; // puts into "PositiveBucket"
        dp[0][-rods[0]] = 0; // puts into "NegativeBucket"

        // loop
        for (int i = 1; i < n; ++i) {
            for (auto &[diff, maxPositiveSum] : dp[i-1]) {
                dp[i][diff] = max(dp[i][diff], maxPositiveSum); // discards
                dp[i][diff+rods[i]] = max(dp[i][diff+rods[i]], maxPositiveSum+rods[i]); // puts into "PositiveBucket"
                dp[i][diff-rods[i]] = max(dp[i][diff-rods[i]], maxPositiveSum);; // puts into "NegativeBucket"
            }
        }

        return dp[n-1][0];
    }
};
