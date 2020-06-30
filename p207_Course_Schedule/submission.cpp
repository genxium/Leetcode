#define MAXN 100000
#define INVALID -1

int indexes[MAXN];
int lowlinks[MAXN];
bool onStack[MAXN];

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
    
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        bool ans = true;
        
        memset(indexes, INVALID, sizeof(indexes));
        memset(lowlinks, INVALID, sizeof(lowlinks));
        memset(onStack, false, sizeof(onStack));
        
        unordered_map<int, vector<int>> adj;
        for (vector<int> &connection : prerequisites) {
            int u = connection[0];
            int v = connection[1];
            
            if (adj.find(u) == adj.end()) {
                adj[u] = vector<int>{v};
            } else {
                adj[u].push_back(v);
            }
        }
        
        stack<int> globalStack;
        vector<vector<int>> sccOut;
        int dfsIndex = 0;
        
        for (int u = 0; u < numCourses; ++u) {
            if (INVALID == indexes[u]) {
                strongconnect(u, dfsIndex, adj, globalStack, sccOut);
            }   
        }
        /*
        printf("Resulted sccOut:\n");
        for (vector<int> &l : sccOut) {
            for (int &v : l) {
                printf("%d ", v);
            }
            printf("\n");
        }
        */
        return (sccOut.size() == numCourses);
    }
};
