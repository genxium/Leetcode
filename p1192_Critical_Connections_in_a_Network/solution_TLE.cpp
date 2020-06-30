#include <stdio.h>
#include <vector>
#include <map>
#include <list>
using namespace std;

#define INVALID -1

class Node {
public:
    int id;
    int index; 
    int lowIndirectLink;
    list<Node*> adj;

    Node(int aId): index(INVALID), lowIndirectLink(INVALID) {
        id = aId;
    }
};

class Solution {
public:
    void dfs(Node* dfsDirectPredecessor, Node* cur, int &index, vector<vector<int>> &bridgesOut) {
        cur->index = index++;
        cur->lowIndirectLink = cur->index;
        for (auto &nb : cur->adj) {
            if (INVALID == nb->index) {
                dfs(cur, nb, index, bridgesOut);
                cur->lowIndirectLink = (cur->lowIndirectLink < nb->lowIndirectLink ? cur->lowIndirectLink : nb->lowIndirectLink);
                if (nb->lowIndirectLink == nb->index) {
                    bridgesOut.push_back({cur->id, nb->id});
                }
            } else {
                if (nb != dfsDirectPredecessor) {
                    cur->lowIndirectLink = (cur->lowIndirectLink < nb->index ? cur->lowIndirectLink : nb->index);
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
        map<int, Node*> nodes; // id->Node
        for (auto &connection : connections) {
            int u = connection[0];
            int v = connection[1];
            Node *uNode, *vNode = NULL;
            
            auto uIt = nodes.find(u); 
            if (uIt == nodes.end()) {
                uNode = new Node(u);
                nodes[u] = uNode;
            } else {
                uNode = uIt->second;
            }
            
            auto vIt = nodes.find(v);
            if (vIt == nodes.end()) {
                vNode = new Node(v);
                nodes[v] = vNode;
            } else {
                vNode = vIt->second;
            }
            
            uNode->adj.push_back(vNode);
            vNode->adj.push_back(uNode);
        }
        
        vector<vector<int>> ans;
        Node *starter = (nodes.begin())->second;
        int index = 0;
        dfs(NULL, starter, index, ans);
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
