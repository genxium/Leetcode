int const MAXN = 20000;
int const PICKED = 0;
int const NOT_PICKED = 1;

int dp[MAXN][2];

class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        memset(dp, 0, sizeof dp);
        map<int, int> counter;
        for (auto num : nums) ++counter[num];
        
        vector<pair<int, int>> arr;
        for (auto it : counter) {
            arr.push_back({it.first, it.second});
        }
        sort(arr.begin(), arr.end(), less<pair<int, int>>());
        
        dp[0][PICKED] = arr[0].first*arr[0].second;
        for (int i = 1; i < arr.size(); ++i) {
            // NOT PICKED
            dp[i][NOT_PICKED] = max(dp[i-1][PICKED], dp[i-1][NOT_PICKED]);
            
            // PICKED
            int prevAddition = (
                                arr[i-1].first < arr[i].first-1
                                ?
                                max(dp[i-1][PICKED], dp[i-1][NOT_PICKED])
                                :
                                (i-2 >= 0 ? max(dp[i-2][PICKED], dp[i-2][NOT_PICKED]) : 0)
                                );
            dp[i][PICKED] = arr[i].first*arr[i].second + prevAddition;
        }
        return max(dp[arr.size()-1][NOT_PICKED], dp[arr.size()-1][PICKED]);
    }
};
