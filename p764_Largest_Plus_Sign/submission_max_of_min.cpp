bool debug = false;
int const MAXN = 500;
bool isMine[MAXN][MAXN];

int up[MAXN][MAXN], down[MAXN][MAXN], leftt[MAXN][MAXN], rightt[MAXN][MAXN]; // up[y][x] is the length of continuous 1's to go up from (y,x)

int dy[] = {0, 0, -1, 1};
int dx[] = {1, -1, 0, 0};

/*
test cases
5
[[4,2]]
2
[]
1
[[0,0]]
*/
class Solution {
public:
    int orderOfLargestPlusSign(int N, vector<vector<int>>& mines) {
        memset(isMine, false, sizeof isMine);
        memset(up, 0, sizeof up);
        memset(down, 0, sizeof down);
        memset(leftt, 0, sizeof leftt);
        memset(rightt, 0, sizeof rightt);
        
        for (auto &mine : mines) isMine[mine[0]][mine[1]] = true;
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                if (isMine[y][x]) continue;
                up[y][x] = leftt[y][x] = 1;
                if (y-1 >= 0) up[y][x] += up[y-1][x];
                if (x-1 >= 0) leftt[y][x] += leftt[y][x-1];
            }
        }
        
        for (int y = N-1; y >= 0; --y) {
            for (int x = N-1; x >= 0; --x) {
                if (isMine[y][x]) continue;
                down[y][x] = rightt[y][x] = 1;
                if (y+1 < N) down[y][x] += down[y+1][x];
                if (x+1 < N) rightt[y][x] += rightt[y][x+1];
            }
        }
        
        int ans = 0;
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                int order = min(min(up[y][x], down[y][x]), min(leftt[y][x], rightt[y][x]));
                ans = max(ans, order);
            }
        }
        
        return ans;
    }
};
