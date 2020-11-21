struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

class Solution {
public:
    int minOperations(vector<int>& nums, int x) {
      queue<vector<int>> q;
      unordered_set<vector<int>, VectorHash> openSet;
      unordered_set<vector<int>, VectorHash> closedSet;
      
      int n = nums.size();
      int sum = 0;
      for (auto &num : nums) {
        sum += num;
      }
      if (sum < x) return -1;
      if (sum == x) return n;
      
      // init
      int l = 0, r = n-1;
      vector<int> candidate0 = {x, l, r};
      q.push(candidate0);
      openSet.insert(candidate0);
      
      int level = 0, ans = INT_MAX;
      while (!q.empty()) {
        int localSize = q.size(); // LevelOrderTraversal search
        for (int k = 0; k < localSize; ++k) {
          auto &expander = q.front();
          int l = expander[1], r = expander[2], remainedX = expander[0];
          // printf("level:%d, expander: {remainedX:%d, l:%d, r:%d, nums[l:%d]:%d, nums[r:%d]:%d}\n", level, remainedX, l, r, l, nums[l], r, nums[r]);
          openSet.erase(expander);
          closedSet.insert(expander);
          
          if (remainedX == 0) {
            ans = min(ans, level);
            return ans;
          }
          
          q.pop();
          
          if (l <= r && r >= 0 && l < n) {
            // no need to consider if "remainedX" drops below 0
            if (remainedX == nums[l] || remainedX == nums[r]) {
              return level+1;
            }
            if (remainedX > nums[l]) {
              vector<int> candidate = {remainedX-nums[l], l+1, r};
              // printf("candidate#1: {remainedX:%d, l:%d, r:%d}\n", candidate[0], candidate[1], candidate[2]);
              if (openSet.find(candidate) == openSet.end() && closedSet.find(candidate) == closedSet.end()) {
                openSet.insert(candidate);
                q.push(candidate);
                // printf("\tinserted\n");
              }
            }
            
            if (remainedX > nums[r]) {
              // printf("candidate#2: {remainedX:%d, l:%d, r:%d}\n", candidate[0], candidate[1], candidate[2]);
              vector<int> candidate = {remainedX-nums[r], l, r-1};
              if (openSet.find(candidate) == openSet.end() && closedSet.find(candidate) == closedSet.end()) {
                openSet.insert(candidate);
                q.push(candidate);
                // printf("\tinserted\n");
              }
            }
          }
        }
        ++level;
      }
      
      if (INT_MAX == ans) return -1;
      return ans;
    }
};
