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
*/
class Solution {
private:
    pair<int, int> toCoor(int u, int xUpper) {
        return {u/xUpper, u%xUpper};
    }
    
    void printAdjList(ADJ_T& adj, int xUpper) {
        for (auto &it : adj) {
            int u = it.first;
            auto coor = toCoor(u, xUpper);
            printf("adj[{%d, %d}]: ", coor.first, coor.second);
            for (auto &it : adj[u]) {
                int nb = it.first;
                auto nbCoor = toCoor(nb, xUpper);
                printf("{%d, %d}=%d ", nbCoor.first, nbCoor.second, it.second);
            }
            printf("\n");
        }
    }

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
    
    int MinCut(ADJ_T &adj, int xUpper) {
        int origVertices = adj.size();
        if (origVertices < 2) {
            return 0;
        }
        int phaseIdx = 0;
        auto a = adj.begin()->first; // choose just an arbitrary node
        int minCut = INT_MAX;
        bool A[MAXIDX]; // also the "closedSet" for each "MinCutPhase"
        int weightWrtA[MAXIDX]; // for each "MinCutPhase"

        while (phaseIdx+1 < origVertices) {
            // printf("phaseIdx: %d\n", phaseIdx);
            //printAdjList(adj, xUpper);
            
            // each "MinCutPhase"
            int totVerticesThisPhase = origVertices-phaseIdx;
            priority_queue<PI, vector<PI>, less<PI>> openHeap; // a MaxHeap 
            memset(A, false, sizeof(A));        
            memset(weightWrtA, 0, sizeof(weightWrtA));

            // init
            A[a] = true;
            int countInA = 1;
            int t = a, // a.k.a. "lastAdded"
                s = INVALID; // a.k.a. "secondLastAdded"
            
            auto& neighboursOfStarter = adj[a];
            for (auto &it : neighboursOfStarter) {
                int nb = it.first;
                weightWrtA[nb] = it.second;
                openHeap.push({
                    weightWrtA[nb], 
                    nb
                });
            }

            // loop
            while (countInA < totVerticesThisPhase) {
                // add to "A" the most tightly connected vertex
                PI expander = openHeap.top(); openHeap.pop();
                int b = expander.second;
                if (true == A[b]) continue;
                A[b] = true;
                ++countInA;

                // update elements in "pq", only those that are neighbours of "b" should be taken care of
                for (auto &it : adj[b]) {
                    int nb = it.first;
                    if (true == A[nb]) continue;
                    weightWrtA[nb] += it.second;

                    // it doesn't matter whether or not "nb" is already in "openHeap"
                    openHeap.push({
                        weightWrtA[nb], 
                        nb
                    });
                }

                // update "t a.k.a. lastAdded" & "s a.k.a. secondLastAdded"
                s = t;
                t = b;
            }

            // cache the phase-cut value
            int phaseCut = 0;
            ADJ_ELE_T& neighboursOfLastAdded = adj[t];
            ADJ_ELE_T& neighboursOfSecondLastAdded = adj[s];

            for (auto &it : neighboursOfLastAdded) {
                int nbOfLastAdded = it.first;
                phaseCut += it.second;
            }

            // shrink G by merging "t a.k.a. lastAdded" into "s a.k.a. secondLastAdded"
            for (auto &it : neighboursOfLastAdded) {
                int nbOfLastAdded = it.first;
                if (nbOfLastAdded == s) continue;

                neighboursOfSecondLastAdded[nbOfLastAdded] += it.second; // note that adj[s] might not contain "nbOfLastAdded" until now
                adj[nbOfLastAdded][s] = neighboursOfSecondLastAdded[nbOfLastAdded]; // it's an UndirectionalGraph
            }

            // printf("\terasing lastAdded: {%d, %d} from adj\n", lastAddedCoor.first, lastAddedCoor.second);
            for (auto &it : neighboursOfLastAdded) {
                adj[it.first].erase(t);
            }
            adj.erase(t);
        
            minCut = (minCut < phaseCut ? minCut : phaseCut);
            ++phaseIdx;
        }
        return minCut;
    }
    
    void tarjanBridgeFind(int dfsDirectPredecessor, int cur, int &dfsIndex, ADJ_T &adj, vector<VI> &bridgesOut) {
        indexes[cur] = dfsIndex++;
        lowIndirectLinks[cur] = indexes[cur];
        auto& neighboursOfCur = adj[cur];
        for (auto &it : neighboursOfCur) {
            int nb = it.first;
            if (INVALID == indexes[nb]) {
                tarjanBridgeFind(cur, nb, dfsIndex, adj, bridgesOut);
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
        // printAdjList(adj, xUpper);   
        if (2 >= adj.size()) {
            // special cases for this problem
            return 1 > adj.size() ? 1 : adj.size();
        }
        
        int ans = 2;
        // Apply TarjanBridgeFinding algorithm to find bridges, the trick here is that if there were no bridge, we can still disconnect the island in AT MOST 2 days, by choosing just any "corner 1" and cutting its 2 neighbours.
        memset(indexes, INVALID, sizeof(indexes));
        memset(lowIndirectLinks, INVALID, sizeof(lowIndirectLinks));
        vector<VI> bridgesOut;
        auto a = adj.begin()->first; // choose just an arbitrary node
        int dfsIndex = 0;
        tarjanBridgeFind(INVALID, a, dfsIndex, adj, bridgesOut);
        if (bridgesOut.size() > 0) {
            ans = 1;
        }
        
        // Applying Stoer-Wagner algorithm to find the min-cut of an UndirectedGraph "adj" here will get TLE, but all test-cases passed in terms of matching the expected outputs.
        // ans = MinCut(adj, xUpper);
        return ans;
    }
};
