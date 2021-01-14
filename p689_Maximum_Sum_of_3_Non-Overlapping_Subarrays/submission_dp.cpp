bool const debug = false;
int const INVALID = INT_MIN;
int const MAXN = 20000, MAXC = 3;
/*
"dp[i][c]" being the largest sum using "nums[0, ..., i-1]" to make "c transactions of size k" on the "prefixSum array"

dp[i][c] = max{
             dp[i-k][c-1] + kPrevSum[i], 
             dp[i-1][c]
           }
*/
int dp[MAXN+1][MAXC+1];
int prevHop[MAXN+1][MAXC+1];

/*
test cases
[1,2,1,2,6,7,5,1]
2
[1,2,1,200,1,2,6,1,3,99,97,7,5,1,100,9]
3
*/
class Solution {
public:
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> ps = {0}; // prefixSum
        for (int i = 0; i < n; ++i) ps.push_back(ps.back()+nums[i]);
        vector<int> kPrevSum;
        
        for (int i = 0; i <= n; ++i) {
            if (i < k) kPrevSum.push_back(INVALID);
            else kPrevSum.push_back(ps[i]-ps[i-k]);
            if (debug) printf("kPrevSum[%d] == %d\n", i, kPrevSum.back());
        }
        
        // init
        memset(prevHop, -1, sizeof prevHop);
        for (int i = 0; i <= n; ++i) {
            dp[i][0] = 0;
            for (int c = 1; c <= 3; ++c) {
                dp[i][c] = INVALID;
            }
        }
        
        int result = 0;
        
        // loop
        for (int i = k; i <= n; ++i) {
            for (int c = 1; c <= 3; ++c) {
                dp[i][c] = dp[i-1][c];
                prevHop[i][c] = prevHop[i-1][c];
                if (INVALID != dp[i-k][c-1]) {
                    int cand = dp[i-k][c-1] + kPrevSum[i];
                    if (cand > dp[i][c]) {
                        dp[i][c] = cand;
                        prevHop[i][c] = i-k;
                    }
                }
                if (debug && INVALID != dp[i][c]) {
                    printf("dp[i:%d][c:%d] == %d, prevHop[i:%d][c:%d] == %d\n", i, c, dp[i][c], i, c, prevHop[i][c]);
                }
                if (c == 3) {
                    result = max(result, dp[i][c]);
                }
            }
        }
        
        vector<int> ans;
        for (int i = 1; i <= n; ++i) {
            if (dp[i][3] == result) {
                ans.push_back(i-k); // The problem wants the "left boundary index"
                result = dp[prevHop[i][3]][2];
                break;
            }
        }
        
        for (int i = 1; i <= n; ++i) {
            if (dp[i][2] == result) {
                ans.push_back(i-k);
                result = dp[prevHop[i][2]][1];
                break;
            }
        }
        
        for (int i = 1; i < n; ++i) {
            if (dp[i][1] == result) {
                ans.push_back(i-k);
                break;
            }
        }
    
        reverse(ans.begin(), ans.end());
        
        return ans;
    }
};
