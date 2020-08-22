#define VI vector<int>
#define CITY_A 0
#define CITY_B 1

class CostCompare {
public:
    bool operator() (VI const &lhs, VI const &rhs) const {
        int lhsExcessiveCostIfSentToCityA = lhs[CITY_A] - lhs[CITY_B];
        int rhsExcessiveCostIfSentToCityA = rhs[CITY_A] - rhs[CITY_B];
        return (lhsExcessiveCostIfSentToCityA < rhsExcessiveCostIfSentToCityA);
    }
};

class Solution {
public:
    int twoCitySchedCost(vector<vector<int>>& costs) {
        sort(costs.begin(), costs.end(), CostCompare());
        int doubleN = costs.size();
        int n = (doubleN >> 1);
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            ans += costs[i][CITY_A]; // sent to CITY_A
            ans += costs[i+n][CITY_B]; // sent to CITY_B
        }
        return ans;
    }
};
