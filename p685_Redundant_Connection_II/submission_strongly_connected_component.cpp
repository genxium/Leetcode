bool const debug = false;

int const MAXN = 1000;
int const INVALID = -1;

int indexes[MAXN+1];
int lowlinks[MAXN+1];
bool onStack[MAXN+1];

/*
test cases
[[1,2],[1,3],[2,3]]
[[1,2],[2,3],[3,4],[4,1],[1,5]]
[[1,2],[2,3],[3,4],[4,1],[5,1]]
[[5,2],[5,1],[3,1],[3,4],[3,5]]
*/
class Solution {
public:
    void strongconnect(int u, int &dfsIndex, unordered_map<int, vector<int>> &adj, stack<int> &globalStack, vector<vector<int>> &sccOut) {
        indexes[u] = dfsIndex++;
        lowlinks[u] = indexes[u];
        onStack[u] = true;
        globalStack.push(u);
        int iUpper = adj[u].size();
        for (int i = 0; i < iUpper; ++i) {
            int nb = adj[u][i];
            if (INVALID == indexes[nb]) {
                strongconnect(nb, dfsIndex, adj, globalStack, sccOut);
                lowlinks[u] = (lowlinks[u] < lowlinks[nb] ? lowlinks[u] : lowlinks[nb]);
            } else {
                // INVALID != indexes[nb]
                if (onStack[nb]) {
                    lowlinks[u] = (lowlinks[u] < indexes[nb] ? lowlinks[u] : indexes[nb]);
                } else {
                    // Already discovered but not currently on stack, implying an already popped SCC.
                    // Deliberately not doing anything here.
                }
            }
        }
        
        if (lowlinks[u] == indexes[u]) {
            vector<int> newScc;
            while (false == globalStack.empty()) {
                int v = globalStack.top();
                globalStack.pop();
                onStack[v] = false;
                newScc.push_back(v);
                if (v == u) break;
            }
            sccOut.push_back(newScc);
        }
    }
    
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        
        memset(indexes, INVALID, sizeof(indexes));
        memset(lowlinks, INVALID, sizeof(lowlinks));
        memset(onStack, false, sizeof(onStack));
        
        unordered_map<int, int> inDegree;
        unordered_map<int, int> intoNodeIndices; // Marks each edge index by each "into-node" 

        int intoNodeIdx = 0;
        unordered_map<int, vector<int>> adj;
        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            ++inDegree[v];
            intoNodeIndices[v] = intoNodeIdx++;
            adj[u].push_back(v);
        }
        
        stack<int> globalStack;
        vector<vector<int>> sccOut;
        int dfsIndex = 0;
        
        for (int u = 0; u < n; ++u) {
            if (INVALID == indexes[u]) {
                strongconnect(u, dfsIndex, adj, globalStack, sccOut);
            }   
        }
        
        for (auto &scc : sccOut) {
            if (1 == scc.size()) continue;
            int iWithLargestIntoNodeIdx = INVALID;
            int largestIntoNodeIdx = INT_MIN;
            
            // The redundant edge should be within the scc of size larger than 1.
            for (int i = 0; i < scc.size(); ++i) {
                int u = scc[i];
                if (inDegree[u] > 1) {
                    // In this case `u` is the "entrance of this scc", making it only possible to delete "the edge which is within this scc and points into `u`", i.e. deleting any other edge wouldn't make a valid tree. 
                    if (debug) {
                        printf("#1 returning at u:%d within an scc whose size > 1.\n", u);
                    }
                    return {scc[(i+1)%scc.size()], u};
                } 
                if (intoNodeIndices[u] > largestIntoNodeIdx) {
                    iWithLargestIntoNodeIdx = i;
                    largestIntoNodeIdx = intoNodeIndices[u];
                }
            }
            
            // In this case there exists a `u` where "adj[u].size() > " being the "exit of this scc", thus deleting "any edge which is within this scc" will work.
            if (debug) printf("#2 returning at u:%d within an scc whose size > 1.\n", scc[iWithLargestIntoNodeIdx]);
            return {scc[(iWithLargestIntoNodeIdx+1)%scc.size()], scc[iWithLargestIntoNodeIdx]};
        }
        
        // No scc with size larger than 1.
        for (auto &[u, inDeg] : inDegree) {
            if (inDegree[u] > 1) {
                if (debug) {
                    printf("#3 Returning at u:%d.\n", u);
                }
                return edges[intoNodeIndices[u]];
            }
        }
        
        if (debug) printf("#4 Returning last edge.\n");
        return edges.back();
    }
};
