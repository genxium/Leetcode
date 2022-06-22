/*
test cases
["babca","bbazb"]
["edcba"]
["ghi","def","abc"]
*/
bool debug = false;

class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int yUpper = strs.size(), xUpper = strs[0].length();
        vector<vector<bool>> lte(xUpper, vector<bool>(xUpper, false)); // "wholeColumn" less than or equal to relations
        for (int j = 0; j < xUpper; ++j) {
            for (int i = 0; i < j; ++i) {
                // compare each character of the i-th column and the j-th column
                bool res = true;
                for (int k = 0; k < yUpper; ++k) {
                    if (strs[k][i] <= strs[k][j]) continue;
                    res = false;
                    break;
                }
                lte[i][j] = res;
                if (debug) printf("lte[%d][%d] = %d\n", i, j, res);
            }
        }
        /*
        "dp[j] == x" means that the "Longest Sorted Subsequence" ended exactly at the j-th column has length "x"

        dp[j] = max{ 1+dp[i] | i < j && wholeColumn[i] <= wholeColumn[j] }
        */
        vector<int> dp(xUpper, 0);
        
        // init
        int lssLen = 1;
        dp[0] = 1;

        // loop
        for (int j = 1; j < xUpper; ++j) {
            int maxBefore = 0;
            for (int i = 0; i < j; ++i) {
                if (lte[i][j] && maxBefore < dp[i]) maxBefore = dp[i];
            }
            dp[j] = 1+maxBefore;
            if (lssLen < dp[j]) {
                if (debug) printf("j:%d, lssLen is updated to %d by maxBefore:%d\n", j, lssLen, maxBefore);
                lssLen = dp[j];
            }
        }
        
        if (debug) printf("xUpper is %d, lssLen is %d\n", xUpper, lssLen);
        return xUpper-lssLen;
    }
};
