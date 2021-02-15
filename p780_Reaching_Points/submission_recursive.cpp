/*
test cases
1
1
3
5
1
1
2
2
9
5
12
8
1
1
1
1000000000
1
16
999999985
16
1
5
999999996
5
1
9
1000000000
9
3
7
3
4
*/
class Solution {
public:
    int gcd(int a, int b) {
        if (a > b) swap(a, b);
        if (0 == a) return b;
        return gcd(b%a, a);
    }
    
    bool solve(int sx, int sy, int tx, int ty) {
        if (tx == sx && ty == sy) return true;
        if (tx == ty) {
            return (sx == tx && sy == ty);
        }
        if (tx < sx || ty < sy) return false;
        if (tx > ty) {
            // tx is definitely from (tx-ty, ty)
            return solve(sx, sy, tx-ty, ty);            
        } else {
            return solve(sx, sy, tx, ty-tx);
        }
    }

    bool reachingPoints(int sx, int sy, int tx, int ty) {
        /*
        For any (tx, ty), without loss of generality, assume "tx <= ty". If "tx == ty", it can only come from "(tx, 0)" which is impossible except that "(sx, sy) == (tx, ty)" from the very beginning.
        
        When "tx < ty", it can only come from "(ty-tx, tx)", and will ultimately come from "(k*d, k*d)" where "d = gcd(tx, ty)".
        
        Another fact to notice is that the path back from (tx, ty) is definitive, which leads to an intuition of recursing.  
        */
        
        int d = gcd(tx, ty);
        if (sx%d != 0 || sy%d != 0) {
            // Trick#1
            return false;
        }
        
        if (tx > ty) {
            if (tx % ty == 0) {
                // Trick#2, in this case we can only iteratively reduce "tx" to "k*ty" while keeping "ty" still.
                return (sy == ty && (sx % ty == 0 && sx <= tx));
            }
            
            // tx is definitely from (tx-ty, ty)
            // Trick#3, reduce the larger of "(tx, ty)" before recursion
            int lowTarget = max(sx, ty);
            int u = (tx-lowTarget)/ty; // reduce tx to "tx - u*ty >= lowTarget"
            int newTx = tx-u*ty;
            if (newTx > tx) return false;
            return solve(sx, sy, newTx, ty);            
        } else {
            // ty >= tx
            if (ty % tx == 0) {
                return (sx == tx && (sy % tx == 0 && sy <= ty));
            }
            int lowTarget = max(sy, tx);
            int u = (ty-lowTarget)/tx;
            int newTy = ty-u*tx;
            if (newTy > ty) return false;
            return solve(sx, sy, tx, newTy);
        }
    }
};
