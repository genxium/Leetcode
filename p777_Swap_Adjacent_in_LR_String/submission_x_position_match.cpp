bool const debug = true;

/*
test cases
"RXXLRXRXL"
"XRLXXRRLX"
"X"
"L"
"LLR"
"RRL"
"XL"
"LX"
"XLLR"
"LXLX"
"LXXLXRLXXL"
"XLLXRXLXLX"
"XXXXXLXXXX"
"LXXXXXXXXX"
*/
class Solution {
public:
    bool canTransform(string start, string end) {
        string stLR = "", edLR = "";
        for (auto ch : start) if ('X' != ch) stLR.push_back(ch);
        for (auto ch : end) if ('X' != ch) edLR.push_back(ch);
        if (stLR != edLR) return false;

        vector<int> stXPos, edXPos;
        for (int i = 0; i < start.length(); ++i) {
            if ('X' != start[i]) continue;
            stXPos.push_back(i);
        }
        for (int i = 0; i < end.length(); ++i) {
            if ('X' != end[i]) continue;
            edXPos.push_back(i);
        }

        if (stXPos.size() != edXPos.size()) return false;

        vector<int> prefixSumOfCountL = {0}, prefixSumOfCountR = {0};
        for (int i = 0; i < start.length(); ++i) {
            prefixSumOfCountL.push_back(prefixSumOfCountL.back() + (start[i] == 'L'));
            prefixSumOfCountR.push_back(prefixSumOfCountR.back() + (start[i] == 'R'));
        }
        
        for (int i = 0; i < stXPos.size(); ++i) {
            if (stXPos[i] < edXPos[i]) {
                // We can have either 'L' or 'X' between start[stXPos[i]+1, edXPos[i]].
                int rCountInBetween = prefixSumOfCountR[edXPos[i]+1] - prefixSumOfCountR[stXPos[i]+1];
                if (rCountInBetween > 0) return false;
            } else if (stXPos[i] > edXPos[i]) {
                // We can have either 'R' or 'X' between start[edXPos[i], stXPos[i]-1].
                int lCountInBetween = prefixSumOfCountL[stXPos[i]] - prefixSumOfCountL[edXPos[i]];
                if (lCountInBetween > 0) return false;
            }
        }

        return true;
    }
};
