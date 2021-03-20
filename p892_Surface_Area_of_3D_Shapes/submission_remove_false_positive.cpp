int dy[] = {+1, -1, 0, 0};
int dx[] = {0, 0, +1, -1};

/*
test cases
[[2]]
[[1,2],[3,4]]
[[1,0],[0,2]]
[[1,1,1],[1,0,1],[1,1,1]]
[[2,2,2],[2,1,2],[2,2,2]]
*/
class Solution {
public:
    int surfaceArea(vector<vector<int>>& grid) {
        int n = grid.size();
        int ans = 0;
        for (int y = 0; y < n; ++y) {
            for(int x = 0; x < n; ++x) {
                int h = grid[y][x];
                for (int i = 1; i <= h; ++i) {
                    ans += 6;
                    if (i < h) --ans; // top face glued
                    if (i > 1) --ans; // bottom face glued
                    for (int k = 0; k < 4; ++k) {
                        int ny = y+dy[k], nx = x+dx[k];
                        if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;
                        int nh = grid[ny][nx];
                        if (nh < i) continue;
                        --ans; // side face glued
                    }
                }
            }
        }
        
        return ans;
    }
};
