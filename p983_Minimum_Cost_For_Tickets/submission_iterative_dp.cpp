class Solution {
public:
    int mincostTickets(vector<int>& days, vector<int>& costs) {
        sort(days.begin(), days.end());
        int n = days.size();
        unordered_map<int, int> dp; // dp[i] = min cost by ENDING the travel at days[i]
        dp[-1] = 0; // init
        for (int i = 0; i < n; ++i) {
            dp[i] = dp[i-1] + costs[0];
            int noEarlierThan7 = days[i]-7;
            auto it1 = upper_bound(days.begin(), days.end(), noEarlierThan7);
            int j1 = (int)(it1-days.begin());
            if (j1-1 < i) {
                dp[i] = min(dp[i], dp[j1-1] + costs[1]);
            }

            int noEarlierThan30 = days[i]-30;
            auto it2 = upper_bound(days.begin(), days.end(), noEarlierThan30);
            int j2 = (int)(it2-days.begin());
            if (j2-1 < i) {
                dp[i] = min(dp[i], dp[j2-1] + costs[2]);
            }
        }

        return dp[n-1];
    }
};
