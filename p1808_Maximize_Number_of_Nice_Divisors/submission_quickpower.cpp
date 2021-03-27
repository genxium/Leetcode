bool debug = false;
typedef uint64_t ULL;
ULL const MOD = 1000000007;

ULL multiplyMod(ULL lhs, ULL rhs) {
    return (lhs*rhs)%MOD;
}

ULL myPow(ULL x, ULL n) {
    // See https://github.com/genxium/Leetcode/tree/master/p50_Pow_x_n for an explanation of the custom integer power implementation.

    if (1 == n || 0 == x || 1 == x) return x; 
    if (0 == n) return 1;
    int const INTEGER_BIT_LENGTH = sizeof(int)*CHAR_BIT;
    int nBinaryLengthUpper = 0;
    while (nBinaryLengthUpper < INTEGER_BIT_LENGTH && (1 << nBinaryLengthUpper) <= n) ++nBinaryLengthUpper;

    ULL ans = 1;
    for (int i = nBinaryLengthUpper-1; i >= 0; --i) {
        ans = multiplyMod(ans, ans);
        if (((1 << i) & n) > 0) {
            ans = multiplyMod(ans, x);
        }
    }

    return ans;
}

/*
test cases
5
8
6
13
23
73
*/
class Solution {
public:
    ULL calcCandidate(int qSumUpper, int t) {
        int qAvg = qSumUpper/t;
        int qRemained = qSumUpper%t;
        ULL ret = myPow(qAvg, t-qRemained);
        ret = multiplyMod(ret, myPow(qAvg+1, qRemained));
        return ret;
    }

    int maxNiceDivisors(int primeFactors) {
        // Assumes that "n" is SUM{pow(p[i], q[i])}, then the number of nice divisors is PROD{q[i]} where the constraint is "SUM{q[i]} <= primeFactors"
        ULL qSumUpper = primeFactors;  
        // Assumes that "n" has "t" different prime factors, we can check the first several to see that "cand" is max when "t == ceil((double)qSumUpper/3)".
        /*
        for (int t = 1; t <= qSumUpper; ++t) {
            ULL cand = calcCandidate(qSumUpper, t);
            if (debug) printf("t:%d, cand:%llu\n", t, cand);
            ans = max(ans, cand);
        }
        */

        int tBest = (int)ceil((double)qSumUpper/3);
        ULL ans = calcCandidate(qSumUpper, tBest);
        return ans;
    }
};
