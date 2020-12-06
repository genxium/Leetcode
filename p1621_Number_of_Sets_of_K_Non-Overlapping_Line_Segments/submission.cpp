typedef uint64_t LL;
bool const debug = false;
LL const INVALID = 0u;
LL const MOD = 1000000007u;

int const MAXN = 1000;
int const MAXK = MAXN-1;

LL C[MAXN+MAXN][MAXN]; // C[m][p] equals the number of ways to take p segments from m distinct ones, regardless of order
LL combination(int m, int p) {
  if (m < 0 || p < 0) return 0;
  if (INVALID != C[m][p]) return C[m][p];
  if (p == m) return C[m][p] = 1; // including {m: 0, p:0}
  if (p < m) {
    return C[m][p] = (combination(m-1, p-1) + combination(m-1, p))%MOD;
  }
  return INVALID;
}

LL nonNegativeSolutionsCount(int k, int r) {
    /*
    The number of "NonNegativeSolution"s of equation "y[0] + ... + y[k-1] == r".
    
    See "Counting.pdf" for an explanation.
    */
    return combination(k+r-1, r);
}

LL positiveSolutionsCount(int k, int p) {
    /*
    The number of "PositiveSolution"s of equation "x[0] + ... + x[k-1] == p".
    
    The answer is equal to count of non-negative solutions to "y[0] + ... + y[k-1] == (p-k)", which is C((p-k)+k-1, k). See "Counting.pdf" for an explanation.
    */
    return combination(p-1, p-k);
    
    /*
    // An alternative recursive approach is shown below, which also applies to counting non-negative solutions if preferred. 
    
    if (k > p) return INVALID;
    if (INVALID != pscCache[k][p]) return pscCache[k][p];
    if (1== k || k == p) return pscCache[k][p] = 1;
    
    LL count = 0L, x0Max = p-(k-1); // x[0] reaches its max value when "x[1] == x[2] == ... == x[k-1] == 1" 
    for (int x0 = 1; x0 <= x0Max; ++x0) {
        count += positiveSolutionsCount(k-1, p-x0);
        count %= MOD;
    }
    return (pscCache[k][p] = count);
    */
}

/*
test cases
6
3
9
3
4
2
3
1
30
7
5
3
3
2
*/
class Solution {
public:
    int numberOfSets(int n, int k) {
        /*
        It's easier to imagine we're choosing from "n-1" segments instead.

        The problem then becomes for each "p" such that "k <= p <= n-1", count all "PositiveSolution"s such that "x[0] + ... + x[k-1] == p". Note that for each "p" there're also "C(n-1, p)" different "sets of segments".      
        */
        memset(C, INVALID, sizeof(C));
        LL ans = 0;
        for (int p = k; p <= n-1; ++p) {
            int notChosenSegsSlotsCount = k+1; 
            int notChosenSegsCount = n-1-p;
            LL nnsc = nonNegativeSolutionsCount(notChosenSegsSlotsCount, notChosenSegsCount);
            LL psc = positiveSolutionsCount(k, p); 
            if (debug) printf("nonNegativeSolutionsCount(notChosenSegsSlotsCount:%d, notChosenSegsCount:%d) == %lu, positiveSolutionsCount(k:%d, p:%d) == %lu\n", notChosenSegsSlotsCount, notChosenSegsCount, nnsc, k, p, psc);
            ans += nnsc*psc;
            ans %= MOD;
        }
        return ans;
    }
};
