bool const debug = false;

/*
test cases
[4,2,3]
[4,2,1]
[1,1,1]
[1,4,1,2]
*/
class Solution {
public:
    int solveNlogn(vector<int>& nums) {
      vector<int> sortedCache;
      for (auto &num : nums) {
        auto lower = lower_bound(sortedCache.begin(), sortedCache.end(), num);
        if (lower == sortedCache.end()) {
            if (debug) printf("#1 inserting num:%d\n", num);
            sortedCache.push_back(num);
        } else {
            auto upper = upper_bound(sortedCache.begin(), sortedCache.end(), num);
            if (upper == sortedCache.end()) {
                if (debug) printf("#2 inserting num:%d\n", num);
                sortedCache.push_back(num);
            } else {
                if (debug) printf("#3 emplacing num:%d at sortedCache[%d]\n", num, (upper-sortedCache.begin()));
                // restarting a new subsequence in the sorted cache @lower
                *upper = num;   
            }
        }
      }
      return sortedCache.size();
    }
    
    bool checkPossibility(vector<int>& nums) {
        int lisLen = solveNlogn(nums);
        if (debug) printf("lisLen is %d\n", lisLen);
        return lisLen >= nums.size()-1;
    }
};
