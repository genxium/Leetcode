#define MAXL 1000
#define INVALID -1
int connectedCompId[MAXL][MAXL]; 

vector<int> diry{-1, +1, 0, 0}; // +1 means down
vector<int> dirx{0, 0, -1, +1}; // +1 means right

class Solution {
public:
    void dfs(int newConnectedCompId, int y, int x, int &yUpper, int &xUpper, vector<vector<char>> &grid) {
      connectedCompId[y][x] = newConnectedCompId;
      for (int i = 0; i < 4; ++i) {
        int ny = y + diry[i];
        int nx = x + dirx[i];
        if (0 > ny || ny >= yUpper || 0 > nx || nx >= xUpper) continue;
        if ('0' == grid[ny][nx] || INVALID != connectedCompId[ny][nx]) continue;
        dfs(newConnectedCompId, ny, nx, yUpper, xUpper, grid);
      }
    }
  
    int numIslands(vector<vector<char>>& grid) {
      memset(connectedCompId, INVALID, sizeof(connectedCompId));
      int yUpper = grid.size();
      if (0 == yUpper) return 0;
      int xUpper = grid[0].size();
      int currentConnectedCompId = 0;
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          if ('1' == grid[y][x] && INVALID == connectedCompId[y][x]) {
            dfs(currentConnectedCompId, y, x, yUpper, xUpper, grid);
            ++currentConnectedCompId;
          }
        }
      }
      return currentConnectedCompId;
    }
};
