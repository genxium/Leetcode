bool debug = false;
int const MAXK = 9;
int const MAXC = 10;
int const INVALID = -1;
/*
dp[p][c][boundedByNs] = the count of distinct-digit-suffixes at position "p", i.e. a suffix string of length "ns.length - p" using free digits indicated by mask "c"; if true == boundedByNs, we could only select digits <= ns[p] at position "p".
*/
int dp[MAXK][(1 << MAXC)][MAXC];
bool seen[MAXC];

string maskbits(int c) {
    string res = "";
    for (int i = 0; i < 10; ++i) {
        res.push_back(((1 << i) & c) ? '1' : '0');
    }
    return res;
}

/*
test cases
1
20
100
1000
23
1234
54321
56789
5667
5645899
566788
*/
class Solution {
public:
    int dfs(int p, int c, bool boundedByNs, string& ns, int lv) {
        if (INVALID != dp[p][c][boundedByNs]) return dp[p][c][boundedByNs];
        if (p == ns.length()) return 0;
        int i = ns[p]-'0'; // the reference digit
        bool isFirstDigit = (__builtin_popcount(c) == 10); // not first digit w.r.t. (0 == p), should check by the intactness of mask "c"
        int lteMask = (1 << i) + ((1 << i) - 1);
        int availableMask = boundedByNs
                        ?
                        (c & lteMask) // if bounded by "i", don't try digits other than (c & lteMask)
                        :
                        c
                        ;
        if (p == ns.length()-1) {
            if (isFirstDigit && ((1 << 0) & availableMask)) return __builtin_popcount(availableMask ^ (1 << 0)); // exclude 0
            else return __builtin_popcount(availableMask);
        }
        int indent = lv*4;
        if (debug) printf("%*sCalcing dp[p:%d][c:%s][boundedByNs:%d] with availableMask = %s\n", indent, "", p, maskbits(c).c_str(), boundedByNs, maskbits(availableMask).c_str());
        int res = 0;
        for (int j = 0; j < 10; ++j) {
            if (!((1 << j) & availableMask)) continue;
            int jMask = (1 << j);
            if (0 == j && isFirstDigit) {
                // move on without reducing "jMask"
                if (debug) printf("%*sMoving on as first digit at p=%d\n", indent, "", p);
                res += dfs(p+1, c, false, ns, lv+1);
            } else {
                bool shouldContinueBounding = (boundedByNs && i == j);
                if (debug) printf("%*sUsing j=%d, shouldContinueBounding=%d\n", indent, "", j, shouldContinueBounding);
                res += dfs(p+1, (c^jMask), shouldContinueBounding, ns, lv+1);
            }
        }
        if (debug) printf("%*sGot dp[p:%d][c:%s][boundedByNs:%d] = %d\n", indent, "", p, maskbits(c).c_str(), boundedByNs, res);
        return dp[p][c][boundedByNs] = res;
    }
        
    int numDupDigitsAtMostN(int n) {
        memset(dp, INVALID, sizeof dp);
        memset(seen, false, sizeof seen);
        string ns = to_string(n);
        int stillDistinctEnd = 0;
        for (char ch : ns) {
            ++stillDistinctEnd;
            if (seen[ch-'0']) break; // it's important to break AFTER the first duplicate digit, not before -- see example "5667"
            seen[ch-'0'] = true;
        }

        int c = (1 << MAXC)-1;
        int pans = dfs(0, c, true, ns, 0); // pans means "parity of ans", which is the count of numbers with all distinct digits less than n
        // by now pans still needs add counting of all distincts under 1*pow(10, k0)
        int ans = n-pans;
        return ans;
    }
};
