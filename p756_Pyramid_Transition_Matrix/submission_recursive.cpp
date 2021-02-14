/*
test cases
"BCD"
["BCG","CDE","GEA","FFF"]
"AABA"
["AAA", "AAB", "ABA", "ABB", "BAC"]
*/
class Solution {
public:
    bool dfs(string &prevLayer, string &curLayer, int iCurr, int expectedCurLayerSize, unordered_map<char, unordered_map<char, unordered_set<char>>> &whitelist) {        
        char u = prevLayer[iCurr], v = prevLayer[iCurr+1];
        for (auto w : whitelist[u][v]) {
            curLayer.push_back(w);
            if (expectedCurLayerSize == 1) {
                return true;
            } 
            if (curLayer.size() < expectedCurLayerSize) {
                bool cand = dfs(prevLayer, curLayer, iCurr+1, expectedCurLayerSize, whitelist);   
                if (cand) return true;
            } else {
                string newLayer = "";
                bool cand = dfs(curLayer, newLayer, 0, expectedCurLayerSize-1, whitelist);
                if (cand) return true;
            }
            curLayer.pop_back(); // backtrace
        }
        
        return false;
    }
    
    bool pyramidTransition(string bottom, vector<string>& allowed) {
        unordered_map<char, unordered_map<char, unordered_set<char>>> whitelist;
        
        for (auto &single : allowed) {
            whitelist[single[0]][single[1]].insert(single[2]);
        }
        
        string newLayer = "";
        return dfs(bottom, newLayer, 0, bottom.length()-1, whitelist);
    }
};
