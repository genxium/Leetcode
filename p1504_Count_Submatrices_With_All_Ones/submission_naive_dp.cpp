/*
Strawman solution

l[y][x] max extend to left on the same row
t[y][x] max extend to top on the same column

dp[y][x][h][w] true if there a all-1-matrices whose bottomright is (y, x) with size h*w
localSum[y][x] the number of all-1-matrices whose bottomright is (y, x)

l[y][x]:
- if (0 == mat[y][x]) return 0
- return (1 == mat[y][x-1] ? l[y][x-1] : x)

t[y][x]:
- if (0 == mat[y][x]) return 0
- return (1 == mat[y-1][x] ? t[y-1][x] : y)

At [y][x]; 
  - compute t[y][x], l[y][x]
  - for h: t[y][x], y; for w: l[y][x], x; 
    - dp[y][x][h][w]:
      - if (0 == mat[y][x]) return false
      - if (!dp[y-1][x][h-1][w]) return false
      - if (!dp[y][x-1][h][w-1]) return false
      - return true 
*/
#define MAXN 150
#define INVALID -1
int dp[MAXN][MAXN][MAXN]; // "dp[y][x][h] == u" means that "u" is the leftmost position on the y-th row where a rectangle of height "h" exists with a bottom edge [u, x]

class Solution {
public:
    int numSubmat(vector<vector<int>>& arr) {
        memset(dp, INVALID, sizeof(dp));  
  
        int ans = 0; 
        int yUpper = arr.size();
        int xUpper = arr[0].size();
        
        dp[0][0][1] = (arr[0][0] == 1 ? 1 : INVALID);
            
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                //printf("Checking arr[y:%d][x:%d] == %d\n", y, x, arr[y][x]);

                if (0 == arr[y][x]) continue;
                for (int h = 1; h <= y+1; ++h) {
                    if (0 == arr[y-h+1][x]) {
                        break; // larger "h" for this "(y, x)" must also have "dp[y][x][h] == INVALID"
                    }

                    // We're having "1 == arr[(y-h+1), ..., y][x]" by now.
                    if (x == 0) {
                        dp[y][x][h] = x;
                        ans += 1; // adds 1 by the "rectangle bottomed at (y, x) with height h, width 1"
                        //printf("\tupdated dp[y:%d][x:%d][h:%d] = %d, ans = %d\n", y, x, h, dp[y][x][h], ans);
                    } else {
                        if (INVALID == dp[y][x-1][h]) {
                            dp[y][x][h] = x;
                            ans += 1; // adds 1 by the "rectangle bottomed at (y, x) with height h, width 1"
                            //printf("\tcut updating along `h` at [y:%d][x:%d][h:%d], because dp[y:%d][x-1:%d][h:%d] = %d, ans = %d\n", y, x, h, y, x-1, h, dp[y][x-1][h], ans);
                            continue; // don't break here, because we oaught to traverse larger "h" and increment "ans" by each "1 == arr[(y-h+1), ..., y][x]"
                        }
                        dp[y][x][h] = dp[y][x-1][h];
                        ans += (x - dp[y][x][h] + 1); // adds 1 by the "rectangle bottomed at (y, x) with height h, width [1, 2, ..., (x-dp[y][x][h]+1)]"   
                        //printf("\tupdated dp[y:%d][x:%d][h:%d] = %d, ans = %d\n", y, x, h, dp[y][x][h], ans);
                    }
                }
            }
        }
        
        return ans; 
    }
};
