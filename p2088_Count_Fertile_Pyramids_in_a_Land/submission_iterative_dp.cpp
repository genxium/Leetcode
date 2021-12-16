bool const debug = false;

class Solution {
public:
    int countPyramids(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(); 
        int maxh = min(m, n), maxpos = m*n;
        if (debug) printf("maxh: %d, maxpos: %d\n", maxh, maxpos);

        vector<int> dp1(maxpos, 0); // dp1[pos]: max height of a pyramid ended at "pos" as the bottom-right
        vector<int> dp2(maxpos, 0); // dp2[pos]: max height of an inverse pyramid ended at "pos" as the top-right

        vector<vector<int>> consOnes(m, vector<int>(n, 0)); // consecutive 1s by "grid[y][x]" in row "grid[y]" 
        for (int y = 0; y < m; ++y) {
            for (int x = 0; x < n; ++x) {
                if (1 == grid[y][x]) consOnes[y][x] = 1;
                if (x > 0 && 1 == consOnes[y][x]) consOnes[y][x] += consOnes[y][x-1]; 
            }
        }

        int ans = 0;
        for (int y = 0; y < m; ++y) {
            for (int x = 0; x < n; ++x) {
                if (0 == grid[y][x]) continue;
                int pos = y*n + x;
                dp1[pos] = 1; // at least 1 by now
                if (y < 1 || x < 1) continue;
                int candPos1 = (y-1)*n + (x-1);
                int hCand1 = dp1[candPos1]; 
                int hCand2 = (consOnes[y][x]+1)/2 - 1;
                int prevh = min(hCand1, hCand2); // The largest height this "pos" can achieve is determined by both the largest height of "candPos1", as well as the number of consecutive ones by far in this row -- the smaller dominates.
                dp1[pos] = 1+prevh;
                if (debug) printf("y: %d, x: %d, dp1 = %d\n", y, x, dp1[pos]);
                ans += (dp1[pos]-1); // this many pyramids end at this "pos" as bottom-right
            }
        }

        for (int y = m-1; y >= 0; --y) {
            for (int x = 0; x < n; ++x) {
                if (0 == grid[y][x]) continue;
                int pos = y*n + x;
                dp2[pos] = 1; // at least 1 by now
                if (y+1 >= m || x < 1) continue;
                int candPos1 = (y+1)*n + (x-1);
                int hCand1 = dp2[candPos1]; 
                int hCand2 = (consOnes[y][x]+1)/2 - 1;
                int prevh = min(hCand1, hCand2);
                dp2[pos] = 1+prevh;
                if (debug) printf("y: %d, x: %d, dp2 = %d\n", y, x, dp2[pos]);
                ans += (dp2[pos]-1);
            }
        }
        
        return ans;
    }
};
