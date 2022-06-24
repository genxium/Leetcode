bool const debug = false;
int const INVALID = INT_MIN;
int const MAXN = 1000, MAXK = 100;
int const STATE_HOLDING_STOCK = 1, STATE_NOT_HOLDING_STOCK = 0;
/*
That "dp[i][c][STATE_HOLDING_STOCK | STATE_NOT_HOLDING_STOCK] == x" means that the maximum profit obtainable from "prices[0, ..., i]" by doing "c" transactions is "x", note that 
- if "STATE_HOLDING_STOCK", there is a "prices[u < i]" already deduced & accounted in "dp[i][c][STATE_HOLDING_STOCK]";
- if "STATE_NOT_HOLDING_STOCK", there is a "prices[v <= i]" already sold & accounted in "dp[i][c][STATE_NOT_HOLDING_STOCK]".

dp[i][c][STATE_NOT_HOLDING_STOCK] = max{
                    dp[i-1][c][STATE_HOLDING_STOCK] + prices[i], // sold at "prices[i]"
                    dp[i-1][c][STATE_NOT_HOLDING_STOCK]
              }

dp[i][c][STATE_HOLDING_STOCK] = max{
                    dp[i-1][c-1][STATE_NOT_HOLDING_STOCK] - prices[i], // buy and start a new transaction at "prices[i]"
                    dp[i-1][c][STATE_HOLDING_STOCK] // keep
              }
*/
int dp[MAXN][MAXK+1][2]; 

/*
test cases
2
[2,4,1]
2
[1,2,4,7]
2
[1,4,2,7]
2
[4,2,1,7]
2
[3,2,6,5,0,3]
2
[1,2,4,2,5,7,2,4,9,0]
3
[1,2,4,2,5,7,2,4,9,0]
4
[1,2,4,2,5,7,2,4,9,0]
*/
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        int halfN = (n >> 1);
        k = (k < halfN ? k : halfN);
        if (0 >= k) {
            return 0;
        }
        
        int ans = 0;
        
        // init
        for (int i = 0; i < n; ++i) {
            dp[i][0][STATE_NOT_HOLDING_STOCK] = 0; // When no transaction was done by the i-th day
            dp[i][0][STATE_HOLDING_STOCK] = INVALID;   
            for (int c = 1; c <= k; ++c) {
                dp[i][c][STATE_NOT_HOLDING_STOCK] = INVALID;
                dp[i][c][STATE_HOLDING_STOCK] = INVALID;                
            }
        }
        dp[0][0][STATE_NOT_HOLDING_STOCK] = 0;
        dp[0][1][STATE_HOLDING_STOCK] = -prices[0];
        
        // loop
        for (int i = 1; i < n; ++i) {
            for (int c = 1; c <= k; ++c) {
                dp[i][c][STATE_NOT_HOLDING_STOCK] = dp[i-1][c][STATE_NOT_HOLDING_STOCK];
                if (INVALID != dp[i-1][c][STATE_HOLDING_STOCK]) {
                    int sellingTodayCand = dp[i-1][c][STATE_HOLDING_STOCK] + prices[i];
                    if (sellingTodayCand > dp[i][c][STATE_NOT_HOLDING_STOCK]) {
                        dp[i][c][STATE_NOT_HOLDING_STOCK] = sellingTodayCand;
                        if (debug) printf("Updated dp[i:%d][c:%d][STATE_NOT_HOLDING_STOCK] to %d by selling at prices[i:%d] == %d for dp[i-1:%d][c:%d][STATE_HOLDING_STOCK] == %d\n", i, c, dp[i][c][STATE_NOT_HOLDING_STOCK], i, prices[i], i-1, c, dp[i-1][c][STATE_HOLDING_STOCK]);
                    }    
                }

                dp[i][c][STATE_HOLDING_STOCK] = dp[i-1][c][STATE_HOLDING_STOCK];
                if (INVALID != dp[i-1][c-1][STATE_NOT_HOLDING_STOCK]) {
                    int buyingTodayCand = dp[i-1][c-1][STATE_NOT_HOLDING_STOCK] - prices[i];
                    if (buyingTodayCand > dp[i][c][STATE_HOLDING_STOCK]) {
                        dp[i][c][STATE_HOLDING_STOCK] = buyingTodayCand;
                        if (debug) printf("Updated dp[i:%d][c:%d][STATE_HOLDING_STOCK] to %d by buying at prices[i:%d] == %d\n", i, c, dp[i][c][STATE_HOLDING_STOCK], i, prices[i]);
                    }    
                }
                
                if (dp[i][c][STATE_NOT_HOLDING_STOCK] <= ans) continue;
                
                ans = dp[i][c][STATE_NOT_HOLDING_STOCK];
                if (debug) printf("\tUpdated ans to %d by dp[i:%d][c:%d][STATE_NOT_HOLDING_STOCK]\n", ans, i, c);              
            }
        }
        
        return ans;
    }
};
