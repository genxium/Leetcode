/*
   Regardless of the transaction count limitation, we'd divide the input time-series by "LocalMinima"s and "LocalMaxima"s.

   [Lemma#1]
   The "LocalMinima"s and "LocalMaxima"s in the input must occur alternatively.

   [Lemma#2]
   We should always buy at a "LocalMinima".
   [RoughProof] If we buy at a "non-LocalMinima", it's always better to instead buy at a nearest "LocalMinima". 

   [Lemma#3]
   We should always sell at a "LocalMaxima".
   [RoughProof] If we sell at a "non-LocalMaxima", it's always better to instead sell at a nearest "LocalMaxima". 

   [Lemma#4]
   Assume that 
   ```
   dp1[i][m] being the largest profit if selling the m-th transaction exactly at prices[i]
   ```
   , then 

   ```
   dp1[i][m] = MAX{dp1[j][m-1]+(prices[i]-MIN(prices[j+1, ..., i-1])) | for all j+1 <= i-1, dp1[j][m-1] != INVALID}. -- (a)
   ```

   By [Lemma#123], we see that only "i"s at "LocalMaxima"s are worth inspecting.
 */

#define MAXN 1000 
#define INVALID -1 
int dp1[MAXN][MAXN];

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
        if (0 != isMinimaMaxima) {
          minimaMaxima.push_back({dedupedPrices[i], isMinimaMaxima});
        }
      }
      /*
      printf("MinimaMaxima:\n");
      for (int i = 0; i < minimaMaxima.size(); ++i) {
        pair<int, int> single = minimaMaxima[i];
        printf("(%d, %d) ", single.first, single.second);
      }
      printf("\n");
      */
      int ans = INT_MIN;

      memset(dp1, INVALID, sizeof(dp1));
      for (int m = 1; m <= k ; ++m) {
        // Starting from "i = 1" because we just won't sell at minimaMaxima[0], or dedupedPrices[0].
        for (int i = 1; i < minimaMaxima.size(); ++i) {
          pair<int, int> single = minimaMaxima[i];
          if (single.second == -1) {
            continue; // don't sell at "LocalMinima"
          }  
          if (m == 1) {
            int reverseMinByFar = minimaMaxima[i-1].first; 
            for (int ii = i-1; ii >= 0; --ii) {
              if (minimaMaxima[ii].second == +1) {
                continue; // don't buy at "LocalMaxima"
              }
              pair<int, int> reverseMinCandidate = minimaMaxima[ii];
              if (reverseMinCandidate.first < reverseMinByFar) {
                reverseMinByFar = reverseMinCandidate.first;
              }
            }
            dp1[i][m] = (minimaMaxima[i].first - reverseMinByFar);  
            //printf("INIT#1 dp1[i:%d][m:%d] is initialized to %d\n", i, m, dp1[i][m]);
          } else {
            int reverseMinByFar = minimaMaxima[i-1].first; 
            for (int j = i-2; j >= 0; --j) {
              pair<int, int> another = minimaMaxima[j];
              if (another.second == -1) {
                continue; // don't sell at "LocalMinima"
              }  
              pair<int, int> reverseMinCandidate = minimaMaxima[j+1];
              if (reverseMinCandidate.first < reverseMinByFar) {
                reverseMinByFar = reverseMinCandidate.first;
              }
              int candidateProfit = (minimaMaxima[i].first - reverseMinByFar);
              if (INVALID != dp1[j][m-1]) {
                candidateProfit += dp1[j][m-1];
              } 
              if (INVALID == dp1[i][m]) {
                dp1[i][m] = candidateProfit;
                //printf("\tUP#1 dp1[i:%d][m:%d] is updated to %d by dp1[j:%d][m-1:%d]==%d\n", j, i, m, dp1[i][m], j, m-1, dp1[j][m-1]);
              } else {
                if (candidateProfit > dp1[i][m]) {
                  dp1[i][m] = candidateProfit;
                  //printf("\tUP#2 dp1[i:%d][m:%d] is updated to %d by dp1[j:%d][m-1:%d]==%d\n", i, m, dp1[i][m], j, m-1, dp1[j][m-1]);
                }
              }
            } 
          }

          if (INVALID != dp1[i][m] && dp1[i][m] > ans) {
            ans = dp1[i][m];
          } 
        }
      }

      if (INT_MIN == ans) {
        return 0;
      }
      return ans;
    }
};
