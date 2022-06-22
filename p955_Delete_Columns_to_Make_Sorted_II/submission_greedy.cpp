/*
test cases
["ca","bb","ac"]
["xc","yb","za"]
["zyx","wvu","tsr"]
["xga","xfb","yfa"]
["abx","agz","bgc","bfc"]
["vdy","vei","zvc","zld"]
*/
bool debug = true;
class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int unorderedCols = 0;
        int yUpper = strs.size(), xUpper = strs[0].size();
        unordered_set<int> strictlyOrderedYsByFar = {}; 
        for (int x = 0; x < xUpper; ++x) {
            int anchor = -1;
            bool isUnorderedCol = false;
            unordered_set<int> tmpStrictlyOrderedYs = {}; // If this is an "unordered col", then "tmpStrictlyOrderedYs" WOULDN'T be added to strictlyOrderedYsByFar!
            for (int y = 0; y < yUpper; ++y) {
                if (strs[y][x] < anchor && !strictlyOrderedYsByFar.count(y)) {
                    isUnorderedCol = true;
                    break;
                }
                if (strs[y][x] > anchor) {
                    tmpStrictlyOrderedYs.insert(y);
                } 
                anchor = strs[y][x];
            }
            if (isUnorderedCol) {
                ++unorderedCols;
            } else {
                for (auto y : tmpStrictlyOrderedYs) strictlyOrderedYsByFar.insert(y);
                if (strictlyOrderedYsByFar.size() == yUpper) break; // all are sorted by far, no need to move on x-direction                
            }
        }
            
        return unorderedCols;
    }
};
