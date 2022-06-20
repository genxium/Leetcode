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
typedef pair<int, int> PII;

int const MAXN = 500;
int isTarget[MAXN], openSet[MAXN], closedSet[MAXN];
int const INVALID = -1;

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
        
        queue<int> q;
        memset(isTarget, false, sizeof isTarget);
        memset(openSet, false, sizeof openSet);
        memset(closedSet, false, sizeof closedSet);
        ADJ adjRoutes; // Easier to work with the indices than "adjStops"
        for (auto &[stop, itsRoutes] : stopToRoutes) {
            for (auto u : itsRoutes) {
                if (stop == target) isTarget[u] = true;
                if (stop == source) q.push(u);
                for (auto v : itsRoutes) {
                    if (u == v) continue;
                    adjRoutes[u].insert(v);
                }   
            }   
        }
        
        // level-order-traversal
        int lv = 1;
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                int u = q.front(); q.pop();
                openSet[u] = false; closedSet[u] = true;
                if (isTarget[u]) return lv;
                for (int v : adjRoutes[u]) {
                    if (openSet[v] || closedSet[v]) continue;
                    q.push(v);
                }
            }
            ++lv;
        }
        
        return INVALID;
    }
};
