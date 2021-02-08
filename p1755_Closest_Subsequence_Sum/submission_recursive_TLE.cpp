typedef int64_t LL;
unordered_map<int, unordered_map<LL, LL>> dp;

class Solution {
private: 
    LL LOWEST;
    int firstPositivePos;

public:
    LL dfs(int i, vector<int> &nums, LL g, int level) {
        // Returns the closest subset sum of "nums[i, ...]" w.r.t. "g"
        if (i == nums.size()) {
            // There's no other possible subset sum.
            return 0;
        }

        int x = nums[i];
        if (x > 0 && g <= 0) {
            // Because "nums[]" is sorted in ascending order.
            return dp[i][g] = 0;
        }

        if (x < 0 && g <= LOWEST) {
            // Must skip "x" because "nums[]" is sorted in ascending order.
            return dp[i][g] = dfs(firstPositivePos, nums, g, level+1);
        }

        // Pick "x".
        /*
        Note that if "|a-(g-x)| < |b-(g-x)|", then "|(a+x)-g| < |(b+x)-g|" and we should assign "(a+x)" to "cand1".
        */
        LL cand1 = x + dfs(i+1, nums, g-x, level+1); 

        // Skip "x".
        LL cand2 = dfs(i+1, nums, g, level+1);

        if (abs(g-cand1) < abs(g-cand2)) return dp[i][g] = cand1;
        else return dp[i][g] = cand2;
    }

    int minAbsDifference(vector<int>& nums, int goal) {
        dp.clear();
        LL g = goal;
        int n = nums.size();
        firstPositivePos = n;
        sort(nums.begin(), nums.end());
        LL positiveSum = 0;
        for (int i = 0; i < n; ++i) {
            int num = nums[i];
            if (num > 0) {
                if (n == firstPositivePos) firstPositivePos = i;
                positiveSum += num;
            }
        }
        LOWEST = g-positiveSum;
        
        return abs(g-dfs(0, nums, g, 0));
    }
};
