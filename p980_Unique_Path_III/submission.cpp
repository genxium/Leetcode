#define MAXN 101

vector<int> diry{0, 0, -1, +1}; // +1 == down
vector<int> dirx{-1, +1, 0, 0}; // +1 == right
bool visited[MAXN*MAXN]; 

class Solution {
  public:
    void dfs(vector<vector<int>>& grid, int &nSteps, int &nonObstacleCount, int &edY, int &edX, int y, int x, int &yUpper, int &xUpper, bool visited[], int &ans) {
      if (y == edY && x == edX) {
        if (nSteps == nonObstacleCount) {
            ans += 1;            
        }
        return;
      } 

      for (int t = 0; t < 4; ++t) {
        int nbY = y + diry[t];
        int nbX = x + dirx[t];
        if (nbY < 0 || nbY >= yUpper || nbX < 0 || nbX >= xUpper) continue;

        int idx = nbY*xUpper + nbX;

        if (true == visited[idx] || grid[nbY][nbX] == -1) {
          continue;
        }
        visited[idx] = true; ++nSteps;
        dfs(grid, nSteps, nonObstacleCount, edY, edX, nbY, nbX, yUpper, xUpper, visited, ans);
        visited[idx] = false; --nSteps; // for "backtracking"
      }
    }

    int uniquePathsIII(vector<vector<int>>& grid) {
      int yUpper = grid.size();
      if (0 == yUpper) return 0;
      int xUpper = grid[0].size();

      int stY, stX, edY, edX;
      int nonObstacleCount = 0; // Including st & ed.
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          if (grid[y][x] == -1) {
              continue;
          }
          nonObstacleCount += 1;
          if (grid[y][x] == 1) {
            stY = y;
            stX = x;
          }

          if (grid[y][x] == 2) {
            edY = y;
            edX = x;
          }
        }    
      }

      int ans = 0, nSteps = 1;
      memset(visited, 0, sizeof(visited));
      visited[stY*xUpper + stX] = true;

      dfs(grid, nSteps, nonObstacleCount, edY, edX, stY, stX, yUpper, xUpper, visited, ans);
      return ans;
    }
};
