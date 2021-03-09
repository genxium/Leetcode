bool debug = false;
typedef pair<int, int> PII;
typedef uint64_t ULL;
ULL const MOD = 1000000007;
ULL const LLMAX = UINT64_MAX;

void dijkstra(int start, unordered_map<int, unordered_map<int, int>> &adj, unordered_map<int, int>& g) {        
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


ULL dfs(int u, int end, unordered_map<int, unordered_map<int, int>> &adj, unordered_map<int, int>& g, vector<ULL> &memo) {
    if (u == end) return memo[u] = 1; // 1 way to reach end from end
    if (LLMAX != memo[u]) return memo[u];
    memo[u] = 0; // guaranteed a way to the end
    for (auto &[v, w] : adj[u]) {
        if (g[v] >= g[u]) continue;
        ULL cand = dfs(v, end, adj, g, memo);
        memo[u] += cand;
        memo[u] %= MOD;
    }
    
    return memo[u];
}

class Solution {
public:
    int countRestrictedPaths(int n, vector<vector<int>>& edges) {
        unordered_map<int, int> g;
        unordered_map<int, unordered_map<int, int>> adj;
        
        for (auto &edge : edges) {
            int u = --edge[0], v = --edge[1];
            adj[u][v] = edge[2];
            adj[v][u] = edge[2];
        }
        
        dijkstra(n-1, adj, g);
            
        vector<ULL> memo(n, LLMAX);
        
        return dfs(0, n-1, adj, g, memo);
    }
};
