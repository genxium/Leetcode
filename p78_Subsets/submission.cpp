class Solution {
public:
    void genSubset(vector<int> &nums, int &n, int k, vector<int> &candidate, vector<vector<int>> &ans) {
        if (k == n) {
            vector<int> clone(candidate);
            ans.push_back(clone);
            return;
        }
        
        candidate.push_back(nums[k]);
        genSubset(nums, n, k+1, candidate, ans); 
        candidate.pop_back(); // backtracking
        genSubset(nums, n, k+1, candidate, ans);
    } 
        
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> ans;
        vector<int> candidate;
        int n = nums.size();
        genSubset(nums, n, 0, candidate, ans);
        return ans;
    }
};
