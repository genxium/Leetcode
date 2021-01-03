bool const debug = false;
vector<int> dy = {+1, -1, 0, 0};
vector<int> dx = {0, 0, +1, -1};

int const MAXL = 50;
int const MAXSTEPS = 50;
int const OFFSET = 2;
int const MOD = 1000000007;
int dp[MAXSTEPS+1][MAXL+OFFSET+1][MAXL+OFFSET+1];

/*
test cases
2
2
2
0
0
1
3
3
0
1
1
2
50
0
0
8
50
23
5
26
*/
class Solution {
public:
    int findPaths(int m, int n, int N, int i, int j) {
        // init
        memset(dp, 0, sizeof dp);
        dp[0][i+OFFSET][j+OFFSET] = 1;
        int ans = 0;
        
        // loop
        for (int steps = 1; steps <= N; ++steps) {
            for (int y = -1; y <= m; ++y) {
                for (int x = -1; x <= n; ++x) {
                    for (int i = 0; i < 4; ++i) {
                        int ny = y+dy[i];
                        int nx = x+dx[i];
                        if (ny < 0 || ny >= m || nx < 0 || nx >= n) continue;
                        // if (dp[steps][y+OFFSET][x+OFFSET]) continue;
                        dp[steps][y+OFFSET][x+OFFSET] += dp[steps-1][ny+OFFSET][nx+OFFSET];    
                        dp[steps][y+OFFSET][x+OFFSET] %= MOD;
                        if (debug) printf("dp[steps:%d][y:%d][x:%d] == %d, deduced from dp[steps-1:%d][ny:%d][nx:%d] == %d\n", steps, y, x, dp[steps][y+OFFSET][x+OFFSET], steps-1, ny, nx, dp[steps-1][ny+OFFSET][nx+OFFSET]);
                        if (dp[steps][y+OFFSET][x+OFFSET] && (y == -1 || x == -1 || y == m || x == n)) {
                            ans += dp[steps][y+OFFSET][x+OFFSET];
                            ans %= MOD;
                        }
                    }
                }
            }
        }
        
        return ans;
    }
};
