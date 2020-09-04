/*
My favourite solution for this problem. It shows some insight about "cycle-detection in UndirectedGraph by bridge-finding", see https://www.yinxiang.com/everhub/note/bb8b112f-e478-4bd5-bcf6-b86e0431584c for some theoretical prerequisites.
*/
#define INVALID -1
#define MAXY 500
#define MAXX 500
#define MAXN 250000 // MAXY*MAXX

int indexes[MAXN];
int lowIndirectLinks[MAXN];
vector<int> diry{0, 0, +1, -1};
vector<int> dirx{+1, -1, 0, 0};

/*
test case #1
[["d","d","a"],["d","d","c"],["d","c","c"],["d","d","c"],["d","a","b"]]

test case #2
[["c","a","a"],["d","a","d"],["d","a","c"],["d","c","d"],["a","a","c"]]

test case #3
[["b","c","a","e"],["e","c","c","a"],["c","c","a","e"]]

test case #4
[["g","d","f","a","d","a","c","c","d","a","b","e","f","g","c"],["b","d","a","a","f","b","b","a","a","e","d","b","g","d","b"],["b","c","d","g","g","c","g","a","b","e","g","g","b","a","b"],["c","b","g","g","f","c","a","g","e","g","e","g","g","c","b"],["e","g","f","g","f","d","c","c","g","g","e","d","c","a","b"],["c","f","a","e","d","e","f","c","d","d","f","a","e","e","e"],["g","d","a","e","d","c","g","d","a","f","a","b","c","a","e"],["g","g","b","g","e","f","d","g","d","f","d","e","c","c","a"],["e","d","a","a","f","b","a","a","d","c","d","c","c","a","e"]]

test case #5
[["c","a","d"],["a","a","a"],["a","a","d"],["a","c","d"],["a","b","c"]]
*/

class Solution {
private:
    pair<int, int> toCoor(int u, int xUpper) {
        return {u/xUpper, u%xUpper};
    }    
    
    void printAdjList(unordered_map<int, vector<int>>& adj, int xUpper) {
        for (auto &it : adj) {
            int u = it.first;
            auto coor = toCoor(u, xUpper);
            printf("adj[{%d, %d}]: ", coor.first, coor.second);
            auto &nbs = adj[u];
            for (auto &nb : nbs) {
                auto nbCoor = toCoor(nb, xUpper);
                printf("{%d, %d} ", nbCoor.first, nbCoor.second);
            }
            printf("\n");
        }
    }
public:   
    void dfs(int dfsDirectPredecessor, int cur, int &dfsIndex, unordered_map<int, vector<int>> &adj, bool &containsSize4Cycle, int &directNonBridgeClusterSizeForCur, int &nonBridgeNeighbourCount) {
        if (containsSize4Cycle) return;
        
        indexes[cur] = dfsIndex++;
        lowIndirectLinks[cur] = indexes[cur];
        int iUpper = adj[cur].size();
        
        // note that when "INVALID != dfsDirectPredecessor", {dfsDirectPredecessor, cur} may or may not be a bridge, yet in either case it doesn't count in "nonBridgeNeighbourCount"
        nonBridgeNeighbourCount = (INVALID == dfsDirectPredecessor ? iUpper : iUpper-1);
        
        int toReduceClusterSizeFromDirectBridges = 0;
        for (int i = 0; i < iUpper; ++i) {
            int nb = adj[cur][i];
            if (INVALID == indexes[nb]) {
                int directNonBridgeClusterSizeForNb = 0;
                int nonBridgeNeighbourCountForNb = 0;
                dfs(cur, nb, dfsIndex, adj, containsSize4Cycle, directNonBridgeClusterSizeForNb, nonBridgeNeighbourCountForNb);
                lowIndirectLinks[cur] = (lowIndirectLinks[cur] < lowIndirectLinks[nb] ? lowIndirectLinks[cur] : lowIndirectLinks[nb]);
                if (lowIndirectLinks[nb] == indexes[nb]) {
                    --nonBridgeNeighbourCount;
                    // {cur, nb} is a bridge, note that by reaching here, "nb" couldn't be dfsDirectPredecessor
                    if (directNonBridgeClusterSizeForNb >= 4 && nonBridgeNeighbourCountForNb >= 2) {
                        containsSize4Cycle = true;
                        return;
                    }
                    toReduceClusterSizeFromDirectBridges += directNonBridgeClusterSizeForNb;
                }
            } else {
                if (INVALID != dfsDirectPredecessor && nb != dfsDirectPredecessor) {
                    lowIndirectLinks[cur] = (lowIndirectLinks[cur] < indexes[nb] ? lowIndirectLinks[cur] : indexes[nb]);
                }   
            }
        }
        
        directNonBridgeClusterSizeForCur = (dfsIndex-indexes[cur]-toReduceClusterSizeFromDirectBridges);
    }
    
    bool containsCycle(vector<vector<char>>& grid) {
        memset(indexes, INVALID, sizeof(indexes));
        memset(lowIndirectLinks, INVALID, sizeof(lowIndirectLinks));
        unordered_map<int, vector<int>> adj;
        
        int yUpper = grid.size(), xUpper = grid[0].size();
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                int u = y*xUpper+x;
                char ref = grid[y][x];
                if (adj.find(u) == adj.end()) {
                    adj[u] = {}; 
                }
                for (int k = 0; k < 4; ++k) {
                    int ny = y+diry[k], nx = x+dirx[k]; 
                    if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                    if (grid[ny][nx] != ref) continue;
                    int v = ny*xUpper+nx;        
                    adj[u].push_back(v);
                }
            } 
        }
        //printAdjList(adj, xUpper);
        
        bool ans = false;
        int dfsIndex = 0;
        int idxUpper = yUpper*xUpper;
        for (int u = 0; u < idxUpper; ++u) {
            if (INVALID != indexes[u]) {
                continue;
            }    
            int directNonBridgeClusterSizeForCur = 0;
            int nonBridgeNeighbourCount = 0;
            dfs(INVALID, u, dfsIndex, adj, ans, directNonBridgeClusterSizeForCur, nonBridgeNeighbourCount);
            //auto coor = toCoor(u, xUpper);
            //printf("After dfs, indexes[{%d, %d}] == %d, dfsIndex == %d, ans == %d, directNonBridgeClusterSizeForCur == %d, nonBridgeNeighbourCount == %d\n", coor.first, coor.second, indexes[u], dfsIndex, ans, directNonBridgeClusterSizeForCur, nonBridgeNeighbourCount);
            if (ans) {
                return true;
            }
            // false == ans
            if (directNonBridgeClusterSizeForCur >= 4 && nonBridgeNeighbourCount >= 2) {
                ans = true;
                return ans;
            }    
        }
        return ans;
    }
}; 
