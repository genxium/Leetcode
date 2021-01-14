#define maxh(a, b) (a > b ? a : b)
#define minh(a, b) (a < b ? a : b)
int dpSell, dpSellYesterday, dpSellTheDayBeforeYesterday, dpBuy; 
/*
- "dpSell[i] == x" means that selling on or before i-th day can reach max profit "x"
- "dpBuy[i] == y" means that buying on or before i-th day can reach max profit "y", where the last implied action must be buying

dpSell[i] = max{dpSell[i-1], dpBuy[i-1]+prices[i]}
dpBuy[i] = max{dpBuy[i-1], -prices[i]+dpSell[i-2]}
*/

class Solution {
public:
    int maxProfit(vector<int>& prices) {
      /*
      test case #0
      []
      
      test case #1
      [1,2,3,0,2]
      
      test case #2
      [2,4,1,7,11]
      */
      int n = prices.size();
      if (0 >= n) return 0;
      
      // init
      dpSell = 0;
      dpSellYesterday = 0;
      dpSellTheDayBeforeYesterday = 0;
      dpBuy = -prices[0];
      
      // loop 
      for (int i = 1; i < n; ++i) {
        dpSellTheDayBeforeYesterday = dpSellYesterday;
        dpSellYesterday = dpSell;
        dpSell = maxh(dpSellYesterday, dpBuy+prices[i]); // the "right-hand-side" is implicitly "value of [i-1]"
        dpBuy = maxh(dpBuy, -prices[i]+dpSellTheDayBeforeYesterday); // the "right-hand-side" is implicitly "value of [i-1]"
      }
      return dpSell;
    }
};
