typedef unordered_set<int> ADJ_ELE;
typedef vector<ADJ_ELE> ADJ_LIST;
int const DISJOINT_SET_NOT_FOUND = -1;
int const MAXN = 10000;
int djsParent[MAXN+1]; // "DisjointSetParent"
bool openSet[MAXN+1], closedSet[MAXN+1];
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
6
2
[[1,4],[2,5],[3,6]]
6
0
[[4,5],[3,4],[3,2],[2,6],[1,3]]
5
1
[[4,5],[4,5],[3,2],[2,3],[3,4]]
*/
class Solution {
public:
    bool tryToConnect(int x, int y, ADJ_LIST &adj) {
        // 1. Check whether there's a connected shortcut in "djsParent[...]".
        if (debug) printf("Finding roots for x:%d, y:%d\n", x, y);
        int xRoot = djsFindRoot(x);
        int yRoot = djsFindRoot(y);
        if (debug) printf("\tfound xRoot:%d, yRoot:%d\n", xRoot, yRoot);
        
        return xRoot == yRoot; // The "critical trick".
        
        ////////////////////////////////////////////////////////////////////////////////////////////
        /*
        [WARNING]
        
        Unnecessary codes below!
        */
        ////////////////////////////////////////////////////////////////////////////////////////////
        if (xRoot == yRoot) {
            return true;
        }
        
        // 2. Check connectivity in ADJ_LIST.
        if (debug) printf("\tcheck ADJ_LIST\n");
        memset(openSet, false, sizeof(openSet));
        memset(closedSet, false, sizeof(closedSet));
        queue<int> q;
        q.push(x); 
        openSet[x] = true;
        if (xRoot != x) {
            q.push(xRoot); 
            openSet[xRoot] = true;
        }
        bool found = false;
        while (!q.empty() && false == found) {
            auto expander = q.front(); q.pop();
            closedSet[expander] = true;
            openSet[expander] = false;
            
            ADJ_ELE& nbs = adj[expander];
            for (auto &nb : nbs) {
                if (openSet[nb] || closedSet[nb]) continue;
                if (nb == y 
                    ||
                    nb == yRoot
                   ) {
                    found = true;
                    break;
                }
                q.push(nb);
                openSet[nb] = true;
            }
        }
        
        if (!found) {
            if (debug) printf("\tnot found\n");
            return false;
        }
        if (debug) printf("\tfound\n");
        
        // Now that "true == found".
        djsUnite(x, y);

        return true;
    }
    
    vector<bool> areConnected(int n, int threshold, vector<vector<int>>& queries) {
        // init
        ADJ_LIST adj(n+1);
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof(djsParent));
        
        for (int z = threshold+1; z <= n; ++z) {
            for (int kz = 2*z; kz <= n; kz += z) {
                adj[z].insert(kz);
                adj[kz].insert(z);
                
                /*
                [WARNING]
                
                There's seemingly no magic here to further reduce time-complexity by just uniting "z" and "k*z" in a peculiar order, e.g. consider "z1: 7, k11*z1: 77, k12*z1: 140" and "z2: 11, k21*z2: 77, k22*z2: 121" where "threshold: 6", none of
                - "djsRoot(77) == 7", or
                - "djsRoot(77) == 11", or
                - "djsRoot(77) == 140", or
                - "djsRoot(77) == 121"
                would have obvious advantage over others.
                */
                djsUnite(kz, z);
            }
        }
        
        vector<bool> ans;
        if (debug) {
            printf("There're %zu queries\n", queries.size());
        }
        int i = 0;
        for (int i = 0; i < queries.size(); ++i) {
            auto &query = queries[i];
            int x = query[0], y = query[1];
            bool res = tryToConnect(x, y, adj);
            ans.push_back(res);
        }
        
        return ans;
    }
};
