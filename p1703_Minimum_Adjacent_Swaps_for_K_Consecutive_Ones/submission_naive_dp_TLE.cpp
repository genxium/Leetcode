bool const debug = false;

int const INVALID = -1;
int const MAXN = 100000;

/*
"dp1[i][c] == x" means that by using "nums[0, ..., i]" to make "c" consecutive "1"s ENDED EXACTLY AT "nums[i]" will take at least "x" swaps.

"dp2[i][c] == y" means that by using "nums[i, ..., n)" to make "c" consecutive "1"s STARTED EXACTLY AT "nums[i]" will take at least "y" swaps.
*/

unordered_map<int, int> dp1[MAXN], dp2[MAXN];
    
/*
test cases
[1,0,0,1,0,1]
2
[1,0,0,0,0,0,1,1]
3
[1,1,0,1]
2
[0,1,1,0,0,1,0,0,0]
3
[1,0,0,1,0,1,1,1,0,1,1]
7
*/
class Solution {
public:
    int minMoves(vector<int>& nums, int k) {
        if (1 == k) return 0;
        
        int n = nums.size();
        
        // init
        for (int i = 0; i < n; ++i) {
            dp1[i].clear(); dp2[i].clear();
            if (1 == nums[i]) {
                dp1[i][0] = dp2[i][0] = 0;  
                dp1[i][1] = dp2[i][1] = 0;
            }
        }
        
        // loop
        for (int i = 1; i < n; ++i) {
            for (int c = 1; c <= k; ++c) {
                if (1 == nums[i]) {
                    if (dp1[i-1].count(c-1)) {
                        dp1[i][c] = dp1[i-1][c-1]; // works for "c == 1" too
                    }
                } else {
                    if (dp1[i-1].count(c)) {
                        dp1[i][c] = c + dp1[i-1][c]; // need move "nums[i]: 0" by "c" steps toward "nums[0]" direction 
                    }
                }    
            }
        }
        
        for (int i = n-2; i >= 0; --i) {
            for (int c = 1; c <= k; ++c) {
                if (1 == nums[i]) {
                    if (dp2[i+1].count(c-1)) {
                        dp2[i][c] = dp2[i+1][c-1]; // works for "c == 1" too
                        if (debug) printf("dp2[i:%d][c:%d] = %d; #1\n", i, c, dp2[i][c]);
                    }
                } else {
                    if (dp2[i+1].count(c)) {
                        dp2[i][c] = c + dp2[i+1][c]; // need move "nums[i]: 0" by "c" steps toward "nums[n-1]" direction
                        if (debug) printf("dp2[i:%d][c:%d] = %d; #2\n", i, c, dp2[i][c]);
                    }
                }    
            }
        }
        
        int ans = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (dp1[i].count(k)) {
                ans = min(ans, dp1[i][k]);
            } 
            if (dp2[i].count(k)) {
                ans = min(ans, dp2[i][k]);    
            }
            for (int c = 1; c < k; ++c) {
                if (dp1[i].count(c) && dp2[i].count(k+1-c)) {
                    ans = min(ans, dp1[i][c]+dp2[i][k+1-c]);
                }
            }
        }

        return ans;
    }
};
