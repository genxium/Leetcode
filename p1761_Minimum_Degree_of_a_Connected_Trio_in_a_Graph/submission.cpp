int const MAXN = 400;
int degrees[MAXN+1];
int isAdj[MAXN+1][MAXN+1]; // isAdjacent

class Solution {
public:
    int minTrioDegree(int n, vector<vector<int>>& edges) {
        memset(isAdj, false, sizeof isAdj);
        memset(degrees, 0, sizeof degrees);

        for (auto &edge : edges) {
            int u = edge[0], v = edge[1];
            ++degrees[u]; ++degrees[v];
            isAdj[u][v] = isAdj[v][u] = true;
        }    
        
        int ans = INT_MAX;
        
        for (int i = 1; i <= n; ++i) {
            for (int j = i+1; j <= n; ++j) {
                if (!isAdj[i][j]) continue;
                for (int k = j+1; k <= n; ++k) {
                    if (!isAdj[i][k] || !isAdj[j][k]) continue;
                    ans = min(ans, degrees[i]+degrees[j]+degrees[k]);
                }
            }
        }
        
        return (ans == INT_MAX ? -1 : ans-6);
    }
};
