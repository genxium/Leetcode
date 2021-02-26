bool debug = false;

class Solution {
public:
    void dfs(int i, vector<string> &currentPair, bool currentValUsedDecimalPt, string &S, vector<string> &ans, int level) {
        int indent = (level*4);
        if (debug) printf("%*si:%d, currentPair.size():%d, currentValUsedDecimalPt:%d, currentPair.back():%s\n", indent, "", i, currentPair.size(), currentValUsedDecimalPt, currentPair.back().c_str());

        if (i == S.length()) {
            if (currentPair.size() == 2) {
                if (!currentValUsedDecimalPt || (currentValUsedDecimalPt && '0' != currentPair.back().back())) {
                    ans.push_back("(" + currentPair[0] + ", " + currentPair[1] + ")");
                    if (debug) printf("%*s\tpushed currentPair into ans\n", indent, "");
                }
            }
            return;
        }
                
        if (currentPair.back().length() == 1 && currentPair.back()[0] == '0') {
            // Leading 0, must use decimal point or comma immediately.
            currentPair.back().push_back('.');
            dfs(i, currentPair, true, S, ans, level+1);
            currentPair.back().pop_back();

            if (currentPair.size() == 1) {
                currentPair.push_back("");
                dfs(i, currentPair, false, S, ans, level+1);
                currentPair.pop_back();
            }
            return;
        }

        currentPair.back().push_back(S[i]);
        dfs(i+1, currentPair, currentValUsedDecimalPt, S, ans, level+1);
        currentPair.back().pop_back();
        
        if (currentPair.back().length() > 0 && !currentValUsedDecimalPt) {
            // Use decimal point
            currentPair.back().push_back('.');
            dfs(i, currentPair, true, S, ans, level+1);
            currentPair.back().pop_back();
        }

        if (currentPair.back().length() > 0 && 1 == currentPair.size()) {
            // Use comma
            if (isdigit(currentPair.back().back())) {
                if (!currentValUsedDecimalPt || (currentValUsedDecimalPt && '0' != currentPair.back().back())) {
                    currentPair.push_back("");
                    dfs(i, currentPair, false, S, ans, level+1);
                    currentPair.pop_back();
                }
            }
        }
    }
    
    vector<string> ambiguousCoordinates(string S) {
        string trimmedS = S.substr(1, S.length()-2);
        vector<string> ans;
        vector<string> currentPair = {""}; 
        dfs(0, currentPair, false, trimmedS, ans, 0);
        return ans;
    }
};
