/*
test cases
[6,10,3]
[5,15,40,5,6]
*/
int const MAXN = 100000, MAXNUM = (MAXN << 1);

class Solution {
public:    
    int gcd(int big, int small) {
        if (0 == small) return big;
        return gcd(small, big%small);
    }
    
    int countDifferentSubsequenceGCDs(vector<int>& nums) {
        // init
        vector<bool> gcdExists(MAXNUM+1, false);
        for (int num : nums) gcdExists[num] = true; // By definition the gcd of a single number is the number itself
        
        // loop 
        for (int cand = 1; cand <= MAXNUM; ++cand) {
            // Checks whether the gcd value "cand" can be fulfilled.
            if (gcdExists[cand]) continue;
            int allMultiplesGcd = 0;
            // The gcd value "cand" exists if and only if "the gcd of all of its existing multiples" equals "cand".
            for (int coef = 2; coef*cand <= MAXNUM; ++coef) {
                int multiple = coef*cand;
                if (!gcdExists[multiple]) continue;
                int big = max(allMultiplesGcd, multiple);
                int small = min(allMultiplesGcd, multiple);
                allMultiplesGcd = gcd(big, small);
            }
            gcdExists[cand] = (allMultiplesGcd == cand);
        }
        
        int ans = 0;
        for (int cand = 1; cand <= MAXNUM; ++cand) {
            ans += gcdExists[cand];
        }
        
        return ans;
    }
};
