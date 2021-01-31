bool const debug = false;
typedef pair<int, int> PII;

/*
test cases
[1]
[1,3,5,4,7]
[2,2,2,2,2]
*/
class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        vector<PII> dp; // "dp[i] == {x, y}" means that ending exactly at "nums[i]", the LIS length is "x" and there're "y" count of it
        
        int lisLength = 1;
        
        // init
        dp.push_back({1, 1});
        
        // loop
        for (int i = 1; i < nums.size(); ++i) {
            int maxBefore = 0, maxBeforeCnt = 1;
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    if (maxBefore < dp[j].first) {
                        maxBefore = dp[j].first;
                        maxBeforeCnt = dp[j].second;
                    } else if (maxBefore == dp[j].first) {
                        maxBeforeCnt += dp[j].second;
                    }
                }
            }
            dp.push_back({1+maxBefore, maxBeforeCnt});
            if (lisLength < dp[i].first) {
                lisLength = dp[i].first;
            }
        }
        
        int ans = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (dp[i].first == lisLength) ans += dp[i].second;
        }
        
        return ans;
    }
};
