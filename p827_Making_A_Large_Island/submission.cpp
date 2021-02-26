bool debug = false;
int const MAXN = 500;
int const INVALID = -1;
int connectedCompId[MAXN][MAXN]; 
int connectedCompSize[MAXN*MAXN]; 

int dy[] = {-1, +1, 0, 0}; // +1 means down
int dx[] = {0, 0, -1, +1}; // +1 means right

/*
test cases
[[0,0],[0,0]]
[[1,0],[0,1]]
[[1,1],[1,0]]
[[1,1],[1,1]]
[[0,0,0,0,0,0,0],[0,1,1,1,1,0,0],[0,1,0,0,1,0,0],[1,0,1,0,1,0,0],[0,1,0,0,1,0,0],[0,1,0,0,1,0,0],[0,1,1,1,1,0,0]]
*/
class Solution {
public:
    void dfs(int newConnectedCompId, int y, int x, int &size, int &yUpper, int &xUpper, vector<vector<int>> &grid) {
        connectedCompId[y][x] = newConnectedCompId;
        ++size;
        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i], nx = x + dx[i];
            if (0 > ny || ny >= yUpper || 0 > nx || nx >= xUpper) continue;
            if (0 == grid[ny][nx] || INVALID != connectedCompId[ny][nx]) continue;
            dfs(newConnectedCompId, ny, nx, size, yUpper, xUpper, grid);
        }
    }
    
    int largestIsland(vector<vector<int>>& grid) {
        int yUpper = grid.size(), xUpper = grid[0].size();
        memset(connectedCompId, INVALID, sizeof connectedCompId);
        memset(connectedCompSize, 0, sizeof connectedCompSize);
        
        int ans = 1; // Can at least get 1.
        int compId = 0;
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                if (0 == grid[y][x]) continue;
                if (INVALID != connectedCompId[y][x]) continue;
                int size = 0;
                dfs(compId, y, x, size, yUpper, xUpper, grid);
                connectedCompSize[compId] = size;
                if (debug) printf("connectedCompSize[compId:%d] == %d\n", compId, size);
                ans = max(ans, size);
                ++compId;
            }
        }
        
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                if (1 == grid[y][x]) continue;
                
                int cand = 1; // This grid itself after changed to 1
                unordered_map<int, int> counter; // compId -> size
                for (int i = 0; i < 4; ++i) {
                    int ny = y + dy[i], nx = x + dx[i];
                    if (0 > ny || ny >= yUpper || 0 > nx || nx >= xUpper) continue;
                    int compIdThere = connectedCompId[ny][nx]; 
                    if (INVALID == compIdThere) continue;
                    counter[compIdThere] = connectedCompSize[compIdThere];
                }
                
                for (auto &it : counter) cand += it.second;
                ans = max(ans, cand);
            }
        }
        
        return ans;
    }
};
