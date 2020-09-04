/*
In general, fitting p different digits into q slots results in 
1. 0 ways if "p < q"
2. p! ways if "p == q"
3. A(p, q) == p!/(p-q)! ways if "p > q".

In this problem p == 10, i.e. [0, 1, 2, .., 9] and "q <= 8" thus case#3 always applies.

However, we have to exclude the cases of preceding zeros (except for q == 1), thus should reduce the amount where we have 1 leading 0, e.g. A(p-1, q-1).

*/

#define MAXP 11
#define MAXQ 9
#define INVALID -1L
#define LL int64_t

class Solution {
private:
    LL cacheA[MAXP][MAXQ], cacheFactorial[MAXP];
    LL factorial(int p) {
        if (INVALID != cacheFactorial[p]) {
            return cacheFactorial[p];
        }
        if (1 >= p) return (cacheFactorial[p] = 1);
        return (cacheFactorial[p] = p*factorial(p-1));
    }
    
    LL A(int p , int q) {
        if (INVALID != cacheA[p][q]) {
            return cacheA[p][q];
        }
        if (p == q) {
            return cacheA[p][q] = factorial(p);
        }
        LL denominator = factorial(p-q);
        LL numerator = factorial(p);
        return cacheA[p][q] = numerator/denominator;
    }
    
public:
    int countNumbersWithUniqueDigits(int n) {
        memset(cacheA, INVALID, sizeof(cacheA));
        memset(cacheFactorial, INVALID, sizeof(cacheFactorial));
        int p = 10;
        LL ans = 0;
        for (int q = 0; q <= n; ++q) {
            switch (q) {
                case 0:
                    ans += 1; // only x == 0 fits
                    break;
                case 1:
                    ans += 9; // the case where x == 0 is included in "q == 0"
                    break;
                default:
                    ans += A(p, q);
                    ans -= A(p-1, q-1);
                    break;
            }
        }
        return ans;
    }
};
