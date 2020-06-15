class Solution {
public:
    void genSolution(int &target, vector<int> &tmpVec, int &tmpSum, vector<int> &candidates, vector< vector<int> > &ans, int candidateIdxToStartSeeking) {
        if (tmpSum == target) {
            ans.push_back(vector<int>(tmpVec));
            return;
        }
        set<int> chosenValuesAtThisPosition;
        // We allow the use of duplicate values from "candidates" by managing "candidateIdxToStartSeeking", yet prohibit "same value at same position" to show up in different "tmpVec"s in "ans".
        for (int i = candidateIdxToStartSeeking; i < candidates.size(); ++i) {
            int val = candidates[i];
            if (chosenValuesAtThisPosition.find(val) != chosenValuesAtThisPosition.end()) continue;
            if (tmpSum + val > target) continue;
            tmpVec.push_back(val);
            tmpSum += val;
            genSolution(target, tmpVec, tmpSum, candidates, ans, i+1);
            tmpVec.pop_back();
            tmpSum -= val;
            chosenValuesAtThisPosition.insert(val);
        }
    }
    
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<int> tmpVec;
        int tmpSum = 0;
        int candidateIdxToStartSeeking = 0;
        vector< vector<int> > ans;
        genSolution(target, tmpVec, tmpSum, candidates, ans, candidateIdxToStartSeeking);
        return ans;   
    }
};
