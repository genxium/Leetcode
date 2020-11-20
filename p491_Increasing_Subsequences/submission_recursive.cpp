class Solution {
public:
    void solve(int i, vector<int>&nums, vector<int> &tmp, set<vector<int>> &tmpSet, set<int> &startingElements) {
      if (tmp.size() > 1) {
        vector<int> clone;
        for (auto c : tmp) {
          clone.push_back(c);
        }
        if (tmpSet.find(clone) != tmpSet.end()) {
          // The prefix is already traversed.
          return;
        }
        tmpSet.insert(clone);
        startingElements.insert(clone[0]);
      }
      
      if (i == nums.size()) {
        return;
      }
      
      for (int j = i; j < nums.size(); ++j) {
        if (tmp.size() > 0 && nums[j] < tmp.back()) continue;
        if (0 == tmp.size() && startingElements.find(nums[j]) != startingElements.end()) {
          // The prefix is already traversed.
          continue;
        }
        tmp.push_back(nums[j]);
        solve(j+1, nums, tmp, tmpSet, startingElements);
        tmp.pop_back();
      }
    }
  
    vector<vector<int>> findSubsequences(vector<int>& nums) {
      set<vector<int>> tmpSet;
      set<int> startingElements;
      
      vector<int> tmp; tmp.reserve(nums.size());
      solve(0, nums, tmp, tmpSet, startingElements);
      
      vector<vector<int>> ans;
      for (auto &t : tmpSet) {
        ans.push_back(t);
      }
      return ans;
    }
};
