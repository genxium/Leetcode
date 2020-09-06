/*
Note that in this problem, "stoneValue" cannot be re-ordered.

The solution here is very similar to 
- that of https://github.com/genxium/Leetcode/tree/master/p1140_Stone_Game_II.
*/
#define INVALID INT_MIN
#define MAXN 50000
int cache[MAXN+1]; // "cache[i] == y" means that by being able to pick the first 3 of "piles[i, ..., n-1]", Alex can get at most "y" points.
int ps[MAXN]; // "PrefixSum"

class Solution {
public:
    int dfs(int i, int n) {
        if (INVALID != cache[i]) {
            return cache[i];
        }
        if (i == n-1) {
            cache[i] = ps[n-1]-(n-2 >= 0 ? ps[n-2] : 0);
            return cache[i];
        }
        int xUpper = min(3, n-i);
        int tmp = INT_MIN;
        for (int x = 1; x <= xUpper; ++x) {
            int subarrSum = ps[n-1]-(i+x > 0 ? ps[i+x-1] : 0);
            int optimalForOponent = dfs(i+x, n); // Assumes that Lee plays optimally hereafter.
            int takingStonesFromFront = ps[i+x-1] - (i > 0 ? ps[i-1] : 0); 
            int candidate = takingStonesFromFront + (subarrSum - optimalForOponent);
            tmp = max(candidate, tmp);
        }
        cache[i] = tmp;
        return cache[i];
    }
    
    string stoneGameIII(vector<int>& piles) {
        memset(ps, 0, sizeof(ps));
        
        int n = piles.size();
        for (int i = 0; i < n; ++i) {
            cache[i] = INVALID;
        }
        
        for (int i = 0; i < n; ++i) {
            if (i == 0) ps[i] = piles[i];
            else ps[i] = ps[i-1]+piles[i];
        }
        
        int optimalForAlice = dfs(0, n);
        int optimalForBob = ps[n-1]-optimalForAlice;
        //printf("Optimal for Alice is %d, optimal for bob is %d\n", optimalForAlice, optimalForBob);
        
        if (optimalForAlice > optimalForBob) {
            return "Alice";
        } else if (optimalForAlice < optimalForBob) {
            return "Bob";
        } else {
            return "Tie";   
        }
    }
};
