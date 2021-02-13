bool debug = false;
int const MAXL = 50;
int const INVALID = -1;

int const STATE_IDLE = 0;
int const STATE_CONTAMINATED = 1;
int const STATE_CONTAINED = 2;

int connectedCompId[MAXL][MAXL]; 
bool willBeInfectedByCurrentComp[MAXL][MAXL]; 

int dy[] = {-1, +1, 0, 0}; // +1 means down
int dx[] = {0, 0, -1, +1}; // +1 means right

/*
test cases
[[0,1,0,0,0,0,0,1],[0,1,0,0,0,0,0,1],[0,0,0,0,0,0,0,1],[0,0,0,0,0,0,0,0]]
[[1,1,1],[1,0,1],[1,1,1]]
[[1,1,1,0,0,0,0,0,0],[1,0,1,0,1,1,1,1,1],[1,1,1,0,0,0,0,0,0]]
[[0,1,0,1,1,1,1,1,1,0],[0,0,0,1,0,0,0,0,0,0],[0,0,1,1,1,0,0,0,1,0],[0,0,0,1,1,0,0,1,1,0],[0,1,0,0,1,0,1,1,0,1],[0,0,0,1,0,1,0,1,1,1]]
[[0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,1,0],[0,0,0,0,0,0,0,1,1,0],[0,0,0,0,0,0,1,1,0,1],[0,0,0,0,0,0,0,1,1,1],[0,0,0,0,0,0,0,1,1,0],[0,0,0,0,0,0,0,1,1,0],[0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0]]
[[0,1,0,1,1,1,1,1,1,0],[0,0,0,1,0,0,0,0,0,0],[0,0,1,1,1,0,0,0,1,0],[0,0,0,1,1,0,0,1,1,0],[0,1,0,0,1,0,1,1,0,1],[0,0,0,1,0,1,0,1,1,1],[0,1,0,0,1,0,0,1,1,0],[0,1,0,1,0,0,0,1,1,0],[0,1,1,0,0,1,1,0,0,1],[1,0,1,1,0,1,0,1,0,1]]
*/
class Solution {
private:
    void dfs(int newConnectedCompId, int y, int x, int &yUpper, int &xUpper, vector<vector<int>> &grid, int &willInfectCellsInComp, int &connectedCompWalls) {
        connectedCompId[y][x] = newConnectedCompId;
        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (0 > ny || ny >= yUpper || 0 > nx || nx >= xUpper) continue;
            if (STATE_IDLE == grid[ny][nx]) {
                // The current cell "grid[y][x]" is '1', and the "connectedCompWalls" should increase when there's an adjacent '0'.
                ++connectedCompWalls;
                if (false == willBeInfectedByCurrentComp[ny][nx]) {
                    willBeInfectedByCurrentComp[ny][nx] = true;
                    ++willInfectCellsInComp;
                }
                continue;
            }
            if (STATE_CONTAMINATED == grid[ny][nx]) {
                if (INVALID != connectedCompId[ny][nx]) continue;
                dfs(newConnectedCompId, ny, nx, yUpper, xUpper, grid, willInfectCellsInComp, connectedCompWalls);
            }
        }
    }
    
public:
    int containVirus(vector<vector<int>>& grid) {
        int yUpper = grid.size();
        if (0 == yUpper) return 0;
        int xUpper = grid[0].size();
        
        int ans = 0;
        int round = 0;
        
        do {
            if (debug) {
                for (int y = 0; y < yUpper; ++y) {
                    for (int x = 0; x < xUpper; ++x) {
                        if (x == xUpper-1) printf("%d\n", grid[y][x]);
                        else printf("%d,", grid[y][x]);
                    }
                }   
            }
            
            int maxWillInfectCellsInComp = 0, maxWillInfectCellsInCompId = INVALID, wallsForMaxWillInfectCellsInComp = 0;
            int currentConnectedCompId = 0;
            memset(connectedCompId, INVALID, sizeof(connectedCompId));
            
            for (int y = 0; y < yUpper; ++y) {
                for (int x = 0; x < xUpper; ++x) {
                    if (STATE_CONTAMINATED != grid[y][x] || INVALID != connectedCompId[y][x]) continue;
                    memset(willBeInfectedByCurrentComp, false, sizeof willBeInfectedByCurrentComp);
                    int connectedCompWalls = 0, willInfectCellsInComp = 0;
                    dfs(currentConnectedCompId, y, x, yUpper, xUpper, grid, willInfectCellsInComp, connectedCompWalls);
                    if (willInfectCellsInComp > maxWillInfectCellsInComp) {
                        maxWillInfectCellsInComp = willInfectCellsInComp;
                        maxWillInfectCellsInCompId = currentConnectedCompId;
                        wallsForMaxWillInfectCellsInComp = connectedCompWalls;
                    }
                    ++currentConnectedCompId;
                }
            }
            
            if (0 == maxWillInfectCellsInComp) break;
            
            if (debug) printf("round#%d, maxWillInfectCellsInComp is %d, maxWillInfectCellsInCompId is %d, wallsForMaxWillInfectCellsInComp is %d\n", round++, maxWillInfectCellsInComp, maxWillInfectCellsInCompId, wallsForMaxWillInfectCellsInComp);
            
            ans += wallsForMaxWillInfectCellsInComp;
            
            // Containing.
            for (int y = 0; y < yUpper; ++y) {
                for (int x = 0; x < xUpper; ++x) {
                    if (STATE_CONTAMINATED != grid[y][x]) continue;
                    if (maxWillInfectCellsInCompId != connectedCompId[y][x]) continue;
                    grid[y][x] = STATE_CONTAINED;
                }
            }
            
            // Contaminating.
            for (int y = 0; y < yUpper; ++y) {
                for (int x = 0; x < xUpper; ++x) {
                    if (STATE_CONTAMINATED != grid[y][x]) continue;
                    if (INVALID == connectedCompId[y][x] || maxWillInfectCellsInCompId == connectedCompId[y][x]) continue;
                    for (int i = 0; i < 4; ++i) {
                        int ny = y + dy[i], nx = x + dx[i];
                        if (0 > ny || ny >= yUpper || 0 > nx || nx >= xUpper) continue;
                        if (STATE_IDLE != grid[ny][nx]) continue;
                        grid[ny][nx] = STATE_CONTAMINATED;
                    }
                }
            }
        } while (true);
      
        return ans;
    }
};
