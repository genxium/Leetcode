bool debug = true;
typedef int LL;
LL const LLMAX = INT_MAX;

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
        LL positiveSum = 0, negativeSum = 0;
        for (auto num : nums) {
            if (0 == num) continue;
            if (num > 0) positiveSum += num;
            else negativeSum += num;
        }

        if (gg >= positiveSum) return abs(positiveSum-gg);
        if (gg <= negativeSum) return abs(negativeSum-gg);

        set<LL> dpFirstHalf = {0}, dpSecondHalf = {0};
        int halfN = (n >> 1);
        int ans = LLMAX;
        for (int i = 0; i < halfN; ++i) {
            vector<int> oldVals(dpFirstHalf.begin(), dpFirstHalf.end());
            for (auto oldVal : oldVals) {
                int newVal = nums[i]+oldVal;
                bool inserted = dpFirstHalf.insert(newVal).second;
                if (inserted) ans = min(ans, abs(newVal-gg));
            }
        }

        for (int i = halfN; i < n; ++i) {
            vector<int> oldVals(dpSecondHalf.begin(), dpSecondHalf.end());
            for (auto oldVal : oldVals) {
                int newVal = nums[i]+oldVal;
                bool inserted = dpSecondHalf.insert(newVal).second;
                if (inserted) {
                    ans = min(ans, abs(newVal-gg));
                    int targetInFirstHalf = gg-newVal;
                    auto it = dpFirstHalf.lower_bound(targetInFirstHalf);
                    if (it == dpFirstHalf.end()) {
                        ans = min(ans, abs(gg-(*dpFirstHalf.rbegin() + newVal)));
                    } else {
                        ans = min(ans, abs(gg-(*it+newVal)));
                        if (it != dpFirstHalf.begin()) {
                            --it;
                            ans = min(ans, abs(gg-(*it+newVal)));
                        }
                    }
                }
            }
        }
        
        return ans;
    }
};
