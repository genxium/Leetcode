bool debug = false;
int const MAXN = 100000;
int const MAXNUM = 50;
int const INVALID = -1;
int const NOT_SET = -2;

typedef vector<int> VI;
typedef vector<pair<int, int>> VPII;

class Solution {
public:
    int gcd(int x, int y) {
        return y == 0 ? x : gcd(y, x % y);
    }
    
    void dfs(int i, int level, VI& nums, vector<VI> &adj, VI& ans, vector<VI>& coprimes, vector<VPII>& occurrences) {
        if (debug) printf("dfs i:%d\n", i);
        occurrences[nums[i]].push_back({i, level});
        for (auto u : adj[i]) {
            if (NOT_SET != ans[u]) continue;
            ans[u] = INVALID;
            
            // The range of "nums[i]" is limited to [1, 50], thus instead of going up every parent to find the nearest coprime ancestor, we save the "occurrences[nums[*]].back() = {nodeId, deepestLevel}" for all visited ancestors of "i".
            int deepestAncestorLv = INT_MIN;
            for (auto anotherNum : coprimes[nums[u]]) {
                if (occurrences[anotherNum].empty()) continue;
                if (occurrences[anotherNum].back().first == u) continue; // skip self
                if (occurrences[anotherNum].back().second > deepestAncestorLv) {
                    deepestAncestorLv = occurrences[anotherNum].back().second;
                    ans[u] = occurrences[anotherNum].back().first;
                }
            }
            
            if (debug) printf("At i:%d, got ans[u:%d] == %d, deepestAncestorLv of nums[u:%d]:%d\n", i, u, ans[u], u, nums[u]);
            
            dfs(u, level+1, nums, adj, ans, coprimes, occurrences);
        }
        occurrences[nums[i]].pop_back();
    }
        
    vector<int> getCoprimes(vector<int>& nums, vector<vector<int>>& edges) {
        vector<VI> coprimes(MAXNUM+1, VI{});
        for (int i = 0; i <= MAXNUM; ++i) {
            for (int j = 0; j <= MAXNUM; ++j) {
                int gcdVal = i > j ? gcd(i, j) : gcd(j, i);
                if (gcdVal == 1) {
                    coprimes[i].push_back(j);
                }
            }
        }
        
        int n = nums.size();
        vector<VI> adj(n, VI{});
        for (auto &edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }
        
        VI ans(n, NOT_SET);
        ans[0] = INVALID;
    
        vector<VPII> occurrences(MAXNUM+1, VPII{});
        dfs(0, 0, nums, adj, ans, coprimes, occurrences);
        return ans;
    }
};
