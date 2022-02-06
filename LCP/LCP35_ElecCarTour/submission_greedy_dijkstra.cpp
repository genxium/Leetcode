bool debug = false;
typedef vector<int> VI;
typedef unordered_map<int, unordered_map<int, int>> ADJ; 
int const INVALID = -1;
int const MAXN = 100;
int const MAXC = 100;
int const OFFSETWIDTH = MAXC;
int g[MAXN*MAXC];
bool closedSet[MAXN*MAXC];

int dijkstra(int start, int end, ADJ &adj, VI& charge, int cnt) {   
    memset(g, INVALID, sizeof g);
    memset(closedSet, false, sizeof closedSet);     
    priority_queue< VI, vector<VI>, greater<VI> > openHeap; // Note that for a same candidate "g[]" value, the less remaining fuel the more prior to be popped from "openHeap"

    // init
    openHeap.push({0, 0, start});
    g[start*OFFSETWIDTH+0] = 0;

    // loop
    while (!openHeap.empty()) {
        auto expander = openHeap.top(); openHeap.pop();
        int minGByFar = expander[0];
        int c = expander[1]; // remaining charge
        int u = expander[2]; // index of city    
        int translatedU = u*OFFSETWIDTH+c; 
        
        if (closedSet[translatedU]) {
            // a CRITICAL STEP to speed up runtime
            // An outdated expander whose "expander[0]" was already updated to a smaller value and popped earlier
            if (debug) printf("Not expanding u = %d, c = %d because closedSet[translatedU:%d]\n", u, c, translatedU);
            continue;
        }
        
        if (u == end) return minGByFar;
        closedSet[translatedU] = true;

        if (debug) printf("Expanding u = %d, c = %d\n", u, c);
        if (c < cnt) {
            int rc = c+1;
            // another CRITICAL STEP to avoid storing and traversing a "adj[MAXN*MAXC][MAXN*MAXC]", just traverse all possible remaining charges in-place when necessary
            // it can be seen that the shortest time to reach "end" must be reaching it with "0 remaining charge", because we started with "0 remaining charge", i.e. if there were a path reaching "end" with above 0 charge, we could've reduced the total time by reducing charging time of a predecessor in the path -- hence always try greedily
            int translatedV = u*OFFSETWIDTH + rc;
            int candidate = minGByFar + (rc-c)*charge[u];
            if (!closedSet[translatedV] && (INVALID == g[translatedV] || candidate < g[translatedV])) {
                g[translatedV] = candidate;
                openHeap.push({candidate, rc, u});
            }
        }

        for (auto& [v, w] : adj[u]) {
            if (c < w) continue;
            int rc = c-w; // remaining charge
            int translatedV = v*OFFSETWIDTH+rc;
            int candidate = minGByFar + w;
            if (!closedSet[translatedV] && (INVALID == g[translatedV] || candidate < g[translatedV])) {
                g[translatedV] = candidate;
                openHeap.push({candidate, rc, v});
            }
        }
    }

    return INT_MAX;
}

class Solution {
public:
    int electricCarPlan(vector<vector<int>>& paths, int cnt, int start, int end, vector<int>& charge) {
        ADJ adj; // sparse directed weighted graph
        for (auto &path : paths) {
            int i = path[0], j = path[1], w = path[2];
            if (adj.count(i) && adj[i].count(j) && w >= adj[i][j]) continue;
            adj[i][j] = w;
            adj[j][i] = w;
        }
        return dijkstra(start, end, adj, charge, cnt);
    }
};
