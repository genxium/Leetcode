bool debug = false;
typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ;
int const MAXN = 10000;
int const INVALID = -1;
int allChildrenPathSum[MAXN], subtreeNodeCnt[MAXN];
bool visited[MAXN];

class Solution {
public:
    void dfs1(int u, ADJ& adj) {
        visited[u] = true;
        allChildrenPathSum[u] = 0;
        subtreeNodeCnt[u] = 1; // itself
        for (auto &v : adj[u]) {
            if (visited[v]) continue;
            dfs1(v, adj);
            allChildrenPathSum[u] += allChildrenPathSum[v] + subtreeNodeCnt[v];
            subtreeNodeCnt[u] += subtreeNodeCnt[v];
        }
        if (debug) printf("allChildrenPathSum[u:%d]: %d, subtreeNodeCnt[u:%d]: %d\n", u, allChildrenPathSum[u], u, subtreeNodeCnt[u]);
    }
    
    void dfs2(int u, int v, ADJ& adj, vector<int> &ans) {
        visited[v] = true;
        if (INVALID == u) {
            // The chosen root
            ans[v] = allChildrenPathSum[v];
        } else {
            ans[v] = allChildrenPathSum[v] // to children of "v"
                + (ans.size() - subtreeNodeCnt[v]) // counting edge (v, u) this many times for going to others via "u" from "v"
                + (ans[u]-(allChildrenPathSum[v]+subtreeNodeCnt[v])); // to others via "u" from "v"
        }
        
        for (auto &w : adj[v]) {
            if (visited[w]) continue;
            dfs2(v, w, adj, ans);
        }
    }
    
    vector<int> sumOfDistancesInTree(int N, vector<vector<int>>& edges) {
        memset(visited, false, sizeof visited);
        memset(allChildrenPathSum, INVALID, sizeof allChildrenPathSum);
        memset(subtreeNodeCnt, INVALID, sizeof subtreeNodeCnt);
        /*
            First find every "allChildrenPathSum(u)" and "subtreeNodeCnt(u)" where the latter includes "u" itself

            In general for u -> v0, v1, v2, ...

            1. allChildrenPathSum(u) = SUM{allChildrenPathSum(v[i]) + subtreeNodeCnt(v[i])}
            2. subtreeNodeCnt(u) = SUM{subtreeNodeCnt(v[i])} 
        */    
        ADJ adj;
        for (auto &edge : edges) {
            int u = edge[0], v = edge[1];
            adj[u].insert(v);
            adj[v].insert(u);
        }
        
        dfs1(0, adj); // Just take 0 as root as an arbitrary choice
        vector<int> ans(N, 0);
        
        memset(visited, false, sizeof visited);
        dfs2(INVALID, 0, adj, ans);
        
        return ans;
    }
};
