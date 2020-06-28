class Solution {
public:
    void genSubset(vector<int> &nums, int &n, int k, vector<pair<int, int>> &counter, vector<int> candidate, vector<vector<int>> &ans) {
        if (k == n) {
            vector<int> clone(candidate);
            ans.push_back(clone);
            return;
        }
        
        auto pair = counter[k];
        for (int i = 0; i <= pair.second; ++i) {
            //printf("using i:%d of pair.first:%d\n", i, pair.first);
            for (int j = 0; j < i; ++j) {
                candidate.push_back(pair.first);
            }
            genSubset(nums, n, k+1, counter, candidate, ans); 
            for (int j = 0; j < i; ++j) {
                candidate.pop_back(); // backtracking
            }
        }
    }
    
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        unordered_map<int, int> counterDict;
        vector<pair<int, int>> counter;
        for (auto val : nums) {
            if (counterDict.count(val) > 0) {
                counterDict[val] ++;
            } else {
                counterDict[val] = 1;
            }
        }
        int n = 0;
        for (auto it : counterDict) {
            ++n;
            counter.push_back(make_pair(it.first, it.second));
        }
        vector<vector<int>> ans;
        vector<int> candidate;
        genSubset(nums, n, 0, counter, candidate, ans); 
        return ans;
    }
};
