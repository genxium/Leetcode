#define INVALID -1
#define MAXN 100

class Solution {
private:
    unordered_map<int, unordered_set<int>> predecessors;
    void updatePredecessors(int v, list<int> &predecessorsByFar, unordered_map<int, list<int>> &adj) {
        for (auto u : predecessorsByFar) {
            predecessors[v].insert(u);
        }
        predecessorsByFar.push_back(v);
        auto nbs = adj[v];
        for (auto &nb : nbs) {
            updatePredecessors(nb, predecessorsByFar, adj);
        }
        predecessorsByFar.pop_back();
    }

public:
    vector<bool> checkIfPrerequisite(int n, vector<vector<int>>& prerequisites, vector<vector<int>>& queries) {
        unordered_set<int> roots;
        for (int u = 0; u < n; ++u) {
            roots.insert(u);
        }
            
        unordered_map<int, list<int>> adj;
        for (auto &edge : prerequisites) {
            int u = edge[0];
            int v = edge[1];
            if (adj.find(u) == adj.end()) {
                adj[u] = {};
            }
            adj[u].push_back(v);
            roots.erase(v);
        }
        
        list<int> predecessorsByFar;
        for (auto &v : roots) {
            updatePredecessors(v, predecessorsByFar, adj);
        }
        
        vector<bool> ans;
        for (auto &query : queries) {
            auto predecessorsOfLatter = predecessors[query[1]];
            if (predecessorsOfLatter.find(query[0]) == predecessorsOfLatter.end()) {
                ans.push_back(false);
            } else {
                ans.push_back(true);
            }
        }
        
        return ans;
    }
};
