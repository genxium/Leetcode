unordered_map<int, unordered_map<int, int>> cache;

class Solution {
public:
    void dfs(int iTopping, vector<int>& toppingCosts, int cost, int target, int &ans) {
        if (cache.count(iTopping) && true == cache[iTopping][cost]) return;
        
        if (iTopping == toppingCosts.size()) {
            if (abs(cost - target) < abs(ans - target)) ans = cost;
            else if (abs(cost - target) == abs(ans - target) && cost < ans) ans = cost;
            return;
        }
        
        if (cost > target && cost-target > abs(ans-target)) return; // no need to move on
        
        // skip
        dfs(iTopping+1, toppingCosts, cost, target, ans);
        
        // use 1
        dfs(iTopping+1, toppingCosts, cost+toppingCosts[iTopping], target, ans);
        
        // use 2
        dfs(iTopping+1, toppingCosts, cost+2*toppingCosts[iTopping], target, ans);
        
        cache[iTopping][cost] = true;
    }
    
    int closestCost(vector<int>& baseCosts, vector<int>& toppingCosts, int target) {
        cache.clear();
        int ans = baseCosts[0];
        for (auto cost : baseCosts) {
            dfs(0, toppingCosts, cost, target, ans);            
        }
        return ans;
    }
};
