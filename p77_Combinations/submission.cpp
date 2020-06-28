class Solution {
public:
    void genCombination(int &k, int selectedCount, vector<int> &candidate, int offsetInSortedNums, vector<int> &sortedNums, vector<vector<int>> &ans) {
        if (selectedCount == k) {
            vector<int> clone(candidate);
            ans.push_back(clone);
            return;
        } 
        if (selectedCount + (sortedNums.size() - offsetInSortedNums) < k) {
            // no need to proceed
            return;
        }
        // choose "sortedNums[offsetInSortedNums]"
        candidate.push_back(sortedNums[offsetInSortedNums]);
        genCombination(k, selectedCount+1, candidate, offsetInSortedNums+1, sortedNums, ans);
        candidate.pop_back(); // backtracking
        
        // doesn't choose "sortedNums[offsetInSortedNums]"
        genCombination(k, selectedCount, candidate, offsetInSortedNums+1, sortedNums, ans);
    }
    
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> ans;
        vector<int> candidate;
        vector<int> sortedNums;
        for (int i = 0; i < n; ++i) {
            sortedNums.push_back(i+1);
        }
        genCombination(k, 0, candidate, 0, sortedNums, ans);
        return ans;
    }
};
