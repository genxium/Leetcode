bool debug = true;
#define RED +1
#define BLUE -1
#define UNCOLORED 0
#define MAXN 2000
int colors[MAXN];

void dfs(int prevColor, int u, vector<vector<int>> &adj, bool &alreadyFalse) {
    if (alreadyFalse) return;
    
    vector<int> nbs = adj[u];
    int curColor = UNCOLORED;
    if (UNCOLORED == prevColor || BLUE == prevColor) {
        curColor = RED; 
    } else {
        curColor = BLUE;
    }
    // check feasibility of the current color
    for (auto &v : nbs) {
        if (curColor == colors[v]) {
            //printf("For u:%d & curColor:%d, v:%d is already colored the same.\n", u, curColor, v);
            // No backtracking needed here.
            alreadyFalse = true;
            return;
        }
    }
    colors[u] = curColor;
    for (auto &v : nbs) {
        if (UNCOLORED != colors[v]) continue;
        dfs(curColor, v, adj, alreadyFalse);
    }
}

/*
test cases
4
[[1,2],[1,3],[2,4]]
3
[[1,2],[1,3],[2,3]]
5
[[1,2],[2,3],[3,4],[4,5],[1,5]]
*/
class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislike) {
        vector<vector<int>> adj(n);
        for (auto &vec : dislike) {
            int u = vec[0], v = vec[1];
            --u; --v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        memset(colors, UNCOLORED, sizeof(colors));
        bool alreadyFalse = false;
        for (int u = 0; u < n; ++u) {
            if (UNCOLORED != colors[u]) continue;
            if (alreadyFalse) return false;
            dfs(UNCOLORED, u, adj, alreadyFalse);
        }
        return !alreadyFalse;
    }
};
