int const MAXN = 1000000000;
int const INVALID = -1;
typedef int64_t LL;

/*
test case#1
247662
*/
class Solution {
public:
    unordered_map<int, int> cache;
    void solve(int prevN, int n, int indent) {
        if (0 == n) {
            cache[0] = 0;
            return;
        }
        if (1 == n) {
            cache[1] = 1;
            return;
        }
        
        if (cache.find(n) != cache.end()) {
            return;
        }
        
        LL mask = 1L;
        while ((mask & n) == 0) {
            mask <<= 1L;
        }
        
        // now that (mask & n) > 0
        if (mask == n) {
            cache[n] = (mask << 1L) - 1L;
            return;
        }
        
        mask <<= 1;
        //printf("%*ssolve prevN:%d, n:%d, mask:%d, n^mask:%d, n^1:%d\n", indent, "", prevN, n, mask, (n^mask), (n^1));
        
        int rightmostFlipped = (n^1);
        int candidate1 = INVALID;
        if (rightmostFlipped != prevN) {
            solve(n, rightmostFlipped, indent+1);
            if (cache[rightmostFlipped] != INVALID) {
                candidate1 = 1+cache[rightmostFlipped];                
            }
        }
        
        int candidate2 = INVALID;
        if (mask <= n) {
            int maskFlipped = (n^mask);
            if (maskFlipped != prevN) {
                solve(n, maskFlipped, indent+1);
                if (cache[maskFlipped] != INVALID) {
                    candidate2 = 1+cache[maskFlipped];                    
                }
            }    
        }
        
        if (candidate1 != INVALID && candidate2 != INVALID) {
            cache[n] = min(candidate1, candidate2);
        } else if (candidate1 != INVALID) {
            cache[n] = candidate1;
        } else {
            // candidate2 != INVALID
            cache[n] = candidate2;
        }
        //printf("%*sGot cache[n:%d] == %d\n", indent, "", n, cache[n]);
    }
    
    int minimumOneBitOperations(int n) {
        // solve(n, n, 0);
        // return cache[n];
        /*
        Checked the special values
        1 -> 1
        2 -> 3
        4 -> 7
        8 -> 15
        1024 -> 2047

        Found the trick accidentally
        1032 = 1024+8 -> 2047-15 = 2032
        1036 = 1024+8+4 -> 2047-15+7 = 2039
        1036 = 1024+8+4+2 -> 2047-15+7-3 = 2036
        */
        int ans = 0;
        LL mask = 1;
        vector<LL> pow2Comp;
        while (mask <= n) {
            if ((n & mask) > 0) {
                pow2Comp.push_back(mask*2L - 1L);
            }
            mask <<= 1;
        }
        // now that mask > n
        reverse(pow2Comp.begin(), pow2Comp.end());
        int factor = -1;
        for (int i = 0; i < pow2Comp.size(); ++i) {
            ans += (i % 2 == 0 ? -factor : factor)*(pow2Comp[i]);
        }
        return ans;
    }
}; 
