bool const debug = false;

typedef vector<int> VI;
typedef vector<VI> VVI;

struct PqCmp {
    bool operator() (VI const& lhs, VI const& rhs) const {
        // each is {y, x, min distance to target color when pushed}
        return lhs[2] > rhs[2];
    }
};

VVI dir = {
    {0, -1},
    {-1, 0},
    {+1, 0},
    {0, +1}
};

int const INVALID = -1;
int const MAXL = 100;
int md[2][MAXL][MAXL]; // min distance to the nearest of color 1 or 2 
int closedSet[MAXL][MAXL];

/*
test cases
[[0,1],[1,0]]
[[0,1,0],[0,0,0],[0,0,1]]
[[1,1,1,1,1],[1,0,0,0,1],[1,0,1,0,1],[1,0,0,0,1],[1,1,1,1,1]]
[[0,1,0,0,0,0],[0,1,1,1,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[0,0,0,0,0,0],[1,1,0,0,0,0]]
*/
class Solution {
public:
    void recolor(int y, int x, int upperY, int upperX, int origColor, int newColor, VVI& grid) {
        if (grid[y][x] != origColor) return;
        grid[y][x] = newColor;
        for (auto &v : dir) {
            int ny = y+v[0], nx = x+v[1];
            if (ny < 0 || nx < 0 || ny >= upperY || nx >= upperX) continue;
            recolor(ny, nx, upperY, upperX, origColor, newColor, grid);
        }
    }

    void calcMd(int color, int upperY, int upperX, VVI& grid) {
        if (debug) printf("calcMd, color:%d\n", color);
        int colorIdx = color-1;
        priority_queue<VI, VVI, PqCmp> pq;
        for (int y = 0; y < upperY; ++y) {
            for (int x = 0; x < upperX; ++x) {
                if (grid[y][x] != color) continue;
                pq.push({y, x, 0});
            }
        }

        if (debug) printf("\tinitial pq size: %d\n", pq.size());
        memset(closedSet, false, sizeof closedSet);
        while (!pq.empty()) {
            auto u = pq.top(); pq.pop();
            int y = u[0], x = u[1], d = u[2];
            if (debug) printf("\texpanding by (y:%d, x:%d, d:%d)\n", y, x, d);
            if (closedSet[y][x]) {
                // already traversed by a later pushed but earlier popped version
                continue;
            }
            md[colorIdx][y][x] = d;
            closedSet[y][x] = true;
            for (auto &v : dir) {
                int ny = y+v[0], nx = x+v[1];
                if (ny < 0 || nx < 0 || ny >= upperY || nx >= upperX) continue;
                if (0 != grid[ny][nx]) continue;
                int cand = d+1;
                if (INVALID == md[colorIdx][ny][nx] || md[colorIdx][ny][nx] > cand) {
                    // relaxed
                    md[colorIdx][ny][nx] = cand; 
                    pq.push({ny, nx, cand});
                }
            }
        }
    }

    int shortestBridge(VVI& grid) {
        int upperY = grid.size(), upperX = grid[0].size();
        memset(md, INVALID, sizeof md);

        bool recolorDone = false;
        for (int y = 0; y < upperY; ++y) {
            if (recolorDone) break;
            for (int x = 0; x < upperY; ++x) {
                if (0 == grid[y][x]) continue;
                recolor(y, x, upperY, upperY, 1, 2, grid);
                recolorDone = true;
                break;
            }
        }

        if (debug) {
            printf("recolorDone.\n");
            for (int y = 0; y < upperY; ++y) {
                for (int x = 0; x < upperY; ++x) {
                    printf("%d ", grid[y][x]);
                }
                puts("");
            }
        }

        calcMd(1, upperY, upperX, grid);

        if (debug) printf("calc md 1 Done.\n");

        calcMd(2, upperY, upperX, grid);

        if (debug) printf("calc md 2 Done.\n");
        
        int ans = INT_MAX;
        for (int y = 0; y < upperY; ++y) {
            for (int x = 0; x < upperY; ++x) {
                if (0 != grid[y][x]) continue;
                if (INVALID == md[0][y][x] || INVALID == md[1][y][x]) continue;
                int cand = md[0][y][x] + md[1][y][x];
                ans = min(ans, cand);
            }
        }

        return ans-1;
    }
};
