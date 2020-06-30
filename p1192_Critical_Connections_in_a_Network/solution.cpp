#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <string.h>
using namespace std;

#define INVALID -1
#define MAXN 100000

int indexes[MAXN];
int lowIndirectLinks[MAXN];

class Solution {
public:
    void dfs(int dfsDirectPredecessor, int cur, int &dfsIndex, map<int, list<int>> &adj, vector<vector<int>> &bridgesOut) {
        indexes[cur] = dfsIndex++;
        lowIndirectLinks[cur] = indexes[cur];
        for (auto &nb : adj[cur]) {
            if (INVALID == indexes[nb]) {
                dfs(cur, nb, dfsIndex, adj, bridgesOut);
                lowIndirectLinks[cur] = (lowIndirectLinks[cur] < lowIndirectLinks[nb] ? lowIndirectLinks[cur] : lowIndirectLinks[nb]);
                if (lowIndirectLinks[nb] == indexes[nb]) {
                    bridgesOut.push_back({cur, nb});
                }
            } else {
                if (INVALID != dfsDirectPredecessor && nb != dfsDirectPredecessor) {
                    lowIndirectLinks[cur] = (lowIndirectLinks[cur] < indexes[nb] ? lowIndirectLinks[cur] : indexes[nb]);
                }   
            }
        }
    }
    
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        /*
        test case #1
        11
        [[0,1],[1,2],[2,3],[3,4],[4,0],[4,5],[5,6],[6,7],[7,5],[4,8],[4,9],[8,10],[9,10]]
        */
        memset(indexes, INVALID, sizeof(indexes));
        memset(lowIndirectLinks, INVALID, sizeof(lowIndirectLinks));
        map<int, list<int>> adj;
        for (auto &connection : connections) {
            int u = connection[0];
            int v = connection[1];
            
            auto uIt = adj.find(u); 
            if (uIt == adj.end()) {
                adj[u] = list<int>{};
            } else {
                adj[u] = uIt->second;
            }
            
            auto vIt = adj.find(v);
            if (vIt == adj.end()) {
                adj[v] = list<int>{};
            } else {
                adj[v] = vIt->second;
            }
            
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        vector<vector<int>> ans;
        int starter = 0, dfsIndex = 0;
        dfs(INVALID, starter, dfsIndex, adj, ans);
        return ans;
    }
};

int main() {
  Solution s;
  vector<vector<int>> connections{{0,1},{1,2},{2,3},{3,4},{4,0},{4,5},{5,6},{6,7},{7,5},{4,8},{4,9},{8,10},{9,10}};
  int n = connections.size();
  vector<vector<int>> ans = s.criticalConnections(n, connections);
  for (auto &single : ans) {
    printf("(%d, %d)\n", single[0], single[1]);
  }
  printf("\n");
  return 0;
}
