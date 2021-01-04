/*
test cases
[5,1,3]
[9,4,2,3,4]
[5,1,2,3,4]
[1,2,3,4,5]
[6,4,8,1,3,2]
[4,7,6,2,3,8,6,1]
*/
class Solution {
public:
    int lcsNlogn(vector<int>& nums) {
      vector<int> sortedCache;
      for (auto &num : nums) {
        auto lower = lower_bound(sortedCache.begin(), sortedCache.end(), num);
        if (lower == sortedCache.end()) {
          sortedCache.push_back(num);
        } else {
          *lower = num;
        }
      }
      return sortedCache.size();
    }
    
    int minOperations(vector<int>& target, vector<int>& arr) {
        unordered_map<int, int> intermediate;
        for (int i = 0; i < target.size(); ++i) {
            intermediate[target[i]] = i;
        }
        
        vector<int> u;
        for (int i = 0; i < arr.size(); ++i) {
            if (intermediate.find(arr[i]) == intermediate.end()) continue;
            u.push_back(intermediate[arr[i]]);
        }
        
        int lcsLength = lcsNlogn(u);
        return target.size() - lcsLength;
    }
};
