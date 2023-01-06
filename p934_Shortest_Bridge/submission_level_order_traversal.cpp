bool const debug = false;

typedef vector<int> VI;
typedef vector<VI> VVI;

VVI dir = {
    {0, -1},
    {-1, 0},
    {+1, 0},
    {0, +1}
};

int const INVALID = -1;
int const MAXL = 100;
int openSet[MAXL][MAXL], closedSet[MAXL][MAXL];

class Solution {
public:
    void recolor(int y, int x, int upperY, int upperX, int origColor, int newColor, VVI& grid, queue<VI>& q) {
        if (grid[y][x] != origColor) return;
        grid[y][x] = newColor;
        q.push({y, x});
        openSet[y][x] = true;
        for (auto &v : dir) {
            int ny = y+v[0], nx = x+v[1];
            if (ny < 0 || nx < 0 || ny >= upperY || nx >= upperX) continue;
            recolor(ny, nx, upperY, upperX, origColor, newColor, grid, q);
        }
    }

    int shortestBridge(vector<vector<int>>& grid) {
        int upperY = grid.size(), upperX = grid[0].size();
        queue<VI> q;
        memset(openSet, false, sizeof openSet);
        memset(closedSet, false, sizeof closedSet);
        bool recolorDone = false;
        for (int y = 0; y < upperY; ++y) {
            if (recolorDone) break;
            for (int x = 0; x < upperY; ++x) {
                if (0 == grid[y][x]) continue;
                recolor(y, x, upperY, upperY, 1, 2, grid, q);
                recolorDone = true;
                break;
            }
        }

        int lv = 0;
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                VI& f = q.front();
                int y = f[0], x = f[1];
                q.pop();
                openSet[y][x] = false;
                closedSet[y][x] = true;
                for (auto &v : dir) {
                    int ny = y+v[0], nx = x+v[1];
                    if (ny < 0 || nx < 0 || ny >= upperY || nx >= upperX) continue;
                    if (openSet[ny][nx] || closedSet[ny][nx]) continue;
                    if (1 == grid[ny][nx]) return lv;  
                    q.push({ny, nx});
                    openSet[ny][nx] = true;    
                }
            }
            ++lv;
        }

        return INVALID;
    }
};
