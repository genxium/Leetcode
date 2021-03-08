class Solution {
public:
    bool isPoorerThan(int v, int u, vector<unordered_map<int, bool>> &memo, vector<unordered_set<int>> &poorerAdj) {
        if (memo[v].count(u)) return memo[v][u];
        bool ret = false;
        for (auto &nb : poorerAdj[v]) {
            if (nb == u) {
                ret = true;
                break;
            }
            
            ret |= isPoorerThan(nb, u, memo, poorerAdj); // The graph is guaranteed to have no cycle so no need to mark "visited[]"
            if (ret) break;
        }
        
        return memo[v][u] = ret;
    }
    
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        
        // [WARNING] Using "Floyd-Warshall" for this problem will result in TLE!
        vector<unordered_set<int>> poorerAdj(n);
        for (auto &p : richer) {
            poorerAdj[p[1]].insert(p[0]); // p[1] is poorer than p[0]
        }
        
        vector<unordered_map<int, bool>> memo(n);
        
        vector<int> ans;
        for (int v = 0; v < n; ++v) {
            int targetU = v; // itself
            int targetQ = quiet[v];
            for (int u = 0; u < n; ++u) {
                if (isPoorerThan(v, u, memo, poorerAdj) && quiet[u] < targetQ) {
                    targetU = u;
                    targetQ = quiet[u];
                }
            }
            
            ans.push_back(targetU);
        }
        
        return ans;
    }
};
