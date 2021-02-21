bool debug = false;

unordered_map<int, unordered_map<int, bool>> cache;

class Solution {
public:
    bool dfs(int ni, int gi, vector<vector<int>>& groups, vector<int>& nums) {
        if (gi == groups.size()) return true;
        if (ni == nums.size()) return false;
        if (cache.count(ni) && cache[ni].count(gi)) return cache[ni][gi];
                
        bool groupMatched = true;
        vector<int> &group = groups[gi];
        if (ni+group.size() > nums.size()) {
            groupMatched = false;
        } else {
            for (int t = 0; t < group.size(); ++t) {
                if (nums[ni+t] != group[t]) {
                    groupMatched = false;
                    break;
                }
            }
        }
        
        bool toRet = false;
        if (groupMatched) {
            if (debug) printf("groupMatched, ni:%d, gi:%d, trying ni+group.size():%d, gi+1:%d\n", ni, gi, ni+group.size(), gi+1);
            toRet |= dfs(ni+group.size(), gi+1, groups, nums);
        }
        if (!toRet) toRet |= dfs(ni+1, gi, groups, nums);
        return cache[ni][gi] = toRet;
    }
    
    bool canChoose(vector<vector<int>>& groups, vector<int>& nums) {
        cache.clear();
        
        return dfs(0, 0, groups, nums);
    }
};
