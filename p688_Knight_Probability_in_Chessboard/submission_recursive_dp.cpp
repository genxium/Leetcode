double const INVALID = -1.0;
int const MAXN = 25;
int const MAXK = 100;
double cache[MAXN][MAXN][MAXK+1];

int const dy[] = {+2, +2, -2, -2, +1, +1, -1, -1};
int const dx[] = {+1, -1, +1, -1, +2, -2, +2, -2};

class Solution {
private:
    double dfs(int y, int x, int N, int k) {
        if (INVALID != cache[y][x][k]) return cache[y][x][k];
        if (k == 0) return cache[y][x][k] = 1.0;
        double res = 0.0;
        for (int i = 0; i < 8; ++i) {
            int ny = y+dy[i], nx = x+dx[i];
            if (ny >= N || ny < 0 || nx >= N || nx < 0) continue;
            res += dfs(ny, nx, N, k-1);
        }
        
        return (cache[y][x][k] = res/8);
    }
    
public:
    double knightProbability(int N, int K, int r, int c) {
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                for (int k = 0; k <= K; ++k) {
                    cache[y][x][k] = INVALID;
                }
            }
        }
        return dfs(r, c, N, K);
    }
};
