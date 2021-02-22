bool const debug = false;
typedef vector<int> VI;
int const INVALID = -1;
int const MAXN = 100000;

int djsFindRoot(int u, VI& djsParent, VI& djsSize) {
    int cur = u;
    int p = djsParent[cur];
    if (INVALID == p) {
        djsParent[u] = u; // lazy init
        djsSize[u] = 1;
        return u;
    }
    while (p != cur) {
        cur = p;
        p = djsFindRoot(cur, djsParent, djsSize); // lazy path compression
    }
    djsParent[u] = p;
    return p;
}

int djsUnite(int u, int v, VI& djsParent, VI& djsSize) {
    int uRoot = djsFindRoot(u, djsParent, djsSize);
    int vRoot = djsFindRoot(v, djsParent, djsSize);
    if (uRoot == vRoot) return uRoot;
    
    // [WARNING] Always attach to smaller root!
    if (uRoot < vRoot) {
        djsSize[uRoot] += djsSize[vRoot];
        return djsParent[vRoot] = uRoot;
    } else {
        djsSize[vRoot] += djsSize[uRoot];
        return djsParent[uRoot] = vRoot;
    }
}

/*
test cases
4
[[3,1,2],[3,2,3],[1,1,3],[1,2,4],[1,1,2],[2,3,4]]
4
[[3,1,2],[3,2,3],[1,1,4],[2,1,4]]
4
[[3,2,3],[1,1,2],[2,3,4]]
*/
class Solution {
public:
    int maxNumEdgesToRemove(int n, vector<vector<int>>& edges) {
        sort(edges.begin(), edges.end(), greater<VI>()); // Always let "type == 3" come first! 
        VI djsParentA(n, INVALID), djsParentB(n, INVALID), djsSizeA(n, 0), djsSizeB(n, 0);
        int addedCnt = 0;
        for (auto &edge : edges) {
            int type = edge[0], u = (--edge[1]), v = (--edge[2]);
            if (debug) printf("Adding edge {type:%d, u:%d, v:%d}, djsSizeA[0]:%d, djsSizeB[0]:%d\n", type, u, v, djsSizeA[0], djsSizeB[0]);
            if (type == 3) {
                int uRootA = djsFindRoot(u, djsParentA, djsSizeA);
                int vRootA = djsFindRoot(v, djsParentA, djsSizeA);
                int uRootB = djsFindRoot(u, djsParentB, djsSizeB);
                int vRootB = djsFindRoot(v, djsParentB, djsSizeB);

                if (uRootA == vRootA && uRootB == vRootB) continue;
                djsUnite(u, v, djsParentA, djsSizeA);
                djsUnite(u, v, djsParentB, djsSizeB);
            } else if (type == 2) {
                int uRootB = djsFindRoot(u, djsParentB, djsSizeB);
                int vRootB = djsFindRoot(v, djsParentB, djsSizeB);

                if (uRootB == vRootB) continue;
                djsUnite(u, v, djsParentB, djsSizeB);                
            } else {
                int uRootA = djsFindRoot(u, djsParentA, djsSizeA);
                int vRootA = djsFindRoot(v, djsParentA, djsSizeA);
                
                if (uRootA == vRootA) continue;
                djsUnite(u, v, djsParentA, djsSizeA);
            }
            if (debug) printf("Added edge {type:%d, u:%d, v:%d}, djsSizeA[0]:%d, djsSizeB[0]:%d\n", type, u, v, djsSizeA[0], djsSizeB[0]);
            ++addedCnt;
        }

        if (djsSizeA[0] < n || djsSizeB[0] < n) return INVALID;
        return edges.size()-addedCnt;
     }
};

