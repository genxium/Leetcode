class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
      int n = nums.size();
      /*
      test case #1
      [1,3,-1,-3,5,3,6,7]
      3
      
      test case #2
      [-7,-8,7,5,7,1,6,0]
      4
      */
      
      /*
      In this solution, "maintainer" keeps up to k non-increasing elements inside.
      
      For example, 
      ```
      [1,3,-1,-3,5,3,6,7]
      3
      ```
      will initialize [3, -1] in the maintainer.
      */
      deque<int> maintainer; 
      
      // init
      vector<int> ans;
      for (int i = 0; i < k; ++i) {
        while (false == maintainer.empty() && maintainer.back() < nums[i]) {
          maintainer.pop_back();
        }
        maintainer.push_back(nums[i]);
      }
      int iUpper = (n-k)+1; // after the iteration, there should be "(n-k)+1" elements in "ans"
      for (int i = 0; i < iUpper; ++i) {
        auto maxi = maintainer.front();
        ans.push_back(maxi);
        
        // move on
        if (i+k >= n) break;
        if (maintainer.front() == nums[i]) {
          maintainer.pop_front();
        } else {
          // otherwise "nums[i]" was already popped by a previous pushing back
        }
        while (false == maintainer.empty() && maintainer.back() < nums[i+k]) {
          maintainer.pop_back();
        }
        maintainer.push_back(nums[i+k]);
      }
      
      return ans;
    }
};
