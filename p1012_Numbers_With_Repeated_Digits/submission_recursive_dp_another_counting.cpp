bool debug = false;
int const MAXK = 9;
int const MAXC = 10;
int const INVALID = -1;
/*
dp[i][k][c] = the count of (k+1)-slot-number within [0, i*pow(10, k)] using free digits indicated by mask "c" to make all slots distinct, ALLOWING "leading 0", e.g. dp[i:7][k:3][c:1110111111] counts "6012" but not "3012" because digit "3" is not free, also it counts "0621" but not "621".
*/
int dp[MAXC][MAXK][(1 << MAXC)];
int lt1p10[MAXK+1];

int comb[MAXC+1][MAXK+1];
int fac[MAXC+1];

string maskbits(int c) {
    string res = "";
    for (int i = 0; i < 10; ++i) {
        res.push_back(((1 << i) & c) ? '1' : '0');
    }
    return res;
}

int combination(int from, int to) {
    if (INVALID != comb[from][to]) return comb[from][to];
    if (from == to) return comb[from][to] = 1;
    if (1 == to) return comb[from][to] = from;
    if (0 == to) return comb[from][to] = 1;
    if (from < to) return comb[from][to] =  0;
    return combination(from-1, to) + combination(from-1, to-1);
}

int factorial(int u) {
    if (INVALID != fac[u]) return fac[u];
    if (1 >= u) return fac[u] = 1;
    return fac[u] = u*factorial(u-1);
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
    int dfs(int i, int k, int c, int k0, int lv) {
        if (INVALID != dp[i][k][c]) return dp[i][k][c];
        if (0 == i) return 0;
        int indent = lv*4;
        if (debug) printf("%*sCalcing dp[i:%d][k:%d][c:%s]\n", indent, "", i, k, maskbits(c).c_str());
        int res = 0;
        if (0 == k) {
            // count whichever smaller digits still usable
            int lteMask = (1 << i) + ((1 << i) - 1);
            int availableMask = (c & lteMask);
            bool zeroUsable = ((1 << 0) & availableMask);
            if (zeroUsable) {
                // It's already accounted in the previous "1 == k" calculation
                availableMask ^= (1 << 0);
            }
            if (debug) printf("%*s#0 Updating availableMask = %s\n", indent, "", maskbits(availableMask).c_str());
            res += __builtin_popcount(availableMask);
        } else {
            bool zeroUsable = ((1 << 0) & c);
            int ltMask = (1 << i) - 1;
            int restAvailableMask = (ltMask & c);
            if (restAvailableMask) {
                int j = (int) log2(restAvailableMask);
                bool isFirstDigit = (__builtin_popcount(c) == 10);
                if (debug) printf("%*sisFirstDigit=%d for [c:%s]\n", indent, "", isFirstDigit, maskbits(c).c_str());
                if (0 < j || !isFirstDigit) {
                    int add1 = dfs(j, k, c, k0, lv+1); // count the (k+1)-slot-distincts within [0, j*pow(10, k)] using mask "c"
                    res += add1;
                    if (debug) printf("%*s#1 Updating add1 = %d by j:%d\n", indent, "", add1, j);
                    int jMask = (1 << j);
                    int add2 = combination(__builtin_popcount(c^jMask), k)*factorial(k); // count also the k-slot-distincts within [0, pow(10, k)) using new mask "c^jMask", e.g. jxxxxxx form
                    if (1 == k && 1 <= j && zeroUsable) {
                        // e.g. if "i == 3, j == 2, zeroUsable == true", then both "add1" and "add2" will count "20"
                        // for 1 < k this is not an issue, because "i*pow(10, k > 1)" is definitely not a distinct-digit-number
                        add2 -= 1;
                    }
                    res += add2;
                    if (debug) printf("%*s#2 Updating add2 = %d by j:%d\n", indent, "", add2, j);
                }
            }

            bool iUsable = ((1 << i) & c);
            if (iUsable && zeroUsable && 1 == k) {
                // count self as a distinct-digit-number
                res += 1;
                if (debug) printf("%*s#3 Updating dp[i:%d][k:%d][c:%s] = %d\n", indent, "", i, k, maskbits(c).c_str(), res);
            }
        }
        if (debug) printf("%*sGot dp[i:%d][k:%d][c:%s] = %d\n", indent, "", i, k, maskbits(c).c_str(), res);
        return dp[i][k][c] = res;
    }
    
    int lessThan1Pow10(int k) {
        if (INVALID != lt1p10[k]) return lt1p10[k];
        // counts all distincts strictly less then 1*pow(10, k), DISALLOWING leading 0
        if (0 == k) lt1p10[k] = 0;
        else if (1 == k) lt1p10[k] = 9; // 1~9
        else lt1p10[k] = 
                9*combination(9, k-1)*factorial(k-1) // Choose anyone leading digit other than 0, then choose the other 9 digits to fill the rest k-1 slots
                +
                lessThan1Pow10(k-1);
        if (debug) printf("lt1p10[k:%d] = %d\n", k, lt1p10[k]);
        return lt1p10[k];
    }
    
    int numDupDigitsAtMostN(int n) {
        memset(comb, INVALID, sizeof comb);
        memset(fac, INVALID, sizeof fac);
        memset(dp, INVALID, sizeof dp);
        memset(lt1p10, INVALID, sizeof lt1p10);
        
        string ns = to_string(n);
        unordered_set<char> seen;
        int stillDistinctEnd = 0;
        for (char ch : ns) {
            ++stillDistinctEnd;
            if (seen.count(ch)) break; // it's important to break AFTER the first duplicate digit, not before -- see example "5667"
            seen.insert(ch);
        }
        int k0 = ns.length()-1, k = ns.length()-1;
        int pans = 0, c = (1 << MAXC)-1; // pans means "parity of ans", which is the count of numbers with all distinct digits less than n
        for (int p = 0; p < stillDistinctEnd; ++p) {
            int i = ns[p]-'0';
            pans += dfs(i, k, c, k0, 0);
            --k;
            c ^= (1 << i); // free digits 
        }
        // by now pans still needs add counting of all distincts under 1*pow(10, k0)
        pans += lessThan1Pow10(k0);
        int ans = n-pans;
        return ans;
    }
};
