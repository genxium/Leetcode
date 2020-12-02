typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ_LIST;
int const DISJOINT_SET_NOT_FOUND = -1;
int const MAXN = 10000;
bool openSet[MAXN+1], closedSet[MAXN+1];
bool const debug = false;

class DisjointSet {
private:
    unordered_map<int, int> disjointSetParent;
public:
    int makeNewSubset(int u) {
        disjointSetParent[u] = u;
        return u;
    }
    
    int findRoot(int u) {
        auto parentItr = disjointSetParent.find(u);
        if (parentItr == disjointSetParent.end()) {
            return DISJOINT_SET_NOT_FOUND;
        }
        int parent = (*parentItr).second;
        if (parent == u) {
            return u;
        }
        int root = findRoot(parent); // lazy path compression
        disjointSetParent[u] = root;
        return root;
    }
    
    int unite(int u, int v) {
        int uRoot = findRoot(u);
        int vRoot = findRoot(v);
        if (uRoot == vRoot) {
            return uRoot;
        }
        disjointSetParent[uRoot] = vRoot;
        return vRoot;
    }
};

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
    bool tryToConnect(int x, int y, ADJ_LIST &adj, DisjointSet &connectedSet) {
        // 1. Check whether there's a connected shortcut in the DisjointSet.
        if (debug) printf("Finding roots for x:%d, y:%d\n", x, y);
        int xRoot = connectedSet.findRoot(x);
        int yRoot = connectedSet.findRoot(y);
        if (debug) printf("\tfound xRoot:%d, yRoot:%d\n", xRoot, yRoot);
        if (xRoot == yRoot && xRoot != DISJOINT_SET_NOT_FOUND) {
            return true;
        }
        if (xRoot == DISJOINT_SET_NOT_FOUND) {
            xRoot = connectedSet.makeNewSubset(x);
        }
        if (yRoot == DISJOINT_SET_NOT_FOUND) {
            yRoot = connectedSet.makeNewSubset(y);
        }
        // 2. Check connectivity in ADJ_LIST.
        if (debug) printf("\tcheck ADJ_LIST\n");
        memset(openSet, false, sizeof(openSet));
        memset(closedSet, false, sizeof(closedSet));
        queue<int> q;
        q.push(x); 
        openSet[x] = true;
        if (xRoot != DISJOINT_SET_NOT_FOUND) {
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
            if (debug) printf("\tnot found");
            return false;
        }
        if (debug) printf("\tfound\n");
        // Now that "true == found".
        
        if (xRoot > yRoot) {
            connectedSet.unite(xRoot, yRoot);   
        } else {
            connectedSet.unite(yRoot, xRoot);
        }

        return true;
    }
    
    vector<bool> areConnected(int n, int threshold, vector<vector<int>>& queries) {
        ADJ_LIST adj;
        for (int z = threshold+1; z <= n; ++z) {
            for (int k = 2; k*z <= n; ++k) {
                adj[z].insert(k*z);
                adj[k*z].insert(z);
            }
        }
        
        vector<bool> ans;
        if (debug) {
            printf("There're %d queries\n", queries.size());
        }
        DisjointSet connectedSet;
        int i = 0; 
        for (int i = 0; i < queries.size(); ++i) {
            auto &query = queries[i];
            int x = query[0], y = query[1];
            bool res = tryToConnect(x, y, adj, connectedSet);
            ans.push_back(res);
        }
        
        return ans;
    }
};
