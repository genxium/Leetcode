bool debug = false;
int const MAXN = 1000;
int dp[MAXN][MAXN]; // dp[i][j] is the longest fibsub that ends EXACTLY at "arr[i]" and the second last element is arr[j]

/*
test cases
[1,2,3,4,5,6,7,8]
[1,3,7,11,12,14,18]
[2,4,7,8]
[2,4,7,8,9,10,14,15,18,23,32,50]
[2,4,5,6,7,8,11,13,14,15,21,22,34]
[6,7,8,9,13,15,16,19,24,32,39,40,51]
*/
class Solution {
public:
    int lenLongestFibSubseq(vector<int>& arr) {
        int ans = 0;
        
        memset(dp, 0, sizeof dp);
        for (int i = 1; i < arr.size(); ++i) {
            for (int m = 0; m < i; ++m) {
                 dp[m][i] = 2;       
            }
        }
        
        for (int j = 2; j < arr.size(); ++j) {
            for (int i = 1; i < j; ++i) {
                int diff = arr[j]-arr[i];
                auto it = lower_bound(arr.begin(), arr.begin()+j, diff);
                if (debug) printf("arr[j:%d]:%d - arr[i:%d]:%d is diff:%d\n", j, arr[j], i, arr[i], diff);
                if (it == arr.end() || *it != diff) continue;
                int m = it - arr.begin();
                if (0 == dp[m][i]) continue;
                if (debug) printf("\tarr[m:%d]:%d, dp[m:%d][i:%d] is %d\n", m, arr[m], m, i, dp[m][i]);
                dp[i][j] = max(dp[i][j], 1+dp[m][i]);
                ans = max(dp[i][j], ans);
            }
        }
        
        return ans;
    }
};
