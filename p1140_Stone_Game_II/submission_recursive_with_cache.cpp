/*
Note that in this problem, "stoneValue" cannot be re-ordered.

The solution here is very similar to 
- that of https://github.com/genxium/Leetcode/tree/master/p1406_Stone_Game_III.
*/
#define INVALID -1
#define MAXN 100
int cache[MAXN+1][MAXN+1]; // "cache[i][m] == y" means that by being able to pick the first "piles[i, ..., i+2*m-1]" of "piles[i, ..., n-1]", Alex can get at most "y" points.   
int ps[MAXN]; // "PrefixSum"

class Solution {
public:
    int dfs(int i, int m, int n) {
        if (INVALID != cache[i][m]) {
            return cache[i][m];
        }
        if (i == n) {
            return 0;
        }
        if (i == n-1) {
            // regardless of m
            cache[i][m] = ps[n-1]-(n-2 >= 0 ? ps[n-2] : 0);
            return cache[i][m];
        }
        int tmp = 0;
        int xUpper = min((m << 1), n-i);
        for (int x = 1; x <= xUpper; ++x) {
            int subarrSum = ps[n-1]-(i+x > 0 ? ps[i+x-1] : 0);
            int nextM = max(m, x);
            int optimalForOponent = dfs(i+x, nextM, n); // Assumes that Lee plays optimally hereafter.
            int takingStonesFromFront = ps[i+x-1] - (i > 0 ? ps[i-1] : 0); 
            int candidate = takingStonesFromFront + (subarrSum - optimalForOponent);
            tmp = max(candidate, tmp);
        }
        cache[i][m] = tmp;
        return cache[i][m];
    }
    
    int stoneGameII(vector<int>& piles) {
        memset(cache, INVALID, sizeof(cache));
        memset(ps, 0, sizeof(ps));
        
        int n = piles.size();
        for (int i = 0; i < n; ++i) {
            if (i == 0) ps[i] = piles[i];
            else ps[i] = ps[i-1]+piles[i];
        }
        
        int ans = dfs(0, 1, n);
        return ans;
    }
};
