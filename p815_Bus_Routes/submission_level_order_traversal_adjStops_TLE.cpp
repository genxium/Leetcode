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

int const INVALID = -1;
int const MAXN = 1000000;
int openSet[MAXN], closedSet[MAXN];

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
        ADJ adjStops;
        unordered_map<int, unordered_set<int>> routeSets;
        for (int i = 0; i < n; ++i) {
            for (int s : routes[i]) {
                routeSets[s].insert(i);
            }
        }
        vector<int> stops;
        for (auto& [u, rsU] : routeSets) stops.push_back(u);
        
        // Here comes the SLOW part, note again that "m = stops.size() < 10^5", hence a nested loop of O(m^2) time-complexity will get TLE!
        for (int j = 0; j < stops.size(); ++j) {
            int u = stops[j];
            for (int k = j+1; k < stops.size(); ++k) {
                int v = stops[k];
                for (auto i : routeSets[u]) {
                    if (!routeSets[v].count(i)) continue;
                    if (debug) printf("stops {%d, %d} are connected by route:%d\n", u, v, i);
                    // stops u and v are connected
                    adjStops[u].insert(v);
                    adjStops[v].insert(u);
                    break;
                }
            }
            if (debug) {
                printf("adjStops[%d]: ", u);
                for (auto v : adjStops[u]) printf(" %d", v);
                puts("");
            }
        }
        
        // level-order-traversal
        int lv = 0;
        queue<int> q;
        memset(openSet, false, sizeof openSet);
        memset(closedSet, false, sizeof closedSet);
        q.push(source);
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                int u = q.front(); q.pop();
                openSet[u] = false; closedSet[u] = true;
                if (target == u) return lv;
                for (int v : adjStops[u]) {
                    if (openSet[v] || closedSet[v]) continue;
                    q.push(v);
                }
            }
            ++lv;
        }
        
        return INVALID;
    }
};
