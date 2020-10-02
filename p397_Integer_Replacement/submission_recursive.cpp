#define ULL uint64_t
unordered_map<ULL, int> cache;

/*
test case#1
2147483647

*/
class Solution {
public:
  void solve(ULL k, ULL n, ULL steps) {
    if (cache.find(k) != cache.end()) return;
    //printf("solve(%d) with steps:%d\n", k, steps);
    if (1 == k) {
      cache[k] = 0;
      return;
    }
    
    if (steps >= n) {
      return;
    }
    
    int res = INT_MAX;
    bool isEven = ((k & 1) == 0);
    if (isEven) {
      ULL tmp = k/2;
      solve(tmp, n, steps+1);
      if (cache.find(tmp) != cache.end()) {
        res = min(res, 1+cache[tmp]);            
      }
    } else {
      ULL tmp1 = k-1;
      solve(tmp1, n, steps+1);
      if (cache.find(tmp1) != cache.end()) {
        res = min(res, 1+cache[tmp1]);            
      }
      ULL tmp2 = k+1;
      solve(tmp2, n, steps+1);
      if (cache.find(tmp2) != cache.end()) {
        res = min(res, 1+cache[tmp2]);            
      }
    }

    cache[k] = res;
  }
  
  int integerReplacement(int n) {
    solve((ULL)n, (ULL)n, 0u);
    return cache[n];
  }
};
