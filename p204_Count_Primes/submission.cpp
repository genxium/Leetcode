#define MAXN 1500000
bool isPrime[MAXN];
int partialSum[MAXN];

class Solution {
public:
    Solution() {
      memset(isPrime, true, sizeof(isPrime));
      memset(partialSum, 0, sizeof(partialSum));

      isPrime[0] = false;
      isPrime[1] = false;
      for (int i = 2; i < MAXN; ++i) {
        if (isPrime[i]) {
          for (int j = (i << 1); j < MAXN; j+=i) {
            isPrime[j] = false;
          }
        }
        partialSum[i] = partialSum[i-1] + (isPrime[i] ? 1 : 0);
      }
    }
    
    int countPrimes(int n) {
      if (0 >= n) return 0;
      return partialSum[n-1];    
    }
};
