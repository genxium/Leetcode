bool const debug = true;
int const INVALID = -1;
int const MAXK = 100;
int const MAXN = 10000;
int dp[MAXK+1][MAXN+1]; // dp[k][n] is the minimum #moves to find "f" when having "k" eggs and "n" remaining floors

/*
test cases
1
2
2
6
3
14
2
2
*/
class Solution {
public:
    int superEggDrop(int k, int n) {
        memset(dp, INVALID, sizeof dp);
        return dfs(k, n);
    }
    
    int dfs(int k, int n) {
        if (0 >= n) return 0;
        
        if (1 == k) {
            // Having 1 egg, can only traverse all floors from the lowest 
            return dp[k][n] = n;
        }
        
        if (INVALID != dp[k][n]) return dp[k][n];
        
        /*
        dp[k][n] = min{ 
                        max{
                            dp[k-1][i-1], // breaks 
                            dp[k][n-i] // doesn't break
                        } 
                   }
                   where "1 <= i <= n"
        
        Note that "dp[k][n]" is monotonically increasing w.r.t. "n", thus "max{dp[k-1][i-1], dp[k][n-i]}" will start decreasing then turn increasing w.r.t. "i". 
        */
        
        // Binary search for the proper "i". 
        int ret = INT_MAX;
        int l = 1, r = n;
        while (l <= r) {
            int i = (l+r)/2;
            int cand1 = dfs(k-1, i-1);
            int cand2 = dfs(k, n-i);
            
            if (cand1 == cand2) {
                ret = min(ret, 1+max(cand1, cand2));
                break;
            }
            if (cand1 > cand2) r = i-1; // cand1 too big -> i is too big
            else l = i+1;
        }
        
        if (l > r) {
            // Couldn't find a single point of minimum, now should have "l == r+1" where "r" is the last visited "i"
            int cand1 = dfs(k-1, l-1);
            int cand2 = dfs(k, n-l);
            ret = min(ret, 1+max(cand1, cand2));
        }
        
        return dp[k][n] = ret;
    }
};
