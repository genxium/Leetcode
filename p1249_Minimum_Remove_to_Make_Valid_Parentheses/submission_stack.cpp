class Solution {
public:
    string minRemoveToMakeValid(string s) {
        stack<int> leftHalfPositions;
        
        unordered_set<int> orphanedRightHalfPositions;
        int l = s.length();
        for (int i = 0; i < l; ++i) {
            if (s[i] == ')') {
                if (leftHalfPositions.empty()) {
                    orphanedRightHalfPositions.insert(i);      
                } else {
                    leftHalfPositions.pop();
                }        
            }
            if (s[i] == '(') {
                leftHalfPositions.push(i);
            }
        }
        unordered_set<int> orphanedLeftHalfPositions;
        while (!leftHalfPositions.empty()) {
            int i = leftHalfPositions.top(); leftHalfPositions.pop();
            orphanedLeftHalfPositions.insert(i);
        }
        
        string ans = "";
        for (int i = 0; i < l; ++i) {
            if (orphanedLeftHalfPositions.find(i) == orphanedLeftHalfPositions.end() 
               &&
               orphanedRightHalfPositions.find(i) == orphanedRightHalfPositions.end()) {
                ans.push_back(s[i]);
            }
        }
        return ans;
    }
};
