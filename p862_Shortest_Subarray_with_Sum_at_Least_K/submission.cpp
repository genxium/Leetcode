class Solution {
public:
    int shortestSubarray(vector<int>& nums, int k) {
        if (0 == nums.size()) {
            return -1;
        }

        // Prepare the "prefixSum[]".
        int res = INT_MAX;
        vector<int> prefixSum = {0};

        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] == k) {
                // The trivial case.
                return 1;
            }
            int newPartialSum = prefixSum.back() + nums[i];
            if (newPartialSum >= k && res > (i + 1)) {
                res = (i + 1);
            }
            prefixSum.push_back(newPartialSum);
        }

        /*
         * Denote "f(r) = biggest{t | t < r && sum[t, ..., r] >= k}", mind that this "f()" WON'T explicitly show in codes, it's just for proving correctness here. 
         * 
         * IF all "nums[i]" WERE non-negative the O(N) solution is pretty straight forward. At each position "i" we control movement of a pair of {l, r} to find "f(i)".
         *
         * for (r = 0; r < nums.size(); ++r) {
         *     // It's safe to stop incrementing "l" once "sum[l, ..., r] < k" is met for the current "r".
         *     while (sum[l, ..., r] >= k && l < r) {
         *         if (res > (r - l)) {
         *             res = (r - l);
         *         }
         *         ++l; 
         *     }
         *     // by now we've got "f(r) == l-1" BECAUSE "sum[l, ..., r] < k && sum[l-1, r] >= k" after the while-loop above
         * }
         *
         * HOWEVER, this problem claims to have negative "nums[i]", thus a fluctuating "prefixSum[]".
         */ 

        list<int> candidatePositions;
        candidatePositions.push_back(0);
        int l = candidatePositions.front(), r = candidatePositions.back();
        
        /*
         * [KEY IDEA] Notice that for any "i > r && sum[0, ..., i] <= sum[0, ..., r]", it's SAFE TO ASSERT that "there WON'T BE any j > i > r, such that f(j) = r", because "in this case f(j) = i is ALWAYS BETTER", making such value of "r" OBSOLETE HERE AFTER.
         * 
         * Therefore when inspecting any "sum[0, ..., i]" and a pair {l, r} where "l == candidatePositions.front(), r == candidatePositions.back(), i > r > l", if "sum[0, ..., i] <= sum[0, ..., r]", we can just 
         * - do "candidatePositions.pop_back()" then "r = candidatePositions.back()"
         * - recheck "sum[0, ..., i]" against "sum[0, ..., r]" and LOOP THE POPPING BACK IF NEEDED
         * - do "candidatePositions.push_back(i)" then "r = candidatePositions.back()"
         * 
         * , which will result in a monotonically increasing "prefixSum[candidatePositions[...]]".
         *
         * By having a monotonically increasing "prefixSum[candidatePositions[...]]", we can then keep an "ever-increasing `l`" to search for each "f(i)". 
         */
        
        for (int i = 1; i < nums.size(); ++i) {
            while (!candidatePositions.empty() && prefixSum[r+1] >= prefixSum[i+1]) {
                candidatePositions.pop_back();
                r = candidatePositions.back();
            }

            while (!candidatePositions.empty() && (prefixSum[i+1] - prefixSum[l+1] >= k)) {
                if (res > (i - l)) {
                    res = (i - l);
                }
                candidatePositions.pop_front();
                l = candidatePositions.front();
            }
            
            candidatePositions.push_back(i);
            l = candidatePositions.front();
            r = candidatePositions.back();
        }

        if (INT_MAX == res) {
            return -1;
        }
        return res;
    }
};

