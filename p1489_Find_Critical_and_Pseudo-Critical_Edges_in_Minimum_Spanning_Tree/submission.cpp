#define DISJOINT_SET_NOT_FOUND -1

class Edge {
public:
    int id, u, v, cost;
};

class EdgeCompareForVec {
public:
    int operator() (Edge const &left, Edge const &right) {
        return left.cost < right.cost;
    }
};

class DisjointSet {
/*
* I'm not sure whether there's a trick to quickly "disassemble" a "DisjointSet" by an "Edge", if so it could be a point of optimization.
*
* Here's another problem that involves "disassembling a node from a subset" https://artofproblemsolving.com/community/c1646h1035738.
*/
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


class Solution {
public:
    int mstKruskal(int n, vector<Edge> &edges, Edge *presetEdge, Edge *toExcludeEdge) {
        //printf("Starting mstKruskal\n");
        int toRet = 0; // Returns the total cost of the mst constructed.     
        int addedEdgeCount = 0;
        DisjointSet growingTreeVertices;
        if (NULL != presetEdge) {
            int u = presetEdge->u;
            int v = presetEdge->v;
            int uRootInGrowing = growingTreeVertices.makeNewSubset(u);
            int vRootInGrowing = growingTreeVertices.makeNewSubset(v);
            growingTreeVertices.unite(uRootInGrowing, vRootInGrowing);
            toRet += presetEdge->cost;
            addedEdgeCount = 1;
            //printf("Adding presetEdge (id: %d, u: %d, v: %d, cost:%d)\n", presetEdge->id, presetEdge->u, presetEdge->v, presetEdge->cost);

        }
    
        int edgeIdx = 0, m = edges.size();
        while (edgeIdx < m) {
            Edge minCostEdge = edges[edgeIdx++];
            if (NULL != presetEdge) {
                if (presetEdge->id == minCostEdge.id) continue;
            }
            if (NULL != toExcludeEdge) {
                if (toExcludeEdge->id == minCostEdge.id) continue;
            }
            int u = minCostEdge.u;
            int v = minCostEdge.v;
            int uRootInGrowing = growingTreeVertices.findRoot(u);
            int vRootInGrowing = growingTreeVertices.findRoot(v);
            if (uRootInGrowing == vRootInGrowing && uRootInGrowing != DISJOINT_SET_NOT_FOUND) {
                continue;
            }
            //printf("Adding edge (id: %d, u: %d, v: %d, cost:%d)\n", minCostEdge.id, minCostEdge.u, minCostEdge.v, minCostEdge.cost);
            if (uRootInGrowing == DISJOINT_SET_NOT_FOUND) {
                uRootInGrowing = growingTreeVertices.makeNewSubset(u);
            }
            if (vRootInGrowing == DISJOINT_SET_NOT_FOUND) {
                vRootInGrowing = growingTreeVertices.makeNewSubset(v);
            }
            growingTreeVertices.unite(uRootInGrowing, vRootInGrowing);
            toRet += minCostEdge.cost;
            addedEdgeCount += 1;
            if (addedEdgeCount == n-1) {
                return toRet;
            }
        }
        return INT_MAX;
    }
        
    vector<vector<int>> findCriticalAndPseudoCriticalEdges(int n, vector<vector<int>>& edges) {
        /*
        * test cse
        6
        [[0,1,1],[1,2,1],[0,2,1],[2,3,4],[3,4,2],[3,5,2],[4,5,2]]
        */
        vector<int> critical;
        vector<int> pseudoCritical;
        
        vector<Edge> rephrasedEdges;
        for (int edgeId = 0; edgeId < edges.size(); ++edgeId) {
            Edge e;
            e.id = edgeId;
            e.u = edges[edgeId][0];
            e.v = edges[edgeId][1];
            e.cost = edges[edgeId][2];
            rephrasedEdges.push_back(e);
        }
        sort(rephrasedEdges.begin(), rephrasedEdges.end(), EdgeCompareForVec());
        
        int overallMin = mstKruskal(n, rephrasedEdges, NULL, NULL);
        //printf("overallMin == %d\n\n", overallMin);
        
        for (auto it = rephrasedEdges.rbegin(); it != rephrasedEdges.rend(); ++it) {
            // Exclude (*it) to see if the new spanning tree is of higher cost, if yes, it's critical.
            Edge e = *it;
            int removedEdgeCost = mstKruskal(n, rephrasedEdges, NULL, &e);
            //printf("removedEdgeCost for edge(id: %d, u: %d, v: %d, cost:%d) is %d\n\n", e.id, e.u, e.v, e.cost, removedEdgeCost);

            if (removedEdgeCost > overallMin) {
                // It's critical.
                critical.push_back(e.id);
            } else {
                // It might be pseudo-critical or redundant, check whether including it compulsorily will result in "overallMin".
                int compulsoryIncludedCost = mstKruskal(n, rephrasedEdges, &e, NULL);
                //printf("compulsoryIncludedCost for edge(id: %d, u: %d, v: %d, cost:%d) is %d\n\n", e.id, e.u, e.v, e.cost, compulsoryIncludedCost);

                if (compulsoryIncludedCost == overallMin) {
                    pseudoCritical.push_back(e.id);
                }
            }
        }
        
        return {critical, pseudoCritical};
    }
};
