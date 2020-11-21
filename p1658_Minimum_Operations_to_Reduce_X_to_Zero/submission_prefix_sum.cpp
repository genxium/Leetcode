class Solution {
public:
    int minOperations(vector<int>& nums, int x) {
      int n = nums.size();
      vector<int> prefixSums = {0}, revPrefixSums = {0}; // Both are monotonically increasing.
      for (int i = 0; i < n; ++i) {
        prefixSums.push_back(prefixSums.back() + nums[i]);
        int revI = n-1-i;
        revPrefixSums.push_back(revPrefixSums.back() + nums[revI]);
      }
      
      if (prefixSums[n] < x) return -1;
      
      int ans = INT_MAX;
      for (int i = 0; i <= n; ++i) {
        int prefixSum = prefixSums[i];
        int target = x - prefixSum;
        auto it = lower_bound(revPrefixSums.begin(), revPrefixSums.end(), target);
        if (it == revPrefixSums.end()) continue;
        if (*it != target) continue;
        int countInTail = (it - revPrefixSums.begin());
        ans = min(ans, i+countInTail);
      }
      
      if (INT_MAX == ans) return -1;
      return ans;
    }
};
