bool const debug = false;
typedef uint64_t LL;
LL const LLMAX = UINT64_MAX;
LL const MOD = 1000000007;

LL gcd(LL x, LL y) {
    // Guaranteed x > y
    if (y == 0) return x;
    return gcd(y, x%y);
}

/*
test cases
1
2
3
4
2
3
5
2
4
3
6
4
1000000000
40000
40000
999999999
38779
40000
*/
class Solution {
public:
    int nthMagicalNumber(int n, int a, int b) {
        if (a < b) swap(a, b);
        LL g = gcd(a, b);
        LL m = (LL)a/g * b;
        /*
        n*max(a,b) <= 4*(10^13) < 2^64
        */
        LL l = 1, r = LLMAX-1;
        while (l < r) {
            LL cand = ((l+r) >> 1);
            if (debug) printf("cand is %lu\n", cand);
            LL cnt1 = cand/a;
            LL cnt2 = cand/b;
            LL cnt3 = cand/m;
            LL cnt = cnt1+cnt2-cnt3;
            if (cnt == n) {
                if ((cand % a == 0) || (cand % b == 0)) return cand%MOD;
                else r = cand-1; // "cand" too big
            } else if (cnt > n) {
                r = cand-1;
            } else {
                l = cand+1;
            }
        }

        return l%MOD;
    }
};
