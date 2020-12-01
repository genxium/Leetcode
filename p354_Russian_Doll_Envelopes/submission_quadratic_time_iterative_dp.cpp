/*
test cases
[[5,4],[6,4],[6,7],[2,3]]
[[1,3],[3,5],[6,7],[6,8],[8,4],[9,5]]
*/
typedef pair<int, int> PII;

class Solution {
public:
    bool isCompatible(PII const &lhs, PII const &rhs) {
        if (lhs.first == rhs.first) return false;
        if (lhs.first < rhs.first) {
            return lhs.second < rhs.second;
        } else {
            return lhs.second > rhs.second;
        }
    }

    int maxEnvelopes(vector<vector<int>>& envelopes) {
        int n = envelopes.size();
        vector<PII> items;
        for (int i = 0; i < n; ++i) {
            auto &envelope = envelopes[i];
            items.push_back({envelope[0], envelope[1]});
        }
        sort(items.begin(), items.end(), less<PII>());
        // sort(items.begin(), items.end(), greater<PII>()); // Also works!
        
        vector<int> dp(n); // "dp[k] == x" means that using "items[0, ..., k]" we can form a set of "x" envelopes if MUST USE "items[k]"
        
        int ans = 0;
        for (int k = 0; k < n; ++k) {
            auto &anchorItem = items[k]; 
            dp[k] = 1;
            for (int i = 0; i < k; ++i) {
                if (false == isCompatible(anchorItem, items[i])) continue;
                dp[k] = max(dp[k], dp[i] + 1);
            }
            ans = max(ans, dp[k]);
        }
        return ans;
    }
};
