int const DISJOINT_SET_NOT_FOUND = -1;
int const MAXN = 200;
int djsParent[MAXN]; // "DisjointSetParent"
bool const debug = false;

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
[[1,1,0],[1,1,0],[0,0,1]]
[[1,1,0],[1,1,1],[0,1,1]]
[[1,0,0,1],[0,1,1,0],[0,1,1,1],[1,0,1,1]]
*/
class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        int yUpper = M.size();
        int xUpper = M[0].size();
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof djsParent);
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                if (M[y][x] != 1) continue;
                if (debug) printf("y:%d, x:%d are friends\n", y, x);
                int yy = y, xx= x;
                if (yy > xx) swap(yy, xx);
                djsUnite(yy, xx);
            }
        }
        
        unordered_set<int> counter;
        for (int y = 0; y < yUpper; ++y) {
            counter.insert(djsFindRoot(y));
        }
        return counter.size();
    }
};
