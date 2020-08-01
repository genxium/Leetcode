#define DEAD 0
#define ALIVE 1
#define DEAD_TURNED_ALIVE 2
#define ALIVE_TURNED_DEAD -1

class Solution {
  public:
    void gameOfLife(vector<vector<int>>& board) {
      vector<int> neighbors{0, 1, -1};

      int yUpper = board.size(), xUpper = board[0].size();
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          // For each cell count the number of live neighbors.
          int liveNeighbors = 0;
          for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
              if ((neighbors[i] == 0 && neighbors[j] == 0)) continue;
              int ny = (y + neighbors[i]);
              if (ny >= yUpper || ny < 0) continue;
              int nx = (x + neighbors[j]);
              if (nx >= xUpper || nx < 0) continue;
              if (ALIVE != board[ny][nx] && ALIVE_TURNED_DEAD != board[ny][nx]) continue;
              ++liveNeighbors;
            }
          }

          if ((board[y][x] == 1) && (liveNeighbors < 2 || liveNeighbors > 3)) {
            board[y][x] = ALIVE_TURNED_DEAD;
          }

          if (board[y][x] == 0 && liveNeighbors == 3) {
            board[y][x] = DEAD_TURNED_ALIVE;
          }
        }
      }

      // Get the final representation for the newly updated board.
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          if (board[y][x] == DEAD_TURNED_ALIVE || board[y][x] == ALIVE) {
            board[y][x] = ALIVE;
          } else {
            board[y][x] = DEAD;
          }
        }
      }
    }
};
