bool const debug = false;
typedef int64_t LL;
typedef vector<LL> VL;
LL const INVALID = -1;
int const MAXN = 100000;
LL const MOD = 1000000007;

LL multiplyMod(LL lhs, LL rhs) {
    return (lhs*rhs)%MOD;
}

LL myPow(LL x, LL n) {
    // See https://github.com/genxium/Leetcode/tree/master/p50_Pow_x_n for an explanation of the custom integer power implementation.

    if (1 == n || 0 == x || 1 == x) return x; 
    if (0 == n) return 1;

    int const INTEGER_BIT_LENGTH = sizeof(int)*CHAR_BIT;
    int nBinaryLengthUpper = 0;
    while (nBinaryLengthUpper < INTEGER_BIT_LENGTH && (1 << nBinaryLengthUpper) <= n) ++nBinaryLengthUpper;

    LL ans = 1;
    for (int i = nBinaryLengthUpper-1; i >= 0; --i) {
        ans = multiplyMod(ans, ans);
        if (((1 << i) & n) > 0) {
            ans = multiplyMod(ans, x);
        }
    }

    return ans;
}


LL modPrimeInverse(LL a, LL modPrime) { 
    /*
    * By "Fermat's Little", "pow(a, modPrime-1)%modPrime == 1" which implies that "pow(a, modPrime-2)" is the answer.
    */
    return myPow(a, modPrime-2);
}

LL modPrimeDivide(LL a, LL b, LL modPrime) { 
    LL inv = modPrimeInverse(b, modPrime); 
    if (INVALID == inv) {
        return INVALID;
    } 
    return (inv * a) % modPrime; 
}

/*
test cases
["Fancy", "append", "addAll", "append", "multAll", "getIndex", "getIndex", "addAll", "getIndex", "append", "getIndex", "multAll", "getIndex", "getIndex", "getIndex"]
[[], [2], [3], [7], [2], [0], [1], [3], [1], [10], [2], [2], [0], [1], [2]]
*/
class Fancy {
private:
    LL accAdded = 0, accMultiplied = 1;
    vector<VL> seqWithDebt;
public:    
    void append(int val) {
        /*
        * The "accAdded" and "accMultiplied" accumulated before current "val" is appended should be deemed as its "debt".
        */
        seqWithDebt.push_back({val, accAdded, accMultiplied});
    }
    
    void addAll(int inc) {
        accAdded += inc;
        accAdded %= MOD;
    }
    
    void multAll(int factor) {
        // There's no room for FenwichTree either because of the "multiplication op".
        accMultiplied *= factor; accMultiplied %= MOD;
        accAdded *= factor; accAdded %= MOD;
    }
    
    int getIndex(int idx) {
        if (idx >= seqWithDebt.size()) return -1;
        auto &ele = seqWithDebt[idx];
        LL debtAdded = ele[1], debtMultiplied = ele[2];
        /* 
        The key idea here is that if "y == *(+(*(*(+(x, a1), b1), b2), a2), b3)", then "x" can be recovered by simply
        ```
        x = (y-effectiveAdded)/effectiveMultiplied"
        ```
        , where "effectiveAdded == (a1*b1*b2+a2)*b3", "effectiveMultiplied == b1*b2*b3".
        
        All the other techniques are minor.
        */
        LL effectiveMultiplied = modPrimeDivide(accMultiplied, ele[2], MOD);
        LL effectiveAdded = (accAdded - debtAdded*effectiveMultiplied%MOD + MOD)%MOD; // Take "debtAdded as x" and "accAdded as y" in the equation above.
        LL ans = (ele[0]*effectiveMultiplied)%MOD + effectiveAdded;
        ans %= MOD;
        return ans;
    }
};

/**
 * Your Fancy object will be instantiated and called as such:
 * Fancy* obj = new Fancy();
 * obj->append(val);
 * obj->addAll(inc);
 * obj->multAll(m);
 * int param_4 = obj->getIndex(idx);
 */
