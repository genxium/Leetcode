typedef vector<int> VI;
typedef unordered_set<int> SI;
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
[[0,0],[0,1],[1,0],[1,2],[2,1],[2,2]]
[[0,0],[0,2],[1,1],[2,0],[2,2]]
[[0,0]]
[[11,16],[29,28],[28,4],[13,13],[9,22],[28,22],[22,19],[13,17],[8,19],[29,7],[3,28],[25,1],[11,4],[3,17],[21,13],[5,8],[16,24],[29,18],[5,5],[11,5],[27,25],[18,27],[19,14],[29,22],[2,2],[24,20],[25,27],[16,18],[22,21],[21,19],[12,21],[15,8],[19,17],[13,2],[9,3],[3,25],[18,17],[2,10],[11,26],[1,19],[28,12],[16,26],[11,15],[22,11],[26,13],[0,12],[0,16],[1,25],[21,25],[6,9],[4,1],[16,5],[8,12],[17,14],[10,10],[0,13],[7,4],[3,18],[10,14],[15,9],[14,3],[9,5],[20,15],[23,25],[29,5]]
*/
class Solution {
public:
    int removeStones(vector<vector<int>>& stones) {
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof djsParent);
        // Group row/col-connected stones, the maximum removal will result in only the number of groups of stones remained, i.e. only 1 stone of each group is remained.
        int n = stones.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i+1; j < stones.size(); ++j) {
                if (stones[i][0] != stones[j][0] && stones[i][1] != stones[j][1]) continue;
                djsUnite(i, j);
            }
        }
        unordered_set<int> roots;
        for (int i = 0; i < n; ++i) {
            int iroot = djsFindRoot(i);
            if (debug) {
                printf("i:%d -> %d\n", i, iroot);   
            }
            roots.insert(iroot);
        }
        return n-roots.size();
    }
};
