bool const debug = false;

typedef uint64_t ULL;
ULL const LL_MAX = UINT64_MAX;

ULL trailingZeroes(ULL const& n) {
    /*
    test case #1 
    2147483647
    */
    
    if (0 == n) return 0;
    // Find out max{i | "pow(5, i) <= n!" && "n! % pow(5, i) == 0"}
    /*
    - Each of 5, 10, 15, 20, 25, 30, 35, ..., 120, 125, ... contributes once to pow(5, i) for n!.
    - Each of 25, 50, 75, 100, 125, 150, ... contributes an extra count to pow(5, i) for n!.
    - Each of 125, 250, 375, 500, ... contributes yet another extra count to pow(5, i) for n!.
    ...
    */
    ULL divisor = 5, toRet = 0, q = 5;
    while (n/divisor > 0) {
        toRet += (n/divisor);
        if (LL_MAX/q < divisor) {
            break;
        }
        divisor *= q;
    }
    return toRet;
}

bool cmp(ULL const& lhs, int const& K) {
    int tz = trailingZeroes(lhs);
    return tz < K;
}

ULL lower_bound(ULL l, ULL r, int target) {
    while (l < r) {
        ULL mid = ((l + r) >> 1);
        ULL tz = trailingZeroes(mid); 
        // if (debug) printf("lower_bound, l:%lu, r:%lu, tz:%lu\n", l, r, tz);
        if (tz < target) {
            l = mid+1; // "mid" won't be included
        } else {
            // tz >= target
            r = mid;
        }
    }

    return l;
}

ULL upper_bound (ULL l, ULL r, int target) {
    while (l < r) {
        ULL mid = ((l + r) >> 1);
        ULL tz = trailingZeroes(mid); 
        //if (debug) printf("upper_bound, l:%lu, r:%lu\n", l, r);
        if (tz <= target) { 
            l = mid+1; // "mid" won't be included
        } else {
            // tz > target
            r = mid;
        }
    }
    return l;
}

/*
test caes
0
1
2
3
4
5
100
1000
10000
1000000000
*/
class Solution {
public:
    int preimageSizeFZF(int K) {
        ULL low = lower_bound(0, LL_MAX-1, K);
        ULL high = upper_bound(0, LL_MAX-1, K);
        if (debug) printf("low:%lu, high:%lu\n", low, high);
        return high-low;
    }
};
