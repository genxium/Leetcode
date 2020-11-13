/*
The problem is very likely a "2-dimensional cost knapsack", where the cost is "count of 0s & count of 1s" and the value is "1" for putting each "str" in the result set. 
*/

typedef pair<int, int> cost_t; // {count_of_0s, count_of_1s}
int const MAXSIZE = 600;
int const MAXM = 100;
int const MAXN = 100;
int dp[MAXSIZE][MAXM+1][MAXN+1]; // "dp[i][usedZeros][usedOnes] == x" means that by using "strs[0, ..., i]" with EXACT cost {usedZeros, usedOnes}, we can include most "x" elements from "strs", NOT NECESSARILY including "strs[i]".

/*
test case#1
["10","0001","111001","1","0"]
5
3

test case#2
["10","0","1"]
1
1
*/
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        vector<cost_t> costs;
        for (int i = 0; i < strs.size(); ++i) {
            string &str = strs[i];
            cost_t single{0, 0};
            for (auto &ch : str) {
                if ('0' == ch) ++single.first;
                if ('1' == ch) ++single.second;
            }
            costs.push_back(single);
            // printf("@i:%d, single == {%d, %d}\n", i, single.first, single.second);
        }
        
        // init
        int ans = 0;
        memset(dp, 0, sizeof(dp));
        if (costs[0].first <= m && costs[0].second <= n) {
            dp[0][costs[0].first][costs[0].second] = 1;
            ans = 1;
        }

        // loop
        for (int i = 1; i < costs.size(); ++i) {
            cost_t single = costs[i];
            for (int usedZeros = 0; usedZeros <= m; ++usedZeros) {
                for (int usedOnes = 0; usedOnes <= n; ++usedOnes) {
                    int candidate1 = dp[i-1][usedZeros][usedOnes];
                    dp[i][usedZeros][usedOnes] = max(dp[i][usedZeros][usedOnes], candidate1);
                    
                    if (usedZeros >= single.first && usedOnes >= single.second) {
                        int candidate2 = 1 + dp[i-1][usedZeros-single.first][usedOnes-single.second];
                        dp[i][usedZeros][usedOnes] = max(dp[i][usedZeros][usedOnes], candidate2);
                    }
                    // printf("dp[i:%d][usedZeros:%d][usedOnes:%d] == %d\n", i, usedZeros, usedOnes, dp[i][usedZeros][usedOnes]);
                    ans = max(ans, dp[i][usedZeros][usedOnes]);
                }
            }
        }
        
        return ans;
    }
};
