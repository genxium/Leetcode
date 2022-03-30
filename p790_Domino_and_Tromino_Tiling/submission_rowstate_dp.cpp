bool debug = true;
int const MOD = 1000000007;
int const MAX_STATE = 4; // 00, 01, 10, 11
int dp[2][MAX_STATE]; // In fact "dp[rowId][state]", but a rotation trick is used here to reduce space-complexity.

// [TODO] Use "matrix power" to speed up the transfer (which seems an overkill for this problem).

/*
test cases
1
2
3
4
5
10
100
500
1000
*/
class Solution {
public:
    int numTilings(int N) {
        /*
        [WARNING]
        
        The solution here views the board as Nx2 instead and puts tiles row by row.
        */
        memset(dp, 0, sizeof dp);
        int PRE = 1, CUR = 0;
        
        // init the 0-th row
        /*
        xx
        
        or 
        
        __
        */
        dp[CUR][0] = 1;
        dp[CUR][3] = 1;
        swap(PRE, CUR); // Moves down by 1 row.
        
        // loop
        for (int i = 1; i < N; ++i) {
            /*
            xx <- PRE, put nothing
            __
            */
            dp[CUR][0] = dp[PRE][3]; // [WARNING] Putting a bar on "dp[PRE][0]" only makes "dp[PRE][3]".
            /* 
            x_ <- PRE, put a bar vertically
            __
            
            __ <- PRE, put an L
            __
            */
            dp[CUR][1] = (dp[PRE][2] + dp[PRE][0])%MOD;
            /* 
            _x <- PRE, put a bar vertically
            __
            
            __ <- PRE, put an L
            __
            */
            dp[CUR][2] = (dp[PRE][1] + dp[PRE][0])%MOD;
            /*
            xx <- PRE, put a bar
            __
            
            x_ <- PRE, put an L
            __
            
            _x <- PRE, put an L
            __
            
            __ <- PRE, put 2 bars vertically
            __
            */
            dp[CUR][3] = (((dp[PRE][3] + dp[PRE][2])%MOD + dp[PRE][1])%MOD + dp[PRE][0])%MOD;
            swap(PRE, CUR); // Moves down by 1 row. 
        }
        
        return dp[PRE][3];
    }
};
