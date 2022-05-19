bool const debug = false;
typedef vector<int> VI;
typedef pair<int, int> PII;
int const MAXN = 100;
vector<int> dy = {0, 0, -1, +1};
vector<int> dx = {-1, +1, 0, 0};
bool const INVALID = -1;

bool closedSet[MAXN*MAXN];
int g[MAXN*MAXN];

class Solution {
public:
    int minimumEffortPath(vector<vector<int>>& heights) {
        memset(closedSet, false, sizeof closedSet);
        memset(g, INVALID, sizeof g);

        priority_queue<PII, vector<PII>, greater<PII>> pq; // min-heap of {g[u], u}
        int m = heights.size();
        int n = heights[0].size();
        g[0] = 0;
        pq.push({g[0], 0});
        while (!pq.empty()) {
            auto expander = pq.top(); pq.pop();
            int u = expander.second;
            int uy = u/n, ux = u%n;
            if (closedSet[u]) {
                if (debug) printf("\tg[uy:%d][ux:%d]:%d, ALREADY closed\n", uy, ux, g[u]);
                continue;
            }
            closedSet[u] = true; // by now, no other route can further reduce g[u], consider a few examples if not understood
            int minGByFar = expander.first;
            if (debug) printf("Checking Neighbours for u = %d, g[uy:%d][ux:%d]:%d\n", u, uy, ux, g[u]);
            for (int i = 0; i < 4; ++i) {
                int vy = uy+dy[i], vx = ux+dx[i];
                if (vy < 0 || vy >= m || vx < 0 || vx >= n) {
                    if (debug) printf("\tOut of range [vy:%d][vx:%d]\n", vy, vx);
                    continue;
                }
                int v = vy*n + vx;
                if (closedSet[v]) continue;
                if (debug) printf("from heights[uy:%d][ux:%d]:%d, trying to update heights[vy:%d][vx:%d]:%d\n", uy, ux, heights[uy][ux], vy, vx, heights[vy][vx]);
                int candG = max(g[u], abs(heights[vy][vx] - heights[uy][ux]));
                if (INVALID == g[v] || candG < g[v]) {
                    g[v] = candG;
                    pq.push({g[v], v});
                    if (debug) printf("\tupdated g[vy:%d][vx:%d]:%d, pushed v:%d into pq\n", vy, vx, g[v], v);
                }
            }
            if (debug) printf("now pq.size() is %lu, pq.empty() is %d\n", pq.size(), pq.empty());
        }
        
        return g[m*n-1];
    }
};
