/*
test cases
[[1,2,7],[3,6,7]]
1
6
[[7,12],[4,5,15],[6],[15,19],[9,12,13]]
15
12
[[1,7],[3,5]]
5
5
*/
bool debug = false;
typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ;

int const MAXN = 500;
int const INVALID = -1;

int g[MAXN][MAXN];
bool closedSet[MAXN]; 

int activeSrcPos = INVALID;

class MinHeapCmp {
public:
    bool operator() (int const &lhs, int const &rhs) const {
        // Note that "INVALID" should be treated as "INFINITE".
        if (INVALID == g[activeSrcPos][lhs] && INVALID == g[activeSrcPos][rhs]) return true;
        if (INVALID == g[activeSrcPos][lhs]) return true;
        if (INVALID == g[activeSrcPos][rhs]) return false;
        return g[activeSrcPos][lhs] > g[activeSrcPos][rhs];
    }
};

void modifiedDijkstra(int s, ADJ &adj, int g[][MAXN], bool* closedSet) {  
    // Deliberately using a "priority_queue" with just "int" instead of "pair<int, int>" to speed up.
    activeSrcPos = s;
    
    priority_queue< int, vector<int>, MinHeapCmp > openHeap;
    
    // init
    openHeap.push(s);
    g[s][s] = 0;
    
    while (!openHeap.empty()) {
        int t = openHeap.top(); openHeap.pop();
        if (closedSet[t]) {
            for (auto &it : adj) {
                int v = it.first;
                if (INVALID == g[s][t] || INVALID == g[t][v]) continue;
                int cand = g[s][t] + g[t][v]; 
                if (INVALID == g[s][v] || cand < g[s][v]) {
                    g[s][v] = cand;
                }
            }
        } else {
            for (auto v : adj[t]) {
                if (INVALID == g[s][t]) continue;
                int cand = g[s][t] + 1; // weight of "t -> v" is always 1 in this problem
                if (INVALID == g[s][v] || cand < g[s][v]) {
                    g[s][v] = cand;
                    openHeap.push(v);
                }
            }
        }
    }
    
    closedSet[s] = true;
}

class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) return 0;
		/*  
        Each "r[i] containing an s[j]" indicates a "1" in the following matrix
        ```
               |   r[0] r[1] r[2] ... r[n-1]
        -------------------------------------
        s[0]   |
        s[1]   |
        s[2]   |
        .      |
        .      |
        .      |
        s[m-1] |
        ```
        where n <= 500 && each s[j] < 10^6 && m < 10^5. The constraint "sum(routes[i].length) <= 10^5" limits the number of "1"s, i.e. if each row has 500 "1"s, then there're at most only 200 rows -- fairly sparse.
        */
        int n = routes.size();
        unordered_map<int, unordered_set<int>> stopToRoutes;
        for (int u = 0; u < routes.size(); ++u) {
            auto &stops = routes[u];
            for (int stop : stops) stopToRoutes[stop].insert(u);
        }

        ADJ adjRoutes; // Easier to work with the indices than "adjStops"
        for (auto &[stop, itsRoutes] : stopToRoutes) {
            for (auto u : itsRoutes) {
                for (auto v : itsRoutes) {
                    if (u == v) continue;
                    adjRoutes[u].insert(v);
                }
            }
        }
        
        memset(closedSet, false, sizeof closedSet);
        memset(g, INVALID, sizeof g);
        for (int u = 0; u < routes.size(); ++u) {
            modifiedDijkstra(u, adjRoutes, g, closedSet);
        }

        int ans = INT_MAX;
        for (auto u : stopToRoutes[source]) {
            for (auto v : stopToRoutes[target]) {
                if (INVALID == g[u][v]) continue;
                ans = min(ans, g[u][v]);
            }
        }

        if (INT_MAX == ans) return INVALID;
        return ans+1; // "ans" is by now the minimum number of "switches", thus "+1" to indicate the number of buses
    }
};
