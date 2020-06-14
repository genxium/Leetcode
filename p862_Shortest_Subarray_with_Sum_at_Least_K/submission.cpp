class Solution {
public:
    int shortestSubarray(vector<int>& nums, int k) {
        if (0 == nums.size()) {
            return -1;
        }
        int res = INT_MAX;
        vector<int> partialSum;
        partialSum.push_back(nums[0]);
        if (nums[0] == k) {
            // The trivial case.
            return 1;
        }
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] == k) {
                // The trivial case.
                return 1;
            }
            int newPartialSum = partialSum.back() + nums[i];
            partialSum.push_back(newPartialSum);
            if (newPartialSum >= k && res > (i + 1)) {
                res = (i + 1);
            }
        }
        
        list<int> candidatePositions;
        candidatePositions.push_back(0);
        int l = candidatePositions.front(), r = candidatePositions.back();

        /*
         * Assume that "f(i) = biggest{j | j < i && partialSum[i] - partialSum[j] >= k}".
         * 
         * At each position "i" we control movement of "l" and "r" to find "f(i)". If all "nums[i]" are non-negative, thus "partialSum[i]" is non-decreasing, the O(N) solution is pretty straight forward.
         * for (r = 0; r < partialSum.size(); ++r) {
         *   // It's safe to stop incrementing "l" once an "l'" where "partialSum[r] - partialSum[l'] < k" is met for the current "r", due to that "partialSum[]" is non-decreasing.
         *   while (partialSum[r] - partialSum[l] >= k && l < r) {
         *     if (res > (r - l)) {
         *       res = (r - l);
         *     }
         *     ++l; 
         *   }
         *   // by now we've got "f(r) == l-1" BECAUSE "partialSum[r] - partialSum[l] < k && partialSum[r] - partialSum[l-1] >= k" after the while-loop above
         * }
         *
         * However, this problem claims to have negative "nums[i]", thus a fluctuating "partialSum[]". Notice that for any "i > r && partialSum[i] <= partialSum[r]", it's SAFE TO ASSERT that "there won't be any u > i such that f(u) = r", because "in this case f(u) = i is always better", making such value of "r" OBSOLETE HERE AFTER.
         * 
         * Therefore when inspecting "partialSum[i]" against "partialSum[r]" where "l == candidatePositions.front(), r == candidatePositions.back(), l < r < i", if "partialSum[i] <= partialSum[r]", we can just 
         * - do "candidatePositions.pop_back()" then "r = candidatePositions.back()"
         * - recheck "partialSum[i]" against "partialSum[r]" and loop the popping back if needed
         * - do "candidatePositions.push_back(i)" then "r = candidatePositions.back()"
         * 
         * , which will result in a monotonically increasing "partialSum[candidatePositions[...]]".
         *
         * What's more, at each "i" where "l < r < i" and "f(r) == l-1", although it's possibly true that "f(i) == f(r) == l-1", we should start seeking "f(i)" from "l" for a smaller length than "r-(l-1)".
         */
        for (int i = 1; i < nums.size(); ++i) {
            while(false == candidatePositions.empty() && partialSum[r] >= partialSum[i]) {
                candidatePositions.pop_back();
                r = candidatePositions.back();
                l = candidatePositions.front();
            }

            while (false == candidatePositions.empty() && partialSum[i] - partialSum[l] >= k) {
                if (res > (i - l)) {
                    res = (i - l);
                }
                candidatePositions.pop_front();
                r = candidatePositions.back();
                l = candidatePositions.front();
            }
            
            candidatePositions.push_back(i);
            r = candidatePositions.back();
            l = candidatePositions.front();
        }
        if (INT_MAX == res) {
            return -1;
        }
        return res;
    }
};
