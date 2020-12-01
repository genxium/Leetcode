typedef pair<int, int> PII;
/*
test cases
[1,3,5,10,15]
[1,2,3,4,5]
[4,5,6,5]
[2,1,2,1]
[1,2,3,5]
[8,9,10,1]
[9,2,8,8,2]
[4,1,3,3,5]
*/
class Solution {
public:
    bool isCompatible(PII const &lhs, PII const &rhs) {
        if (lhs.first == rhs.first) return true;
        if (lhs.first < rhs.first) {
            return lhs.second <= rhs.second;
        } else {
            return lhs.second >= rhs.second;
        }
    }
    
    int bestTeamScore(vector<int>& scores, vector<int>& ages) {
        // Note that in this problems, the two dimensions "score & age" are in fact totally symmetric.
        int n = scores.size();
        vector<PII> items;
        for (int i = 0; i < n; ++i) {
            items.push_back({ages[i], scores[i]});
        }
        sort(items.begin(), items.end(), less<PII>());
        // sort(items.begin(), items.end(), greater<PII>()); // Also works!
        
        vector<int> dp(n); // "dp[k] == x" means that using "items[0, ..., k]" we can form a team of score "x" if MUST USE "items[k]"
        
        int ans = 0;
        for (int k = 0; k < n; ++k) {
            auto &anchorItem = items[k]; 
            dp[k] = anchorItem.second;
            for (int i = 0; i < k; ++i) {
                if (false == isCompatible(anchorItem, items[i])) continue;
                dp[k] = max(dp[k], dp[i] + anchorItem.second);
            }
            ans = max(ans, dp[k]);
        }
        return ans;
    }
};
