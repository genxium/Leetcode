bool const debug = false;
int const MAXN = 1000;
int const DISJOINT_SET_NOT_FOUND = -1;
int djsParent[MAXN+1]; // "DisjointSetParent"

int djsFindRoot(int u) {
    int cur = u;
    int p = djsParent[cur];
    if (DISJOINT_SET_NOT_FOUND == p) {
        djsParent[u] = u; // lazy init
        return u;
    }
    while (p != cur) {
        cur = p;
        p = djsFindRoot(cur); // lazy path compression
    }
    djsParent[u] = p;
    return p;
}
    
int djsUnite(int u, int v) {
    int uRoot = djsFindRoot(u);
    int vRoot = djsFindRoot(v);
    // Either "uRoot > vRoot" or "uRoot < vRoot" doesn't matter.
    return (djsParent[vRoot] = uRoot);
}

/*
test cases
[[1,2],[1,3],[2,3]]
[[1,2], [2,3], [3,4], [1,4], [1,5]]
*/
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof djsParent);
        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int uRoot = djsFindRoot(u);
            int vRoot = djsFindRoot(v);
            if (uRoot == vRoot) return {u, v};
            djsUnite(u, v);
        }
        return {};
    }
};
