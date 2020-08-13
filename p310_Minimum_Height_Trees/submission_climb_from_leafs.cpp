class Solution {
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (1 == n) {
            return {0};
        }
        unordered_map<int, unordered_set<int>> adj;
        int E = edges.size();
        for (int i = 0; i < E; ++i) {
            int u = edges[i][0];
            int v = edges[i][1];
        
            if (adj.find(u) == adj.end()) {
                adj[u] = unordered_set<int>{};
            } 
            adj[u].insert(v);
          
            if (adj.find(v) == adj.end()) {
                adj[v] = unordered_set<int>{};
            }
            adj[v].insert(u);
        }
        
        queue<int> openQueue;
        unordered_set<int> closedSet; // in fact it's unnecessary to use a "closedSet" in this solution, because "visited leafs" are removed from "adj" once drawn from queue
        
        for (auto &it : adj) {
            if (1 == it.second.size()) {
                openQueue.push(it.first);
            }
        }
        
        int V = n;
        while (V > 2) {
            int sizeThisRound = openQueue.size();
            while (sizeThisRound--) {
                int u = openQueue.front(); openQueue.pop();
                --V;
                closedSet.insert(u);
                auto nbs = adj[u];
                for (auto &nb : nbs) {
                    if (closedSet.find(nb) != closedSet.end()) continue;
                    adj[nb].erase(u);
                    if (1 == adj[nb].size()) {
                        // just made a new leaf
                        openQueue.push(nb);
                    }
                }   
            }
        }
        
        vector<int> ans;
        while (false == openQueue.empty()) {
            ans.push_back(openQueue.front());
            openQueue.pop();
        }
        
        return ans;
    }
};
