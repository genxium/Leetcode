bool const debug = true;
int const MAXN = 1000, MAXK = 1000;
int const MOD = 1000000007;

struct FenwickTree {
    vector<int> bit;  // binary indexed tree
    int n;

    FenwickTree(int n) {
        this->n = n;
        bit.assign(n, 0);
    }

    int sum(int r) {
        int ret = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1) 
            ret = (ret + bit[r]) % MOD;
        return ret;
    }

    int sum(int l, int r) {
        if (l == 0) return sum(r);
        else return (sum(r) - sum(l-1) + MOD)%MOD;
    }

    void add(int idx, int delta) {
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] = (bit[idx] + delta) % MOD;
    }
};

/*
int dp[MAXN+1][MAXK+1]; // "dp[i][c] == x" means that using [1, ..., i], there're "x" arrays of "c" inverse-pairs

dp[i][c] = sum{dp[i-1][c-j] | 0 <= j < i-1}, for each permutation of [1, ..., i-1] of "t" inverse-pairs, adding number "i" to the "i" slots can make new arrays of "t+i-1", "t+i-2", ..., "t" inverse-pairs, respectively

 1 2 ... i-2 i-1
^ ^ ^   ^   ^   ^
*/

/*
test cases
3
0
3
1
5
2
5
3
5
5
5
6
1000
500
1000
1000
1
0
*/
class Solution {
public:
    int solveBruteforce(int n, int k) {
        int dp[MAXK+1];

        // init 
        memset(dp, 0, sizeof dp);
        dp[0] = 1; // as "dp[2][0] = 1", for [1, 2]
        dp[1] = 1; // as "dp[2][1] = 1", for [2, 1]
        
        // loop 
        for (int i = 3; i <= n; ++i) {
            for (int c = k; c >= 0; --c) {
                if (0 == c) dp[c] = 1; 
                else {
                    int sum = 0;
                    int jUpper = min(c, i-1);
                    for (int j = 0; j <= jUpper; ++j) {
                        sum += dp[c-j];
                        sum %= MOD;
                    } 
                    dp[c] = sum;
                }
            }
        }
        
        return dp[k];
    }
    
    int solveFenwickTree(int n, int k) {
        
        FenwickTree ftIns(k+1);
        
        // init
        ftIns.add(0, 1); // as "dp[2][0] = 1", for [1, 2]
        ftIns.add(1, 1); // as "dp[2][1] = 1", for [2, 1]
       
        for (int i = 3; i <= n; ++i) {
            for (int c = k; c >= 0; --c) {
                if (0 == c) continue;
                else {
                    int jUpper = min(c, i-1);
                    int sum = ftIns.sum(c-jUpper, c);
                    int oldVal = ftIns.sum(c, c);
                    int delta = (sum-oldVal+MOD)%MOD;
                    ftIns.add(c, delta);
                }
            }
        }
        
        return ftIns.sum(k, k);
    }
    
    int kInversePairs(int n, int k) {
        if (1 == n) {
            if (0 == k) return 1;
            else return 0;
        }

        return solveFenwickTree(n, k);
    }
};
