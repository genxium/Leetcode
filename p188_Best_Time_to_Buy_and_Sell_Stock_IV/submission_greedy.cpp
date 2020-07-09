#define INVALID -1 

class ListEle {
public:
  int lmin, lmax;
  ListEle *next;
  ListEle(int aLmin, int aLmax) {
    this->lmin = aLmin;
    this->lmax = aLmax;
    this->next = NULL;
  }
};

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
      int curPrice = prices[0];
      dedupedPrices.push_back(curPrice);
      for (int i = 1; i < prices.size(); ++i) {
        if (prices[i] != curPrice) {
          curPrice = prices[i]; 
          dedupedPrices.push_back(curPrice);
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

      In fact, it’s unnecessary to consider “not using all of k transactions”, as the optimal strategy is “buying at each LMIN[i] and selling immediately at LMAX[i]” if there were no transaction-count-limit. 

      It can be proved by the fact that “LMAX[i-1] > LMIN[i]”, thus
      - “(LMAX[i] - LMIN[i-1]) < (LMAX[i]-LMIN[i]) + (LMAX[i-1] - LMIN[i-1])”, and 
      - "MergeDeduction = (LMAX[i-1] - LMIN[i])".

      This hints a greedy approach to the rescue, that we'll always either
      - merge the smallest {LMAX[i-1] - LMIN[i]}, or
      - drop the smallest {LMAX[i] - LMIN[i]}
      , up to "(halfN-1)-(k-1)" times.
      */
      /*
      printf("MinimaMaxima(size: %d):\n", n);
      for (int i = 0; i < n; ++i) {
        pair<int, int> single = minimaMaxima[i];
        printf("%d ", single.first);
      }
      printf("\n");
      */
      ListEle *head = NULL, *cur = NULL;
      int ans = 0, mergeCountUpper = (halfN-k);
      
      for (int i = 0; i < n; i+=2) {
        ListEle *singleNode = new ListEle(minimaMaxima[i].first, minimaMaxima[i+1].first);
        if (NULL == head) {
          cur = singleNode;
          head = cur;
        } else {
          cur->next = singleNode;
          cur = cur->next;  
        }
        ans += (cur->lmax - cur->lmin);
      }
      //printf("initially ans == %d\n", ans);
      
      int mergeCount = 0;
      while (mergeCount < mergeCountUpper) {
        int minDeduction = INT_MAX;
        cur = head;
        ListEle *prev = NULL, *targetPrev = NULL, *target = NULL;
        int targetType = -1; // 0: to delete target 1: to merge target(as left-hand-side) 
        
        //printf("At mergeCount:%d\n", mergeCount);
        while (NULL != cur) {
          //printf("\tinvestigating cur:(%d, %d)\n", cur->lmin, cur->lmax);
          int type0Candidate = (cur->lmax - cur->lmin);
          if (type0Candidate < minDeduction) {
            minDeduction = type0Candidate;
            targetType = 0;
            target = cur;
            targetPrev = prev;
          }
          if (NULL != cur->next) {
            int type1Candidate = (cur->lmax - cur->next->lmin);  
            if (type1Candidate < minDeduction) {
              minDeduction = type1Candidate;
              targetType = 1;
              target = cur;
              targetPrev = prev;
            }
          }
          prev = cur;
          cur = cur->next;
        }
        
        //printf("\ttargetType == %d\n", targetType);
        
        if (targetType == 0) {
          if (NULL != targetPrev) {
            targetPrev->next = target->next;
          } else {
            // head == target
            head = target->next;
          }
        }
        if (targetType == 1) {
          ListEle* cachedNext = target->next;
          target->next = cachedNext->next;
          target->lmax = cachedNext->lmax;
        }
        ans -= minDeduction;
        ++mergeCount;
      }

      return ans;
    }
};
