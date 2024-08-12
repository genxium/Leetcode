#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <string.h>
using namespace std;

#define INVALID -1
#define MAXN 100000

int indexes[MAXN];
int lowIndirectLinks[MAXN];

class Solution {
    public:
        void findbridge(int dfsDirectPredecessor, int cur, int &dfsIndex, unordered_map<int, vector<int>> &adj, vector<vector<int>> &bridgesOut) {
            indexes[cur] = dfsIndex++;
            lowIndirectLinks[cur] = indexes[cur];
            int iUpper = adj[cur].size();
            for (int i = 0; i < iUpper; ++i) {
                int nb = adj[cur][i];
                if (INVALID == indexes[nb]) {
                    findbridge(cur, nb, dfsIndex, adj, bridgesOut);
                    lowIndirectLinks[cur] = (lowIndirectLinks[cur] < lowIndirectLinks[nb] ? lowIndirectLinks[cur] : lowIndirectLinks[nb]);
                    if (lowIndirectLinks[nb] == indexes[nb]) {
                        bridgesOut.push_back({cur, nb});
                    }
                } else {
                    if (INVALID != dfsDirectPredecessor && nb != dfsDirectPredecessor) {
                        /*          
                        [WARNING] NOT using "lowIndirectLinks[nb]" here because when "INVALID != indexes[nb]", the value of "indexes[nb]" could've been assigned at the beginning of its own "tarjanFindBridge(..., nb, ...)" which has NOT YET returned (due to graph being "undirectional"), i.e. "lowIndirectLinks[nb]" not determined yet.  
                         */
                        lowIndirectLinks[cur] = (lowIndirectLinks[cur] < indexes[nb] ? lowIndirectLinks[cur] : indexes[nb]);

                        /*
                        [WARNING] However, using the following assignment instead is still correct for finding bridges. Because if {cur, nb} is a bridge where "cur" is the "dfsDirectPredecessor", then there is no chance "lowIndirectLinks[nb]" can be assign to a value smaller than either "indexes[nb]" or "indexes[cur]", i.e. "lowIndirectLinks[nb] >= indexes[nb]" as long as successors of "nb" during the whole depth-first-search couldn't reach back to "cur or any predecessor of cur" unless via {nb, cur}. 
                        Tested in https://leetcode.com/problems/critical-connections-in-a-network.    
                         */
                        // lowIndirectLinks[cur] = (lowIndirectLinks[cur] < lowIndirectLinks[nb] ? lowIndirectLinks[cur] : lowIndirectLinks[nb]);
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
            unordered_map<int, vector<int>> adj;
            for (vector<int> &connection : connections) {
                int u = connection[0];
                int v = connection[1];

                auto uIt = adj.find(u); 
                if (uIt == adj.end()) {
                    adj[u] = vector<int>{};
                } else {
                    adj[u] = uIt->second;
                }

                auto vIt = adj.find(v);
                if (vIt == adj.end()) {
                    adj[v] = vector<int>{};
                } else {
                    adj[v] = vIt->second;
                }

                adj[u].push_back(v);
                adj[v].push_back(u);
            }

            vector<vector<int>> ans;
            int starter = 0, dfsIndex = 0;
            findbridge(INVALID, starter, dfsIndex, adj, ans);
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
