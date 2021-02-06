/*
A naive and TOO SLOW idea.

"dp[i] == x" means that selling EXACTLY on i-th day can reach max profit "x"

dp[i] = max{dp[j] + prices[i] - min(prices[t]) | j+2 <= i-1 && j+2 <= t <= i-1}
*/

/*
Assume that BY the i-th day, "dp[i][STATE_NOT_HOLDING_STOCK | STATE_HOLDING_STOCK] == x" being the max profit of respective state. Note that in this solution, "i" is 1-based, i.e. at the i-th day the price is "prices[i-1]".

dp[i][STATE_HOLDING_STOCK] = max{dp[i-1][STATE_HOLDING_STOCK], -prices[i-1]+dp[i-2][STATE_NOT_HOLDING_STOCK]}
dp[i][STATE_NOT_HOLDING_STOCK] = max{dp[i-1][STATE_NOT_HOLDING_STOCK], dp[i-1][STATE_HOLDING_STOCK]+prices[i-1]}
*/
int const MAXN = 10000;
int const INVALID = -1;
int const STATE_HOLDING_STOCK = 1, STATE_NOT_HOLDING_STOCK = 0;
int dp[MAXN+1][2];

/*
test cases
[]
[1,2,3,0,2]
[2,4,1,7,11]
[2,1,4]
*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (0 >= n) return 0;

        // init
        memset(dp, INVALID, sizeof dp);
        dp[0][STATE_NOT_HOLDING_STOCK] = 0; 
            
        dp[1][STATE_HOLDING_STOCK] = -prices[0];
        dp[1][STATE_NOT_HOLDING_STOCK] = 0;

        // loop
        for (int i = 2; i <= n; ++i) {
            int priceToday = prices[i-1];
            dp[i][STATE_HOLDING_STOCK] = dp[i-1][STATE_HOLDING_STOCK];
            if (i >= 2) {
                int candidateBuyToday = -priceToday+dp[i-2][STATE_NOT_HOLDING_STOCK];
                dp[i][STATE_HOLDING_STOCK] = max(dp[i][STATE_HOLDING_STOCK], candidateBuyToday);    
            }
            
            dp[i][STATE_NOT_HOLDING_STOCK] = dp[i-1][STATE_NOT_HOLDING_STOCK];
            int candidateSellToday = dp[i-1][STATE_HOLDING_STOCK] + priceToday;
            dp[i][STATE_NOT_HOLDING_STOCK] = max(dp[i][STATE_NOT_HOLDING_STOCK], candidateSellToday);
        }
        
        return dp[n][STATE_NOT_HOLDING_STOCK];
    }
};
