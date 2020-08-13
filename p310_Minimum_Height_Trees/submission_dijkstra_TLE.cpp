class Solution {
public:
    int dijkstra(int start, unordered_map<int, vector<int>> &adj) {        
        // See https://www.yinxiang.com/everhub/note/8f531e5d-f4ab-4164-a459-266ab4b8cda8 for basics of the Dijkstra algorithm.
        priority_queue< pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > openHeap;
        set<int> closedSet;
        unordered_map<int, int> g; // "g[u] == x" being the shortest path to reach "u" from "start"
        
        // init
        openHeap.push({0, start});
        g[start] = 0;
        int toRet = 0;
        
        // loop
        while (false == openHeap.empty()) {
            auto expander = openHeap.top(); openHeap.pop();
            int u = expander.second;
            int minGByFar = expander.first;
            if (minGByFar > toRet) {
                toRet = minGByFar;
            }
            
            //printf("checking expander (minGByFar: %d, u: %d)\n", minGByFar, u);
            closedSet.insert(u);
            auto nbs = adj[u];
            for (auto &nb : nbs) {
                int v = nb;
                if (closedSet.find(v) != closedSet.end()) continue;

                int candidate = minGByFar + 1;
                if (g.find(v) == g.end() || candidate < g[v]) {
                    g[v] = candidate;
                    openHeap.push({candidate, v});
                }
            }
        }
        return toRet;    
    }
    
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        unordered_map<int, vector<int>> adj;
        int E = edges.size();
        for (int i = 0; i < E; ++i) {
            int u = edges[i][0];
            int v = edges[i][1];
        
            if (adj.find(u) == adj.end()) {
                adj[u] = vector<int>{};
            } 
            adj[u].push_back(v);
          
            if (adj.find(v) == adj.end()) {
                adj[v] = vector<int>{};
            }
            adj[v].push_back(u);
        }
        
        vector<int> ans;
        int minDepthByFar = INT_MAX;
        for (int i = 0; i < n; ++i) {
            int depth = dijkstra(i, adj);
            //printf("depth rooted at i:%d is %d\n", i, depth);
            if (depth < minDepthByFar) {
                minDepthByFar = depth;
                ans.clear();
                ans.push_back(i);
            } else if (depth == minDepthByFar) {
                ans.push_back(i);
            } else {
                // depth > minDepthByFar
            }
        }
        return ans;
    }
};
