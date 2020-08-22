class Solution {
public:
    /*
    The term "lis" refers to "LongestIncreasingSubsequence". See https://github.com/genxium/Leetcode/blob/master/p300_Longest_Increasing_Subsequence for more information.
    */
    bool lisNlogn(vector<int>& nums) {
        /*
        Consider that "nums[] == {100, 200, 300, 400, 1, 201, 3, 500}"
                                                    ^
        , our "sortedCache" grew to {100, 200, 300, 400} successfully, and would be updated to {1, 200, 300, 400}, then {1, 200, 201, 400}, and then {3, 200, 201, 400}, and finally {3, 200, 201, 400, 500}.
        */
        vector<int> sortedCache;
        for (auto &num : nums) {
            auto lower = lower_bound(sortedCache.begin(), sortedCache.end(), num);
            if (lower == sortedCache.end()) {
                sortedCache.push_back(num);
                if (sortedCache.size() >= 3) return true;
            } else {
                *lower = num;
            }
        }
        return false;
    }

    bool increasingTriplet(vector<int>& nums) {
        return lisNlogn(nums);
    }
};
