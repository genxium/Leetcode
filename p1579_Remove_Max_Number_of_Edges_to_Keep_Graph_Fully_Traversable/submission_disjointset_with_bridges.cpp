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

void findBridges(int dfsDirectPredecessor, int cur, int &dfsIndex, unordered_map<int, vector<VI>> &adj, VI &indexes, VI &lowIndirectLinks, unordered_set<int> &bridgesOut) {
    indexes[cur] = dfsIndex++;
    lowIndirectLinks[cur] = indexes[cur];
    int iUpper = adj[cur].size();
    for (int i = 0; i < iUpper; ++i) {
        VI &nb = adj[cur][i];
        int nbNodeId = nb[1], edgeIdx = nb[2];
        if (INVALID == indexes[nbNodeId]) {
            findBridges(cur, nbNodeId, dfsIndex, adj, indexes, lowIndirectLinks, bridgesOut);
            lowIndirectLinks[cur] = min(lowIndirectLinks[cur], lowIndirectLinks[nbNodeId]);
            if (lowIndirectLinks[nbNodeId] == indexes[nbNodeId]) {
                bridgesOut.insert(edgeIdx);
            }
        } else {
            if (INVALID != dfsDirectPredecessor && nbNodeId != dfsDirectPredecessor) {
                lowIndirectLinks[cur] = (lowIndirectLinks[cur] < indexes[nbNodeId] ? lowIndirectLinks[cur] : indexes[nbNodeId]);
            }
        }
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
        unordered_map<int, vector<VI>> adjA, adjB;
        int edgeIdx = 0;
        for (auto &edge : edges) {
            int type = edge[0], u = (--edge[1]), v = (--edge[2]); edge.push_back(edgeIdx);
            if (type == 3) {
                adjA[u].push_back({type, v, edgeIdx});
                adjA[v].push_back({type, u, edgeIdx});
                adjB[u].push_back({type, v, edgeIdx});
                adjB[v].push_back({type, u, edgeIdx});
            } else if (type == 2) {
                adjB[u].push_back({type, v, edgeIdx});
                adjB[v].push_back({type, u, edgeIdx});
            } else {
                // type == 1
                adjA[u].push_back({type, v, edgeIdx});
                adjA[v].push_back({type, u, edgeIdx});
            }
            ++edgeIdx;
        }
        if (debug) printf("Built adjA.size():%d, adjB.size():%d\n", adjA.size(), adjB.size());

        VI dfsIndexesA(n, INVALID), dfsIndexesB(n, INVALID);
        VI lowIndirectLinksA(n, INVALID), lowIndirectLinksB(n, INVALID);
        unordered_set<int> bridgesOutA, bridgesOutB;

        int dfsIndexA = 0; 
        for (int i = 0; i < n; ++i) {
            if (INVALID != lowIndirectLinksA[i]) continue;
            findBridges(INVALID, i, dfsIndexA, adjA, dfsIndexesA, lowIndirectLinksA, bridgesOutA);
        }
        if (debug) printf("Built bridgesOutA.size():%d\n", bridgesOutA.size());
        
        int dfsIndexB = 0; 
        for (int i = 0; i < n; ++i) {
            if (INVALID != lowIndirectLinksB[i]) continue;
            findBridges(INVALID, i, dfsIndexB, adjB, dfsIndexesB, lowIndirectLinksB, bridgesOutB);
        }
        if (debug) printf("Built bridgesOutB.size():%d\n", bridgesOutB.size());
        
        // Kruscal's algorithm for MinimumSpanningTree using bridges first
        VI djsParentA(n, INVALID), djsParentB(n, INVALID), djsSizeA(n, 0), djsSizeB(n, 0);
        for (auto bridgeId : bridgesOutA) {
            auto &edge = edges[bridgeId];
            if (debug) printf("\tAdding bridge {type:%d, u:%d, v:%d, edgeIdx:%d} for A\n", edge[0], edge[1], edge[2], bridgeId);
            djsUnite(edge[1], edge[2], djsParentA, djsSizeA);
            if (edge[0] == 3) {
                djsUnite(edge[1], edge[2], djsParentB, djsSizeB);
            }
        }
        if (debug) printf("Added all of bridgesOutA\n");

        for (auto bridgeId : bridgesOutB) {
            auto &edge = edges[bridgeId];
            if (debug) printf("\tAdding bridge {type:%d, u:%d, v:%d, edgeIdx:%d} for B\n", edge[0], edge[1], edge[2], bridgeId);
            djsUnite(edge[1], edge[2], djsParentB, djsSizeB);
            if (edge[0] == 3) {
                djsUnite(edge[1], edge[2], djsParentA, djsSizeA);
            }
        }
        if (debug) printf("Added all of bridgesOutB\n");

        unordered_set<int> allBridges;
        for (auto bridgeId : bridgesOutA) allBridges.insert(bridgeId);
        for (auto bridgeId : bridgesOutB) allBridges.insert(bridgeId);

        int extraAddedCnt = 0;
        for (int edgeIdx = 0; edgeIdx < edges.size(); ++edgeIdx) {
            // Note that "type == 3" still comes first.
            auto &edge = edges[edgeIdx];
            int type = edge[0], u = edge[1], v = edge[2];
            // if (debug) printf("Adding edge {type:%d, u:%d, v:%d, edgeIdx:%d}, djsSizeA[0]:%d, djsSizeB[0]:%d\n", type, u, v, edgeIdx, djsSizeA[0], djsSizeB[0]);
            if (allBridges.count(edgeIdx)) continue;
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
            if (debug) printf("Added edge {type:%d, u:%d, v:%d, edgeIdx:%d}, djsSizeA[0]:%d, djsSizeB[0]:%d\n", type, u, v, edgeIdx, djsSizeA[0], djsSizeB[0]);
            ++extraAddedCnt;
        }

        if (djsSizeA[0] < n || djsSizeB[0] < n) return INVALID;
        return edges.size()-(allBridges.size() + extraAddedCnt);
     }
};
