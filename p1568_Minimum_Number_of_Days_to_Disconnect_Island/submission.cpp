#define MAXL 30
#define MAXIDX 900
#define INVALID -1

#define VI vector<int>
#define PI pair<int, int>

// "adj[u][v] = w" means that "u -> v" is weighted "w"
#define ADJ_ELE_T unordered_map<int, int>
#define ADJ_T unordered_map<int, ADJ_ELE_T> 
int connectedCompId[MAXL][MAXL];

vector<int> diry{-1, +1, 0, 0}; // +1 means down
vector<int> dirx{0, 0, -1, +1}; // +1 means right

// TarjanBridgeFinding algorithm specific
int indexes[MAXIDX];
int lowIndirectLinks[MAXIDX];

/*
test case #1
[[1,0,1],[1,1,1],[1,0,1],[1,0,1]]
  
test case #2
[[1,0,1,1],[1,1,1,1],[1,0,1,1],[1,0,1,1]]

test case #3
[[1,1,0,1,1],[1,1,1,1,1],[1,1,0,1,1]]

test case #4
[[1,1,0,1,1],[1,1,1,1,1],[1,1,0,1,1],[1,1,0,1,1]]

test case #5
[[1,1,1,1,1],[1,1,1,1,1],[1,1,0,1,1],[1,1,0,1,1]]

test case #6
[[1,1,0,1,1],[1,1,1,1,1],[1,1,0,1,1],[1,1,1,1,1]]
  
test case #7
[[1,1]]

test case #8
[[0,1,1],[1,1,1],[1,1,0]]
*/
class Solution {
public:
    void dfsCountIslands(int newConnectedCompId, int y, int x, int &yUpper, int &xUpper, vector<vector<int>> &grid) {
        connectedCompId[y][x] = newConnectedCompId;
        for (int i = 0; i < 4; ++i) {
            int ny = y + diry[i];
            int nx = x + dirx[i];
            if (0 > ny || ny >= yUpper || 0 > nx || nx >= xUpper) continue;
            if (0 == grid[ny][nx] || INVALID != connectedCompId[ny][nx]) continue;
            dfsCountIslands(newConnectedCompId, ny, nx, yUpper, xUpper, grid);
        }
    }
    
    int numIslands(vector<vector<int>>& grid) {
        // See https://github.com/genxium/Leetcode/blob/master/p200_Number_of_Islands/submission.cpp for details.
        memset(connectedCompId, INVALID, sizeof(connectedCompId));
        int yUpper = grid.size();
        if (0 == yUpper) return 0;
        int xUpper = grid[0].size();
        int currentConnectedCompId = 0;
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
              if (1 == grid[y][x] && INVALID == connectedCompId[y][x]) {
                dfsCountIslands(currentConnectedCompId, y, x, yUpper, xUpper, grid);
                ++currentConnectedCompId;
              }
            }
        }
        return currentConnectedCompId;
    }

    void findarticulation(int dfsDirectPredecessor, int cur, int &dfsIndex, ADJ_T &adj, unordered_set<int> &articulationOut) {
        indexes[cur] = dfsIndex++;
        lowIndirectLinks[cur] = indexes[cur];
        auto& neighboursOfCur = adj[cur];
        int children = 0;
        for (auto &it : neighboursOfCur) {
            int nb = it.first;
            if (INVALID == indexes[nb]) {
                findarticulation(cur, nb, dfsIndex, adj, articulationOut);
                lowIndirectLinks[cur] = (lowIndirectLinks[cur] < lowIndirectLinks[nb] ? lowIndirectLinks[cur] : lowIndirectLinks[nb]);
                if (lowIndirectLinks[nb] >= indexes[cur] && INVALID != dfsDirectPredecessor) {
                    articulationOut.insert(cur);
                }
                children++;
            } else {
                if (INVALID != dfsDirectPredecessor && nb != dfsDirectPredecessor) {
                    lowIndirectLinks[cur] = (lowIndirectLinks[cur] < indexes[nb] ? lowIndirectLinks[cur] : indexes[nb]);
                    /* 
                    Unlike "findbridge", the following version of "lowIndirectLinks[nb]" assignment doesn't work, consider an example below.

                    If "cur" sits inside a cycle "(prev1, prev2, prev3, prev4, cur, prev1)" where the traversal order in the cycle is "prev1 -> prev2 -> prev3 -> prev4 -> cur -> ... -> nb", yet between "cur -> nb" it went through "cur -> prev4 (skipped) -> prev1 (updated lowIndirectLinks[cur] = lowIndirectLinks[prev1] <= indexes[prev1] < indexes[cur])".

                    Further assume that "cur" and "nb" sit in another cycle "cur -> nb -> ob -> pb -> qb -> cur", then "findarticulation(..., nb, ...)" would update "lowIndirectLinks[nb] = ... = lowIndirectLinks[qb] = lowIndirectLinks[cur] < indexes[cur]", as the "articulation condition for `cur`" being "lowIndirectLinks[nb] >= indexes[cur] && INVALID != dfsDirectPredecessor", we'd fail to recognize "cur" as a valid articulation.
                    */
                    //lowIndirectLinks[cur] = (lowIndirectLinks[cur] < lowIndirectLinks[nb] ? lowIndirectLinks[cur] : lowIndirectLinks[nb]);
                }
            }
        }

        if (INVALID == dfsDirectPredecessor && 1 < children) {
            articulationOut.insert(cur);
        }
    }
    
    int minDays(vector<vector<int>>& grid) {
        int numIslandsOrig = numIslands(grid);
        if (0 == numIslandsOrig || 1 < numIslandsOrig) return 0;
        
        // now that "1 == numsIslandOrig"
        ADJ_T adj;                                                                                                                                                   
        int yUpper = grid.size(), xUpper = grid[0].size();                                                   
        for (int y = 0; y < yUpper; ++y) {                                                                   
            for (int x = 0; x < xUpper; ++x) {     
                if (1 != grid[y][x]) continue;
                int u = y*xUpper+x;
                for (int k = 0; k < 4; ++k) {                                                                 
                    int ny = y+diry[k], nx = x+dirx[k];                                                       
                    if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;                           
                    if (1 != grid[ny][nx]) continue;                                                   
                    int v = ny*xUpper+nx;                                                                 
                    adj[u][v] = 1; // initially the weight between each "u -> v" is all "1"
                }                                                                      
            }                                                                              
        }                                                                                           
        
        if (2 >= adj.size()) {
            // special cases for this problem
            return 1 > adj.size() ? 1 : adj.size();
        }
        
        int ans = 2;
        memset(indexes, INVALID, sizeof(indexes));
        memset(lowIndirectLinks, INVALID, sizeof(lowIndirectLinks));
        unordered_set<int> articulationOut;
        auto a = adj.begin()->first; // choose just an arbitrary node
        int dfsIndex = 0;
        findarticulation(INVALID, a, dfsIndex, adj, articulationOut);
        if (articulationOut.size() > 0) {
            ans = 1;
        }

        return ans;
    }
};
