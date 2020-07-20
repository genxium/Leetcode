#define LL int64_t
vector<int> factors{2, 3, 5};
vector<int> uglyNumbers;

class Solution {
public:
    int nthUglyNumber(int n) {
      priority_queue<LL, vector<LL>, greater<LL>> openHeap;
      set<LL> closedSet;
      openHeap.push(1L);
      closedSet.insert(1L);
      LL closedSetSizeUpper = (n << 1);
      while (false == openHeap.empty() && closedSet.size() < closedSetSizeUpper) {
        LL expander = openHeap.top(); openHeap.pop();
        for (auto &factor : factors) {
          LL newVal = expander*factor;
          if (closedSet.find(newVal) != closedSet.end()) continue;
          openHeap.push(newVal);
          closedSet.insert(newVal);
        }
      }
      int i = 0;
      for (auto it = closedSet.begin(); it != closedSet.end() && i < n; ++i, ++it) {
        if (i == n-1) {
          return (int)(*it);
        }
      }
      return 0;
    }
};
