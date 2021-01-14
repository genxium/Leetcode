#define INVALID -1 
#define MAXN 5000
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
int dpSell[MAXN], dpBuy[MAXN]; 
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
      int n = prices.size(), ans = 0;
      if (0 >= n) return ans;
      
      memset(dpSell, 0, sizeof(dpSell));
      memset(dpBuy, 0, sizeof(dpBuy));
      // init
      dpSell[0] = 0;
      dpBuy[0] = -prices[0];
      
      // loop 
      for (int i = 1; i < n; ++i) {
        dpSell[i] = max(dpSell[i-1], dpBuy[i-1]+prices[i]);
        if (i >= 2) {
          dpBuy[i] = max(dpBuy[i-1], -prices[i]+dpSell[i-2]);
        } else {
          dpBuy[i] = max(dpBuy[i-1], -prices[i]);
        }

        if (ans < dpSell[i]) {
          ans = dpSell[i];
        }
      }
      return ans;
    }
};
