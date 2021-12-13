bool debug = false;
typedef unordered_map<int, int> ADJ; 
typedef pair<int, int> PII;
typedef uint64_t ULL;
typedef int64_t LL;
ULL const ULLMAX = UINT64_MAX;
LL const LLMAX = INT64_MAX;

void dijkstra(int start, ADJ &adj, int n2, unordered_map<int, int>& g) {        
    priority_queue< PII, vector<PII>, greater<PII> > openHeap;

    unordered_set<int> closedSet;

    // init
    openHeap.push({0, start});
    g[start] = 0;

    // loop
    while (false == openHeap.empty()) {
        auto expander = openHeap.top(); openHeap.pop();
        int u = expander.second;    
        int minGByFar = expander.first;
        closedSet.insert(u);

        if (debug) printf("Expanding u = %d\n", u);
        for (int steps = 1; steps <= 6 && u+steps <= n2; ++steps) {
            int v = u+steps;
            int candidate = minGByFar + 1;
            if (adj.count(v)) {
                // In case of snake or ladder, this "v" is NOT IMMEDIATELY REACHABLE!
                v = adj[v];
            }
            if (closedSet.count(v)) continue;
            if (debug) printf("\tv = %d\n", v);
            if (!g.count(v) || candidate < g[v]) {
                if (debug) printf("\tinserting v = %d into openHeap\n", v);
                g[v] = candidate;
                openHeap.push({candidate, v});
            }   
        }
    }
}

/*
test cases
[[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,-1,-1,-1,-1,-1],[-1,35,-1,-1,13,-1],[-1,-1,-1,-1,-1,-1],[-1,15,-1,-1,-1,-1]]
[[-1,-1],[-1,3]]
[[-1,-1,-1],[-1,9,8],[-1,8,9]]
[[1,1,-1],[1,1,1],[-1,1,1]]
*/
class Solution {
public:
    int snakesAndLadders(vector<vector<int>>& board) {
        int n = board.size(), n2 = n*n;
        ADJ adj;
        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < n; ++x) {
                if (-1 == board[y][x]) continue;
                int stepIdx = -1;
                if ((n & 1) > 0) {
                    stepIdx = ((y & 1) == 0) ? n2-(y*n + n-x-1) : n2-(y*n + x);
                } else {
                    stepIdx = ((y & 1) > 0) ? n2-(y*n + n-x-1) : n2-(y*n + x);
                }
                adj[stepIdx] = board[y][x];
                if (debug) printf("Snake or ladder, adj[%d] -> %d\n", stepIdx, board[y][x]);
            }
        }
        
        unordered_map<int, int> g;
        int start = 1;        
        dijkstra(start, adj, n2, g);
        int ans = n2+1;
        if (g.count(n2)) {
            ans = min(ans, g[n2]);
        }
        
        if (n2+1 == ans) ans = -1;
        
        return ans;
    }
};
