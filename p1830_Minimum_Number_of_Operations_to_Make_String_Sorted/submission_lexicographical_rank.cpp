bool debug = false;
typedef uint64_t ULL;
ULL const MOD = 1000000007;
int const MAXN = 3000;

ULL fac[MAXN+1]; // factorials

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


ULL modPrimeInverse(ULL a, ULL modPrime) { 
    /*
    * By "Fermat's Little", "pow(a, modPrime-1)%modPrime == 1" which implies that "pow(a, modPrime-2)" is the answer.
    */
    return myPow(a, modPrime-2);
}

ULL modPrimeDivide(ULL a, ULL b, ULL modPrime) { 
    ULL inv = modPrimeInverse(b, modPrime); 
    return (inv * a) % modPrime; 
}

/*
test cases
"acb"
"cba"
"ccba"
"aabaa"
"cdbea"
"string"
"leetcode"
"leetcodeleetcode"
"leetcodeleetcodeleetcode"
*/
class Solution {
public:
    int makeStringSorted(string s) {
        fac[0] = 1;
        for (int i = 1; i <= MAXN; ++i) {
            fac[i] = fac[i-1]*i;
            fac[i] %= MOD;
        }
        
        /*
        The operation mentioned in this problem is in fact "previous permutation", thus the problem is asking to compute the "LexicographicalRank" of the input.
        */
        vector<int> suffixChCnt(26, 0);
        int n = s.length();
        ULL ans = 0;
        for (int i = n-1; i >= 0; --i) {
            ++suffixChCnt[(s[i]-'a')];
            int slotCnt = n-1-i;
            for (int k = 0; ('a'+k) < s[i]; ++k) {
                if (0 == suffixChCnt[k]) continue;
                // If leading by character ('a'+k).
                ULL inc = fac[slotCnt];
                ULL toDiv = 1;
                // Remove duplicate counts introduced by duplicate characters
                for (int kk = 0; kk < 26; ++kk) {
                    if (kk == k) {
                        toDiv *= fac[suffixChCnt[kk]-1];
                    } else {
                        toDiv *= fac[suffixChCnt[kk]];
                    }
                    toDiv %= MOD;
                }
                inc = modPrimeDivide(inc, toDiv, MOD); 
                ans += inc;
                ans %= MOD;
            }
        }
        
        return ans;
    }
};
