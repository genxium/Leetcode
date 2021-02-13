bool const debug = false;
typedef vector<int> VI;
int const MAXN = 100;
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
* I'm not sure whether there's a trick to quickly "disassemble" a "DisjointSet" by an "Edge", if so it could be a point of optimization.
*
* Here's another problem that involves "disassembling a node from a DisjointSet" https://artofproblemsolving.com/community/c1646h1035738.
*/

/*
test cases
5
[[0,1,1],[1,2,1],[2,3,2],[0,3,2],[0,4,3],[3,4,3],[1,4,6]]
4
[[0,1,1],[1,2,1],[2,3,1],[0,3,1]]
6
[[0,1,1],[1,2,1],[0,2,1],[2,3,4],[3,4,2],[3,5,2],[4,5,2]]
*/

bool edgeCmp(VI const& lhs, VI const& rhs) {
    return lhs[2] < rhs[2]; // less cost comes first
}

class Solution {
public:
    int mstKruskal(int n, vector<VI> &edges, VI &presetEdge, VI &toExcludeEdge) {
        //printf("Starting mstKruskal\n");
        int toRet = 0; // Returns the total cost of the mst constructed.     
        int addedEdgeCount = 0;
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof djsParent);
        
        if (!presetEdge.empty()) {
            int u = presetEdge[0];
            int v = presetEdge[1];
            djsUnite(u, v);
            toRet += presetEdge[2];
            addedEdgeCount = 1;
            //printf("Adding presetEdge (u: %d, v: %d, cost:%d)\n", presetEdge[0], presetEdge[1], presetEdge[2]);
        }
    
        for (auto &minCostEdge : edges) {
            if (!toExcludeEdge.empty()) {
                if (toExcludeEdge[0] == minCostEdge[0] && toExcludeEdge[1] == minCostEdge[1]) continue;
                if (toExcludeEdge[1] == minCostEdge[0] && toExcludeEdge[0] == minCostEdge[1]) continue;
            }
            int u = minCostEdge[0];
            int v = minCostEdge[1];
            int uRootInGrowing = djsFindRoot(u);
            int vRootInGrowing = djsFindRoot(v);
            if (uRootInGrowing == vRootInGrowing) {
                // Already connected.
                continue;
            }
            //printf("Adding edge (u: %d, v: %d, cost:%d)\n", minCostEdge[0], minCostEdge[1], minCostEdge[2]);
            djsUnite(uRootInGrowing, vRootInGrowing);
            toRet += minCostEdge[2];
            addedEdgeCount += 1;
            if (addedEdgeCount == n-1) {
                return toRet;
            }
        }
        return INT_MAX;
    }
        
    vector<vector<int>> findCriticalAndPseudoCriticalEdges(int n, vector<vector<int>>& edges) {
        vector<int> critical;
        vector<int> pseudoCritical;
        int edgeIdx = 0;
        for (auto &edge : edges) {
            edge.push_back(edgeIdx++);
        }
        
        sort(edges.begin(), edges.end(), &edgeCmp);
        VI empty;
        
        int overallMin = mstKruskal(n, edges, empty, empty);
        //printf("overallMin == %d\n\n", overallMin);
        
        for (auto &e : edges) {
            // Exclude (*it) to see if the new spanning tree is of higher cost, if yes, it's critical.
            int removedEdgeCost = mstKruskal(n, edges, empty, e);
            if (debug) printf("removedEdgeCost for edge(u: %d, v: %d, cost:%d) is %d\n\n", e[0], e[1], e[2], removedEdgeCost);

            if (removedEdgeCost > overallMin) {
                // It's critical.
                critical.push_back(e[3]);
            } else {
                // It might be pseudo-critical or redundant, check whether including it compulsorily will result in "overallMin".
                int compulsoryIncludedCost = mstKruskal(n, edges, e, empty);
                if (debug) printf("compulsoryIncludedCost for edge(u: %d, v: %d, cost:%d) is %d\n\n", e[0], e[1], e[2], compulsoryIncludedCost);

                if (compulsoryIncludedCost == overallMin) {
                    pseudoCritical.push_back(e[3]);
                }
            }
        }
        
        return {critical, pseudoCritical};
    }
};

