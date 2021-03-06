int MAXVAL = 1024;

/*
test cases
[1,2,0,3,0]
1
[3,4,5,2,1,7,3,4,7]
3
[1,2,4,1,2,5,1,2,6]
3
[16,5,15,6,1,25,0,13,12,12,7,16,4,25,3]
3
[28,10,1,17,15,29,3,23,10,8,23,21,12,31,24,23,11,19,27,3,15,26,29,20,19,25,3,31,16,20,19,17,11,2,4]
14
*/
class Solution {
public:
    int minChanges(vector<int>& nums, int k) {
        /*
        Consider an easier problem.
        ```
        Given "arr[0, ..., k)", find the min #changes to make "XOR{arr[0, ..., k)} == 0".
        ```

        Denote "dp[i][v]" as the min #changes to make "XOR{arr[0, ..., i]} == v". It's obvious that

        ```
        dp[i][v] = MIN{dp[i-1][u]+(v^u == arr[i] ? 0 : 1) | u in dp[i-1]}
        ```

        Note that each "dp[i]" is capable of covering ANY "v", because we can CHANGE "arr[i]" ARBITRARILY.
        */

        /*
        Now that "nums[]" has a size "n >= k", yet to ensure that every k-segment XOR-ed to 0, it's obvious that the final state satisfies

        ```
        nums[0] == nums[k], nums[1] == nums[1+k], ..., nums[k-1] == nums[k-1+k], nums[k] == nums[k+k], ..., nums[k-1+k] == nums[k-1+2*k]
        ```

        , i.e. each "nums[t < k] == nums[t + c*k]". It means that we can only consider "nums[0, ..., k)" and almost the same "dp[i][v]" in the easier problem, with a little modification.

        When computing "dp[i < k][v]" from a certain "dp[i-1][u]" for this problem, even if "nums[i] == v^u", we might need some #changes to make all the other "nums[i+c*k] -> v^u" too.
        */

        vector<unordered_map<int, int>> freq(k); // "freq[i < k][v] == x" means that value "v" occurs "x" times among "nums[i+c*k]" where "c >= 0 && i+c*k < n"  
        vector<int> size(k, 0); // "size[i]" counts how many elements are contained in "freq[i]" 

        int n = nums.size();
        for (int i = 0; i < k; ++i) {
            for (int j = i; j < n; j += k) {
                ++freq[i][nums[j]];
                ++size[i];
            }
        }

        // init
        vector<vector<int>> dp(k, vector<int>(MAXVAL, INT_MAX));
        for (int v = 0; v < MAXVAL; ++v) {
            dp[0][v] = size[0]-freq[0][v]; // some can remain unchanged
        }

        // loop 
        for (int i = 1; i < k; ++i) {
            int prevMinChanges = INT_MAX;
            for (int u = 0; u < MAXVAL; ++u) {
                if (dp[i-1][u] < prevMinChanges) prevMinChanges = dp[i-1][u];
            }

            for (int v = 0; v < MAXVAL; ++v) {
                /* First, assume that we change ALL "nums[i+c*k]" to a value that doesn't exist among "nums[i+c*k]". In this case as we can change "nums[i+c*k]" arbitrarily, just pick a value that makes "w^v == p" where "nums[i+c*k] == w" and "dp[i-1][p] == MIN{dp[i-1]}". */
                dp[i][v] = min(dp[i][v], prevMinChanges+size[i]);

                /* Second, assume that we change ALL "nums[i+c*k]" to a value among themselves. */
                // [WARNING] As discussed above, "dp[i-1]" is capable of covering ANY "u", thus too costly to traverse as the final layer here, i.e. traversing "for (auto &[u, oldChanges] : dp[i-1])" at this layer instead will result in TLE! 
                for (auto &[t, occurredCnt] : freq[i]) {
                    int u = v^t;
                    if (INT_MAX == dp[i-1][u]) continue;
                    int cand = dp[i-1][u] + (size[i]-occurredCnt);
                    if (dp[i][v] > cand) dp[i][v] = cand;
                }
            }
        }

        return dp[k-1][0];
    }
};
