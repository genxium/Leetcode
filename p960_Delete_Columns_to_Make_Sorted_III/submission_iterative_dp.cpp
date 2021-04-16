bool debug = false;

class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int yUpper = strs.size(), xUpper = strs[0].length();
        vector<vector<char>> transposed(xUpper, vector<char>(yUpper));
        for (int x = 0; x < xUpper; ++x) {
            for (int y = 0; y < yUpper; ++y) {
                transposed[x][y] = strs[y][x];
            }
        }
        
        vector<vector<bool>> lte(xUpper, vector<bool>(xUpper, false)); // less than or equal to
        for (int i = 0; i < xUpper; ++i) {
            for (int j = 0; j < i; ++j) {
                bool res = true;
                for (int k = 0; k < yUpper; ++k) {
                    if (transposed[j][k] > transposed[i][k]) {
                        res = false;
                        break;
                    }
                }
                lte[j][i] = res;
            }
        }
        
        /*
        "dp[i] == x" means that the "Longest Sorted Subsequence" ended exactly at transposed[i] has length "x"

        dp[i] = max{ 1+dp[j] | j < i && transposed[j] <= transposed[i] }
        */
        vector<int> dp(transposed.size(), 0);
        
        // init
        int lssLen = 1;
        dp[0] = 1;

        // loop
        for (int i = 1; i < transposed.size(); ++i) {
            int maxBefore = 0;
            for (int j = 0; j < i; ++j) {
                if (lte[j][i] && maxBefore < dp[j]) maxBefore = dp[j];
            }
            dp[i] = 1+maxBefore;
            if (lssLen < dp[i]) {
                if (debug) printf("i:%d, lssLen is updated to %d by maxBefore:%d\n", i, lssLen, maxBefore);
                lssLen = dp[i];
            }
        }
        
        if (debug) printf("xUpper is %d, lssLen is %d\n", xUpper, lssLen);
        return xUpper-lssLen;
    }
};
