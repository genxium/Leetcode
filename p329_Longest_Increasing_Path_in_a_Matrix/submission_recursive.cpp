#define INVALID -1

vector<int> diry{+1, -1, 0, 0};
vector<int> dirx{0, 0, -1, +1};

class Solution {
public:
    void printAdj(unordered_map<int, vector<int>> &adj, int &xUpper, vector<vector<int>> &matrix) {
      for (auto &it : adj) {
        auto idx = it.first;
        int y = idx/xUpper, x = idx%xUpper;
        printf("(y:%d, x:%d):%d\n", y, x, matrix[y][x]);
        auto nbs = it.second;
        for (auto &nb : nbs) {
          int ny = nb/xUpper, nx = nb%xUpper;
          printf("\t(ny:%d, nx:%d):%d\n", ny, nx, matrix[ny][nx]);
        }
      }
    }

    int search(int i, int pathLengthNow, unordered_map<int, vector<int>> &adj, vector<bool> visited, vector<int> &maxPathLengths, int &ans) {
      if (INVALID != maxPathLengths[i]) {
        return maxPathLengths[i];
      }

      int toRet = 1;

      visited[i] = true;
      if (pathLengthNow > ans) {
        ans = pathLengthNow;
      }
      vector<int> nbs = adj[i];
      int tmp = 0;
      for (auto &nb : nbs) {
        if (true == visited[nb]) continue;
        int candidate = search(nb, pathLengthNow+1, adj, visited, maxPathLengths, ans);
        if (candidate > tmp) tmp = candidate;
      }
      visited[i] = false; // backtracking
      toRet += tmp;
      maxPathLengths[i] = toRet;
      return toRet;
    }

    int longestIncreasingPath(vector<vector<int>>& matrix) {
      /*
      test case #1
      [[9,9,4],[6,6,8],[2,1,1]]
      */
      unordered_map<int, vector<int>> adj;
      int yUpper = matrix.size();
      if (0 == yUpper) return 0;
      int xUpper = matrix[0].size();

      int size = yUpper*xUpper;
      vector<bool> visited(size, false);

      unordered_set<int> zeroIndegreeGrids;
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          int idx = y*xUpper+x;
          zeroIndegreeGrids.insert(idx);
        }
      }

      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          int idx = y*xUpper+x;
          adj[idx] = {};
          for (int i = 0; i < 4; ++i) {
            int ny = y+diry[i], nx = x+dirx[i];
            if (0 > ny || 0 > nx || yUpper <= ny || xUpper <= nx) continue;
            if (matrix[ny][nx] <= matrix[y][x]) continue;
            int nidx = ny*xUpper+nx;
            adj[idx].push_back(nidx);
            zeroIndegreeGrids.erase(nidx);
          }
        }
      }

      vector<int> maxPathLengths(size, INVALID);

      //printAdj(adj, xUpper, matrix);

      int ans = 0;
      for (auto &i : zeroIndegreeGrids) {
        //printf("Starting from (y:%d, x:%d):%d\n", i/xUpper, i%xUpper, matrix[i/xUpper][i%xUpper]);
        int candidate = search(i, 1, adj, visited, maxPathLengths, ans);
        if (candidate > ans) ans = candidate;
      }
      return ans;
    }
};
