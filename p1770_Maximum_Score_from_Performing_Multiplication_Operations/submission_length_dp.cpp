bool debug = false;
int const MAXETCHED = 1000;
int const INVALID = -1;
int dp[MAXETCHED+1][MAXETCHED+1]; // dp[etchedLenLeft][etchedLenRight]

class Solution {
public:
    int maximumScore(vector<int>& nums, vector<int>& multipliers) {
        int ans = INT_MIN, n = nums.size(), m = multipliers.size();
        // init
        memset(dp, 0, sizeof dp);

        // loop 
        for (int totalEtchedLen = 1; totalEtchedLen <= m; ++totalEtchedLen) {
            for (int etchedLenLeft = 0; etchedLenLeft <= totalEtchedLen; ++etchedLenLeft) {
                int etchedLenRight = totalEtchedLen-etchedLenLeft;
                if (debug) printf("etchedLenLeft:%d, etchedLenRight:%d\n", etchedLenLeft, etchedLenRight);
                int multiplier = multipliers[totalEtchedLen-1];
                dp[etchedLenLeft][etchedLenRight] = INT_MIN;
                if (etchedLenLeft-1 >= 0) {
                    dp[etchedLenLeft][etchedLenRight] = max(dp[etchedLenLeft][etchedLenRight], multiplier*nums[etchedLenLeft-1] + dp[etchedLenLeft-1][etchedLenRight]);
                }
                if (etchedLenRight-1 >= 0) {
                    dp[etchedLenLeft][etchedLenRight] = max(dp[etchedLenLeft][etchedLenRight], multiplier*nums[n-etchedLenRight] + dp[etchedLenLeft][etchedLenRight-1]);
                }

                if (etchedLenLeft + etchedLenRight == m) {
                    ans = max(ans, dp[etchedLenLeft][etchedLenRight]);
                }
            }
        }

        return ans;
    }
};
