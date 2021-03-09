bool debug = false;
typedef pair<int, int> PII;
/*
test cases
[[2,1,1],[2,3,1],[3,4,1]]
4
2
[[1,2,1]]
2
1
[[1,2,1]]
2
2
[[1,2,1],[2,1,3]]
2
2
*/
class Solution {
public:
    void dijkstra(int start, unordered_map<int, unordered_map<int, int>> &adj, unordered_map<int, int>& g) {        
        // See https://www.yinxiang.com/everhub/note/8f531e5d-f4ab-4164-a459-266ab4b8cda8 for basics of the Dijkstra algorithm.
        priority_queue< PII, vector<PII>, greater<PII> > openHeap;
        set<int> closedSet;
        
        // init
        openHeap.push({0, start});
        g[start] = 0;
        
        // loop
        while (false == openHeap.empty()) {
            auto expander = openHeap.top(); openHeap.pop();
            int minGByFar = expander.first;
            int u = expander.second;            
            closedSet.insert(u);
            auto &nbs = adj[u];
            if (debug) printf("checking expander (minGByFar: %d, u: %d, #successors:%d)\n", minGByFar, u, nbs.size());
            for (auto &nb : nbs) {
                int v = nb.first;
                if (closedSet.count(v)) continue;
                int candidate = minGByFar + nb.second;
                if (!g.count(v) || candidate < g[v]) {
                    g[v] = candidate;
                    openHeap.push({candidate, v});
                }
            }
        }    
    }

    int networkDelayTime(vector<vector<int>>& edges, int n, int k) {
        unordered_map<int, unordered_map<int, int>> adj;
        for (auto &edge : edges) {
            int u = edge[0]-1;
            int v = edge[1]-1;
            adj[u][v] = edge[2];
        }
        unordered_map<int, int> g; // "g[u] == x" being the shortest path to reach "u" from "start"
        --k;
        dijkstra(k, adj, g);
        if (g.size() < n) return -1;
        
        int ans = 0;
        for (auto it : g) {
            ans = max(ans, it.second);
        }
        
        return ans;
    }
};
