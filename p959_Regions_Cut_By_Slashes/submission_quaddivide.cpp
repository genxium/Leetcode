bool debug = false;
typedef vector<int> VI;
typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ;

int dy[] = {+1, -1, 0, 0};
int dx[] = {0, 0, +1, -1};

class Solution {
public:
    void dfs(int newConnectedCompId, int nodeI, unordered_set<int>& seen, ADJ& adj) {
        if (seen.count(nodeI)) return;
        seen.insert(nodeI);
        for (auto &nb : adj[nodeI]) {
            dfs(newConnectedCompId, nb, seen, adj);
        }
    }

    void connect(ADJ& adj, int i, int j) {
        adj[i].insert(j);
        adj[j].insert(i);        
    }
    
    int regionsBySlashes(vector<string>& grid) {
        /*
        Assume that (y,x) grid is ALWAYS separated to 4 sections as follows
          
            1
          ----
        0 |\/|  2
          |/\|
          ----
           3
           
        where the i-th section has a "node index = 4*(y*xUpper+x)+i" 
        */
        ADJ adj;
        int yUpper = grid.size(), xUpper = grid[0].size();
        if (debug) printf("Inited grid of size (%d, %d)\n", yUpper, xUpper);
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                int mainOffset = 4*(y*xUpper+x), val = grid[y][x];
                switch (val) {
                    case '/':
                        connect(adj, mainOffset+0, mainOffset+1);
                        connect(adj, mainOffset+2, mainOffset+3);
                        break;
                    case '\\':
                        connect(adj, mainOffset+0, mainOffset+3);
                        connect(adj, mainOffset+1, mainOffset+2);    
                        break;
                    default:
                        connect(adj, mainOffset+0, mainOffset+1);
                        connect(adj, mainOffset+0, mainOffset+3);
                        connect(adj, mainOffset+2, mainOffset+1);
                        connect(adj, mainOffset+2, mainOffset+3);
                        break;
                }
                
                for (int i = 0; i < 4; ++i) {
                    int ny = y+dy[i], nx = x+dx[i];
                    if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                    int nmainOffset = 4*(ny*xUpper+nx);
                    switch (i) {
                        case 0: connect(adj, mainOffset+3, nmainOffset+1); break;
                        case 1: connect(adj, mainOffset+1, nmainOffset+3); break;
                        case 2: connect(adj, mainOffset+2, nmainOffset+0); break;
                        case 3: connect(adj, mainOffset+0, nmainOffset+2); break;
                    }
                }
            }
        }
        
        if (debug) printf("Inited\n");
        
        int ans = 0;
        unordered_set<int> seen;
        for (auto& [nodeI, nbs] : adj) {
            if (seen.count(nodeI)) continue;
            if (debug) printf("New comp at nodeI:%d\n", nodeI);
            dfs(ans, nodeI, seen, adj);
            ++ans;
        }
        
        return ans;
    }
};
