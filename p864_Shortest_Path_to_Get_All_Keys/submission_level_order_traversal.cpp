bool debug = true;
int const INVALID = -1;
int const MAXL = 30;
int const MAXK = 6;
int const MAXKS = (1 << MAXK);
bool openSet[MAXL][MAXL][MAXKS], closedSet[MAXL][MAXL][MAXKS];
typedef vector<int> VI;

int dy[] = {+1, -1, 0, 0};
int dx[] = {0, 0, +1, -1};

/*
test cases
["@.a.#","###.#","b.A.B"]
["@..aA","..B#.","....b"]
["@Aa"]
*/
class Solution {
public:
    int shortestPathAllKeys(vector<string>& grid) { 
        int stY = INVALID, stX = INVALID;
        int keyCnt = 0;
        int yUpper = grid.size(), xUpper = grid[0].size();
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                if (grid[y][x] == '@') {
                    stY = y;
                    stX = x;
                } 
                if ('a' <= grid[y][x] && grid[y][x] <= 'f') ++keyCnt;
            }
        }

        memset(openSet, false, sizeof openSet);
        memset(closedSet, false, sizeof closedSet);
        queue<VI> q; // {y, x, keystate} 
        q.push({stY, stX, 0});
        openSet[stY][stX][0] = true;

        int lv = 0, ans = INT_MAX;
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                VI &expander = q.front();
                int y = expander[0], x = expander[1], s = expander[2];
                openSet[y][x][s] = false;
                closedSet[y][x][s] = true;
                if (__builtin_popcount(s) == keyCnt) {
                    ans = min(ans, lv);
                } else {
                    for (int i = 0; i < 4; ++i) {
                        int ny = y+dy[i], nx = x+dx[i];
                        if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                        if (grid[ny][nx] == '#') continue;
                        if ('A' <= grid[ny][nx] && grid[ny][nx] <= 'F') {
                            // A lock
                            int keyIdx = grid[ny][nx] - 'A';
                            if ((s & (1 << keyIdx)) == 0) continue;
                        }
                        int ns = s;
                        if ('a' <= grid[ny][nx] && grid[ny][nx] <= 'z') {
                            // A key
                            int keyIdx = grid[ny][nx] - 'a';
                            ns |= (1 << keyIdx);
                        }
                        if (openSet[ny][nx][ns] || closedSet[ny][nx][ns]) continue;
                        q.push({ny, nx, ns});
                        openSet[ny][nx][ns] = true;
                    }
                }
                q.pop();
            }
            ++lv;
        }

        return INT_MAX == ans ? INVALID : ans;
    }
};
