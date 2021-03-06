#define MAXN 10000 
#define INVALID -1 
int dp1[MAXN]; // the second dimension "[m]" is reduced to save space-complexity

class Solution {
  public:
    int isLocalMinimaOrLocalMaxima(int index, vector<int>& dedupedPrices) {
      int n = dedupedPrices.size();
      if (0 <= index-1 && index+1 < n) {
        if (dedupedPrices[index] > dedupedPrices[index-1] && dedupedPrices[index] > dedupedPrices[index+1]) {
          return +1; // maxima
        }
        if (dedupedPrices[index] < dedupedPrices[index-1] && dedupedPrices[index] < dedupedPrices[index+1]) {
          return -1; // minima
        }
        return 0;
      } else if (index+1 < n) {
        if (dedupedPrices[index] > dedupedPrices[index+1]) {
          return +1; // maxima
        }
        if (dedupedPrices[index] < dedupedPrices[index+1]) {
          return -1; // minima
        }
        return 0;
      } else if (0 <= index-1) {
        if (dedupedPrices[index] > dedupedPrices[index-1]) {
          return +1; // maxima
        }
        if (dedupedPrices[index] < dedupedPrices[index-1]) {
          return -1; // minima
        }
        return 0;
      } else {
        return 0;
      }
    } 

    int maxProfit(int k, vector<int>& prices) {
      if (1 >= prices.size()) {
        return 0;
      }
      /*
         test case #1
         2
         [3,2,6,5,0,3]

         test case #2
         2
         [-1, 1, -2, 2, -4, 4, -8, 8, -16, 16, -32, 32, -64, 64, -128, 128, -256, 256, -512, 512]

         test case #3
         2
         [-1, -1, 1, 1, -2, 2, -4, 4, -8, 8, -16, 16, -32, -32, -32, 32, -64, 64, -128, 128, 128, 128, -256, 256, -512, -512, -512, 512, 512, 512]

         test case #4
         8
         [-1, -1, 1, 1, -2, 2, -4, 4, -8, 8, -16, 16, -32, -32, -32, 32, -64, 64, -128, 128, 128, 128, -256, 256, -512, -512, -512, 512, 512, 512]

         test case #5
         1
         [2, -8, 4, -4, 8, -1024, -16, 16, -32, 32, -64, 64, -128, 128, -256, 256, -1, 512, 1023, -15, 1024, -1999]

         test case #6
         10
         [2, -8, 4, -4, 8, -1024, -16, 16, -32, 32, -64, 64, -128, 128, -256, 256, -1, 512, 1023, -15, 1024, -1999]

         test case #7
         2
         [1,2,4,2,5,7,2,4,9,0]
       */
      vector<int> dedupedPrices;
      int cur = prices[0];
      dedupedPrices.push_back(cur);
      for (int i = 1; i < prices.size(); ++i) {
        if (prices[i] != cur) {
          cur = prices[i]; 
          dedupedPrices.push_back(cur);
        } 
      }

      vector<pair<int, int>> minimaMaxima;
      for (int i = 0; i < dedupedPrices.size(); ++i) {
        int isMinimaMaxima = isLocalMinimaOrLocalMaxima(i, dedupedPrices);
        if (0 == isMinimaMaxima) {
          continue;
        }
        if (i == 0 && isMinimaMaxima == 1) {
          // We can just cut a "LocalMaxima" at the head.
          continue;
        }
        if (i == dedupedPrices.size()-1 && isMinimaMaxima == -1) {
          // We can just cut a "LocalMinima" at the tail.
          continue;
        }
        minimaMaxima.push_back({dedupedPrices[i], isMinimaMaxima});
      }
      int n = minimaMaxima.size(); 
      if (0 >= n) {
        return 0;
      }
      int halfN = (n >> 1);
      k = (k < halfN ? k : halfN);
      if (0 >= k) {
        return 0;
      }
      /*
      Now that "minimaMaxima" should have an even ".size()" and look like
      ```
      (LMIN[0], LMAX[0]), (LMIN[1], LMAX[1]), ..., (LMIN[halfN-1], LMAX[halfN-1])
      ```
      , thus the problem is equivalent to "choosing (m-1) spots out of (halfN-1) to maximize SUM{GROUP_MAX_DIFF(c) | 0 <= c < m, 1 <= m <= k}".
      */
      
      /*
      printf("MinimaMaxima(size: %d):\n", n);
      for (int i = 0; i < n; ++i) {
        pair<int, int> single = minimaMaxima[i];
        printf("(%d, %d) ", single.first, single.second);
      }
      printf("\n");
      */
      int ans = INT_MIN;

      memset(dp1, INVALID, sizeof(dp1));
      // preprocess "m == 1"
      int minByFar = minimaMaxima[0].first; 
      for (int i = 1; i < n; i+=2) {
        int candidateMinByFar = minimaMaxima[i-1].first;
        if (candidateMinByFar < minByFar) {
          minByFar = candidateMinByFar; 
        }
        dp1[i] = (minimaMaxima[i].first - minByFar);
        if (INVALID != dp1[i] && dp1[i] > ans) {
          ans = dp1[i];
        }
      }
      
      for (int m = 2; m <= k ; ++m) {
        /*
         * Stops at "i = 1" because we just won't sell at minimaMaxima[0], or dedupedPrices[0].
         * 
         * The traversal of "i" must be in reverse order such that we won't update "dp1[i][m]" by "dp1[j < i][m]". 
         */
        for (int i = n-1; i >= 1; i-=2) {
          pair<int, int> single = minimaMaxima[i];
          int reverseMinByFar = minimaMaxima[i-1].first; 
          int jLower = ((m-1) << 1) - 1; // to do the (m-1)-th sale at minimaMaxima[j], "j" should be at least "2*(m-1) - 1" 
          for (int j = i-2; j >= jLower; j-=2) {
            pair<int, int> another = minimaMaxima[j];
            pair<int, int> reverseMinCandidate = minimaMaxima[j+1];
            if (reverseMinCandidate.first < reverseMinByFar) {
              reverseMinByFar = reverseMinCandidate.first;
            }
            int candidateProfit = (minimaMaxima[i].first - reverseMinByFar);
            if (INVALID != dp1[j]) {
              candidateProfit += dp1[j];
            } 
            if (INVALID == dp1[i]) {
              dp1[i] = candidateProfit;
              //printf("\tUP#1 dp1[i:%d][m:%d] is updated to %d by dp1[j:%d][m-1:%d]==%d\n", j, i, m, dp1[i], j, m-1, dp1[j]);
            } else {
              if (candidateProfit > dp1[i]) {
                dp1[i] = candidateProfit;
                //printf("\tUP#2 dp1[i:%d][m:%d] is updated to %d by dp1[j:%d][m-1:%d]==%d\n", i, m, dp1[i], j, m-1, dp1[j]);
              }
            }
          } 

          if (INVALID != dp1[i] && dp1[i] > ans) {
            ans = dp1[i];
          } 
        }
      }

      if (INT_MIN == ans) {
        return 0;
      }
      return ans;
    }
};

