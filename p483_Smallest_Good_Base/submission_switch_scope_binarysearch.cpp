int const INVALID = -1;
typedef uint64_t ULL;

/*
test case#1
"13"

test case#2
"4681"

test case#3
"1000000000000000000"

test case#4
"999999999999999999"

test case#5
"3541"

test case#6
"916365355264637559"
// should return "971306"
*/
class Solution {
public:
    int findProperM(ULL k, ULL &nn) {
        int m = 0;
        ULL acc = 0, ele = 1;
        while (acc < nn) {
            acc += ele;
            ele *= k;
            ++m;
        }
        
        if (acc == nn) {
            // printf("For nn:%lu, k:%lu, m == %d yields acc == %lu\n", nn, k, m, acc);
            return m;
        }
        return INVALID;
    }
    
    ULL findProperK(int m, ULL &nn) {
        ULL kLow = 11u, kHigh = nn-1, validK = UINT64_MAX;
        while (kLow <= kHigh) {
            ULL candidate = ((kLow + kHigh) >> 1);
            //printf("Testing base candidate %lu for nn:%lu and m:%d\n", candidate, nn, m);   
            ULL acc = nn, ele = 1;
            int i = 0;
            for (; i < m; ++i) {
                if (acc < ele) break;
                acc -= ele;
                if (UINT64_MAX/candidate >= ele) {
                    ele *= candidate;   
                } else {
                    if (i == m-1) {
                        // doesn't matter because we're ending
                        continue;
                    } else {
                        // "ele" will overflow
                        break;
                    }
                }
            }
            
            if (acc == 0u && i == m) {
                validK = min(validK, candidate);
                kHigh = candidate - 1;
                continue;
            }
            if (i < m) {
                // candidate is too large
                kHigh = candidate - 1;
                continue;
            }
            // i == m, but acc > 0, candidate is too small
            kLow = candidate + 1;
            continue;
        }
        
        return validK; // as an invalid indicator 
    }
    
    string smallestGoodBase(string n) {
        // In general, an m-digit number based "k", i.e. "n = 1...11(k)" is valued "n = 1 + k + ... + k^(m-1) = (k^m - 1)/(k-1)", therefore for a fixed "k" we should be able to find a proper non-negative integer "m" to fit in. 
        
        ULL nn = 0u; // 2^64 is larger than 10^18
        for (int i = 0; i < n.length(); ++i) {
            nn *= 10u;
            nn += (ULL)(n[i]-'0');
        }
        
        // First try "k = 2 ~ 10".
        for (ULL k = 2; k <= 10; ++k) {
            int m = findProperM(k, nn);
            if (INVALID != m) {
                return to_string(k);
            }
        }
        
        /*
        * Now that "k > 10", we can assert that
        * - k definitely exists, bcz "k == n-1" is always a valid solution, and that
        * - m definitely goes smaller than or equal to 17 (since n <= 10^18), thus we can just traverse m in [17, 1].
        */
        for (int m = 17; m >= 1; --m) {
            ULL validK = findProperK(m, nn);
            if (UINT64_MAX != validK) return to_string(validK);
        }
        
        return to_string(nn-1);
    }
};
