int const MAXN = 50000;
int const INVALID = -1;
int const STATE_HOLDING_STOCK = 1, STATE_NOT_HOLDING_STOCK = 0;
int dp[MAXN][2];

class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int n = prices.size();
        memset(dp, INVALID, sizeof dp);
        // init
        dp[0][STATE_HOLDING_STOCK] = -prices[0];
        dp[0][STATE_NOT_HOLDING_STOCK] = 0;
        
        // loop
        for (int i = 1; i < n; ++i) {
            dp[i][STATE_HOLDING_STOCK] = dp[i-1][STATE_HOLDING_STOCK];
            int candidateBuyToday = dp[i-1][STATE_NOT_HOLDING_STOCK] - prices[i];
            dp[i][STATE_HOLDING_STOCK] = max(dp[i][STATE_HOLDING_STOCK], candidateBuyToday);
            
            dp[i][STATE_NOT_HOLDING_STOCK] = dp[i-1][STATE_NOT_HOLDING_STOCK];
            int candidateSellToday = dp[i-1][STATE_HOLDING_STOCK] + prices[i] - fee;
            dp[i][STATE_NOT_HOLDING_STOCK] = max(dp[i][STATE_NOT_HOLDING_STOCK], candidateSellToday);
        }
        
        int ans = dp[n-1][STATE_NOT_HOLDING_STOCK];
        return ans;
    }
};
