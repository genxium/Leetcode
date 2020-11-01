vector<int> diry{+1, -1, 0, 0};
vector<int> dirx{0, 0, +1, -1};

int const MAXXY = 100;
bool openSet[MAXXY][MAXXY], closedSet[MAXXY][MAXXY];

/*
test case#1
[[1,1,0],[1,1,1]]

test case#2
[[1,1],[1,1]]
*/
class Solution {
public:
    void solve(int sy, int sx, int yUpper, int xUpper, vector<vector<int>>& grid, int &sharedEdgesCount) {
      queue<pair<int, int>> q;
      q.push({sy, sx});
      openSet[sy][sx] = true;
      while (!q.empty()) {
        auto expander = q.front(); q.pop();
        int y = expander.first, x = expander.second;
        closedSet[y][x] = true;
        openSet[y][x] = false;
        for (int i = 0; i < 4; ++i) {
          int dy = diry[i], dx = dirx[i];
          int ny = y+dy, nx = x+dx;
          if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
          if (closedSet[ny][nx]) continue;
          if (grid[ny][nx] != 1) continue;
          //printf("{y:%d, x:%d} and {ny:%d, nx:%d} has a new shared edge\n", y, x, ny, nx);
          ++sharedEdgesCount;
          if (openSet[ny][nx]) continue;
          openSet[ny][nx] = true;
          q.push({ny, nx});
        }
      }      
    }
  
    int islandPerimeter(vector<vector<int>>& grid) {
      memset(openSet, false, sizeof(openSet));
      memset(closedSet, false, sizeof(closedSet));
      int yUpper = grid.size();
      int xUpper = grid[0].size();
      int oneCount = 0, sharedEdgesCount = 0;
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          if (1 == grid[y][x]) {
            ++oneCount;
            if (closedSet[y][x]) continue;
            solve(y, x, yUpper, xUpper, grid, sharedEdgesCount);
          }
        }
      }
      return 4*oneCount-2*sharedEdgesCount;
    }
};
