typedef pair<int, int> PII;

struct RevCmp {
    bool operator() (int lhs, PII const& rhs) {
        return lhs > rhs.first;
    }
};

/*
test cases
[1,4,3,7,4,5]
3
[5,5,4,5,4,1,1,1]
0
*/
class Solution {
public:
    int maximumScore(vector<int>& nums, int k) {
        int n = nums.size();

        vector<PII> dp1, dp2;
        int min1 = nums[k]; // the min by [k, j]
        for (int j = k; j < n; ++j) {
            min1 = min(min1, nums[j]); // monotonically decreasing during traversal 
            dp1.push_back({min1, j-k+1});
        }

        int min2 = nums[k]; // the min by [i, k] traversed in reverse order 
        for (int i = k; i >= 0; --i) {
            min2 = min(min2, nums[i]); // monotonically decreasing during traversal
            dp2.push_back({min2, k-i+1});
        }

        int ans = 0;
        for (auto &[targetMin, len] : dp1) {
            ans = max(ans, targetMin*len);
            if (dp2.size() > 0) {
                // If "targetMin" should be the minimum in the range of interest
                auto it = upper_bound(dp2.begin(), dp2.end(), targetMin, RevCmp()); 
                --it;
                ans = max(ans, targetMin*(len+it->second-1));
            }
        }

        for (auto &[targetMin, len] : dp2) {
            ans = max(ans, targetMin*len);
            if (dp1.size() > 0) {
                auto it = upper_bound(dp1.begin(), dp1.end(), targetMin, RevCmp()); 
                --it;
                ans = max(ans, targetMin*(len+it->second-1));
            }
        }

        return ans;
    }
};
