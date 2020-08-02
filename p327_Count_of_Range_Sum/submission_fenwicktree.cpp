#define LL int64_t

struct FenwickTree {
  vector<LL> bit;  // binary indexed tree
  int n;

  FenwickTree(int n) {
    this->n = n;
    bit.assign(n, 0L);
  }

  FenwickTree(vector<LL> &a) : FenwickTree(a.size()) {
    for (size_t i = 0; i < a.size(); i++) {
      add(i, a[i]);      
    }
  }

  LL sum(int r) {
    LL ret = 0;
    for (; r >= 0; r = (r & (r + 1)) - 1) {
      ret += bit[r];      
    }
    return ret;
  }

  LL sum(int l, int r) {
    return sum(r) - sum(l - 1);
  }

  void add(int idx, LL delta) {
    for (; idx < n; idx = idx | (idx + 1)) {
      bit[idx] += delta; 
    }
  }
};

class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
      /*
      test case #1
      [2147483647,-2147483648,-1,0]
      -1
      0
      */
      int n = nums.size();
      int ans = 0;
      vector<LL> prefixSums{0L}; // to compute all RangeSum[0, j], we need a "fixed PrefixSum of 0"
      for (int i = 0; i < n; ++i) {
        prefixSums.push_back(prefixSums.back()+(LL)nums[i]);  
      }
      map<LL, int> sortedOrderOf;
      vector<LL> sortedDedupedPrefixSums;
      for (int i = 0; i < prefixSums.size(); ++i) {
        if (sortedOrderOf.find(prefixSums[i]) == sortedOrderOf.end()) {
          sortedOrderOf[prefixSums[i]] = 0;
          sortedDedupedPrefixSums.push_back(prefixSums[i]);
        }
      }
      sort(sortedDedupedPrefixSums.begin(), sortedDedupedPrefixSums.end());
      for (int i = 0; i < sortedDedupedPrefixSums.size(); ++i) {
        sortedOrderOf[sortedDedupedPrefixSums[i]] = i;
      }
      
      FenwickTree t(sortedDedupedPrefixSums.size());
      
      for (int j = 0; j < prefixSums.size(); ++j) {
        LL xLower = prefixSums[j] - upper; 
        LL xUpper = prefixSums[j] - lower; 
        int i1 = lower_bound(sortedDedupedPrefixSums.begin(), sortedDedupedPrefixSums.end(), xLower) - sortedDedupedPrefixSums.begin(); // prefixSum[j] - sortedPrefixSum[i1] <= upper
        int i2 = upper_bound(sortedDedupedPrefixSums.begin(), sortedDedupedPrefixSums.end(), xUpper) - sortedDedupedPrefixSums.begin(); // prefixSum[j] - sortedPrefixSum[i2] < lower
        int increment = t.sum(i1, i2-1);
        //printf("@j:%d, prefixSums[j]:%d, xLower:%d, xUpper:%d, increment:%d from i1:%d, i2:%d\n", j, prefixSums[j], xLower, xUpper, increment, i1, i2);
        ans += increment;
        t.add(sortedOrderOf[prefixSums[j]], 1);
      }
      
      return ans;
    }
};
