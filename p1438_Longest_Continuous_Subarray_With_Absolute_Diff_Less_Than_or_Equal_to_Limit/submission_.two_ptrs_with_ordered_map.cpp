int const MAXN = 100000;

class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
      int ans = 1;
      int l = 0, r = 0, n = nums.size();
      map<int, int> counter;
      counter[nums[r]] = 1;
      while (r < n) {
          ++r;
          if(r >= n) break;
          
          if (counter.find(nums[r]) != counter.end()) ++counter[nums[r]];
          else counter[nums[r]] = 1;
          
          int rangemin = counter.begin()->first;
          int rangemax = counter.rbegin()->first;
          while (l <= r && abs(rangemax-rangemin) > limit) {
            --counter[nums[l]];
            if (0 == counter[nums[l]]) counter.erase(nums[l]);
            ++l;
            rangemin = counter.begin()->first;
            rangemax = counter.rbegin()->first;
          }
          ans = max(ans, r-l+1);
      }
      return ans;
    }
};
