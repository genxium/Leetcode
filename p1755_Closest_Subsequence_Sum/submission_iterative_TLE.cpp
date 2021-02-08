bool debug = true;
typedef int64_t LL;
LL const LLMAX = INT64_MAX;
LL const LLMIN = INT64_MIN;

/*
test cases
[5,-7,3,5]
6
[7,-9,15,-2]
-5
[1,2,3]
-7
[-7933,-1642,-6137,6234,4728,5474,2439]
-428059487
[3346,-3402,-9729,7432,2475,6852,5960,-7497,3229,6713,8949,9156,3945,-8686,1528,5022,-9791,-3782,-191,-9820,7720,-6067,-83,6793,340,7793,8742,8067]
-20357
[10000000,10000000,10000000,10000000,-10000000,-10000000,-10000000,-6285,-1782,-5306,8809,-8656,1370,-7465,-6992,-6619,-171,388,-1843,-1175,7354,-8370,1854,-8979,-7434,-5445,3085,2191,-2069,-6925,774,10000000,10000000,10000000,-10000000,-10000000,-10000000,-10000000]
100000
*/

class Solution {
public:
    int minAbsDifference(vector<int>& nums, int goal) {
        LL gg = goal;
        
        int n = nums.size();
        sort(nums.begin(), nums.end());
        LL positiveSum = 0;
        for (auto num : nums) {
            if (num > 0) positiveSum += num;
        }
        int const LOWEST = gg-positiveSum;
        
        set<LL> dp = {0};
        
        for (int i = 0; i < n; ++i) {
            if (0 == nums[i]) continue; // trick #1
            unordered_set<LL> newVals;
            for (auto oldVal : dp) {
                if (oldVal > gg && nums[i] > 0) break; // trick #2, nums[] is sorted
                if (oldVal <= LOWEST && nums[i] < 0) continue; // trick #3, nums[] is sorted and "positiveSum" is limited
                LL newVal = oldVal + nums[i];
                if (newVal == gg) return 0;
                newVals.insert(newVal);
            }
            for (auto newVal : newVals) dp.insert(newVal);
        }
        
        LL ans = INT64_MAX;
        auto it = dp.lower_bound(gg);
        if (it == dp.end()) {
            ans = abs(gg-*dp.rbegin());
        } else {
            ans = abs(gg-*it);
            if (it != dp.begin()) {
                --it;
                ans = min(ans, abs(gg-*it));
            }
        }
        
        return ans;
    }
};

