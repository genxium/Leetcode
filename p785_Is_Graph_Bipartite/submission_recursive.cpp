#define RED +1
#define BLUE -1
#define UNCOLORED 0
#define MAXN 200
int colors[MAXN];

class Solution {
public:
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

    bool isBipartite(vector<vector<int>>& adj) {
        /*
        test case #1
        [[4],[],[4],[4],[0,2,3]]
        */
        int n = adj.size();
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
