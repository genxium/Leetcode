class Solution {
public:
    uint absu(int n) {
        if (n == INT_MIN) return uint(INT_MAX) + 1;
        else return uint(abs(n));
    }
    
    int trailingZeroes(int n) {
        /*
        test case #1 
        2147483647
        */
        
        uint un = absu(n);
        if (0u == un) return 0;
        // Find out max{i | "pow(5, i) <= n!" && "n! % pow(5, i) == 0"}
        /*
        - Each of 5, 10, 15, 20, 25, 30, 35, ..., 120, 125, ... contributes once to pow(5, i) for n!.
        - Each of 25, 50, 75, 100, 125, 150, ... contributes an extra count to pow(5, i) for n!.
        - Each of 125, 250, 375, 500, ... contributes yet another extra count to pow(5, i) for n!.
        ...
        */
        uint divisor = 5;
        int ans = 0;
        while (un/divisor > 0u) {
            ans += int(un/divisor);
            if (UINT_MAX/5u < divisor) {
                break;
            } else {
                divisor *= 5u;
            }
        }
        return ans;
    }
};
