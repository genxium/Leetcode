class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
      /*
      test case #1
      [1,3,-1,-3,5,3,6,7]
      3
      
      test case #2
      [-7,-8,7,5,7,1,6,0]
      4
      */
      map<int, int> maintainer;
      for (int i = 0; i < k; ++i) {
        if (maintainer.find(nums[i]) == maintainer.end()) {
          maintainer[nums[i]] = 1;          
        } else {
          ++maintainer[nums[i]];
        }
      }
      vector<int> ans;
      int n = nums.size();
      for (int i = 0; i < n; ++i) {
        auto maxIt = maintainer.rbegin();
        ans.push_back(maxIt->first);
        // move on
        if (i+k >= n) break;
        --maintainer[nums[i]];
        if (0 == maintainer[nums[i]]) {
          maintainer.erase(nums[i]);
        }
        if (maintainer.find(nums[i+k]) == maintainer.end()) {
          maintainer[nums[i+k]] = 1;          
        } else {
          ++maintainer[nums[i+k]];
        }
      }
      return ans;
    }
};
