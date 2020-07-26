#define MAXN 100000
#define MOD 1000000007
#define LL int64_t

LL cntOdd[MAXN], cntEven[MAXN]; // cntOdd[i], cntEven[i] both end exactly at arr[i]

class Solution {
public:
    int numOfSubarrays(vector<int>& arr) {
      memset(cntOdd, 0, sizeof(cntOdd));
      memset(cntEven, 0, sizeof(cntEven));
      
      LL ans = 0;
      if ((arr[0] & 1) > 0L) {
        ++cntOdd[0];
      } else {
        ++cntEven[0];
      }
      
      ans += cntOdd[0];
      ans %= MOD;
      
      for (int i = 1; i < arr.size(); ++i) {
        LL val = arr[i];
        if ((val & 1L) > 0L) {
          cntOdd[i] = cntEven[i-1] + 1L; cntOdd[i] %= MOD;
          cntEven[i] = cntOdd[i-1]; cntEven[i] %= MOD;
        } else {
          cntOdd[i] = cntOdd[i-1]; cntOdd[i] %= MOD;
          cntEven[i] = cntEven[i-1] + 1L; cntEven[i] %= MOD;
        }
        ans += cntOdd[i];
        ans %= MOD;
      }
      return (int)ans;
    }
};
