bool debug = false;
typedef pair<int, int> PII;
typedef uint64_t ULL;
typedef int64_t LL;
ULL const ULLMAX = UINT64_MAX;
LL const LLMAX = INT64_MAX;

void dijkstra(int start, unordered_map<int, unordered_map<int, int>> &adj, unordered_map<int, int>& g) {        
    priority_queue< PII, vector<PII>, greater<PII> > openHeap;
    unordered_set<int> closedSet;

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
        if (debug) printf("[Dijkstra] Checking expander (minGByFar: %d, u: %d, #successors:%d)\n", minGByFar, u, nbs.size());
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

class Solution {
public:
    int reachableNodes(vector<vector<int>>& edges, int maxMoves, int n) {
        unordered_map<int, int> g;
        unordered_map<int, unordered_map<int, int>> adj;

        for (auto &edge : edges) {
            int u = edge[0], v = edge[1];
            adj[u][v] = edge[2]+1;
            adj[v][u] = edge[2]+1;
        }
        
        dijkstra(0, adj, g);

        int ans = 0, overlappedNodes = 0;
        for (int u = 0; u < n; ++u) {
            if (!g.count(u) || g[u] > maxMoves) continue;
            if (debug) printf("Checking u:%d\n", u);
            int inc = 0, incOverlapped = 0;

            // g[u] <= maxMoves
            inc += 1; // "u" itself
            for (auto &[v, distance] : adj[u]) {
                int cnt = distance-1;
                int extra = min(maxMoves-g[u], cnt);
                inc += extra;
                int extraFromV = 0;
                if (maxMoves > g[v]) {
                    extraFromV = min(maxMoves-g[v], cnt);
                }
                if (debug) printf("\t(u:%d, v:%d), extra: %d, extraFromV: %d, cnt: %d\n", u, v, extra, extraFromV, cnt);
                incOverlapped += max(0, extra+extraFromV-cnt);
            }
            if (debug) printf("\tu:%d, inc: %d, incOverlapped: %d\n", u, inc, incOverlapped);
            ans += inc;
            overlappedNodes += incOverlapped;
        }

        ans -= (overlappedNodes/2);
        return ans;
    }
};
