bool const debug = true;

class Solution {
public:
    vector<int> movesToStamp(string stamp, string target) {
        // Start by the last step, therefore at last this "ans" will be reversed
        vector<int> ans;
        queue<int> q;

        int n = target.length(), l = stamp.length();
        vector<bool> fulfilled(n, false);
        
        vector<pair<unordered_set<int>, unordered_set<int>>> subarrs(n-l+1); // subarrs[i] refers to "target[i, ..., i+l)", where each "subarrs[i].first" contains the letters already fulfilled  
        for (int i = 0; i+l <= n; ++i) {
            // Consider each subarray of "target[i, ..., i+l)"
            for (int j = i; j < i+l; ++j) {
                if (stamp[j-i] == target[j]) subarrs[i].first.insert(j);
                else subarrs[i].second.insert(j);
            }
            if (subarrs[i].second.empty()) {
                // This subarr can be stamped last. If there're multiple such subarrs, the order doesn't matter.
                for (auto &j : subarrs[i].first) {
                    fulfilled[j] = true;
                    q.push(j);                    
                }
                ans.push_back(i);
            }
        }
        
        while (!q.empty()) {
            int j = q.front(); q.pop();
            int iLower = max(j-l+1, 0), iUpper = min(j, n-l); // The "subarrs[i]" that covers "target[j]"
            for (int i = iLower; i <= iUpper; ++i) {
                // This check ensures that a same "i" won't re-enter "ans".
                if (subarrs[i].second.count(j)) {
                    subarrs[i].second.erase(j);
                    if (subarrs[i].second.empty()) {
                        for (auto &k : subarrs[i].first) {
                            if (fulfilled[k]) continue;
                            fulfilled[k] = true;
                            q.push(k);                    
                        }
                        ans.push_back(i);
                    }
                }
            }
        }
        
        for (auto b : fulfilled) if (!b) return vector<int>();
        
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
