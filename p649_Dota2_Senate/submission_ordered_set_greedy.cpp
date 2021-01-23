/*
test cases
"RD"
"RDD"
"DRDRR"
"RDRDD"
*/
class Solution {
public:
    string predictPartyVictory(string senate) {
        int lastPos = -1;
        set<int> rPos, dPos;
        for (int i = 0; i < senate.length(); ++i) senate[i] == 'R' ? rPos.insert(i) : dPos.insert(i);
        while (!rPos.empty() && !dPos.empty()) {
            auto rCand = rPos.upper_bound(lastPos);
            auto dCand = dPos.upper_bound(lastPos);
            if (rCand == rPos.end() && dCand == dPos.end()) {
                lastPos = -1;
                continue;
            }
            if (rCand != rPos.end() && dCand != dPos.end()) {
                if (*rCand < *dCand) {
                    lastPos = *rCand;
                    auto toDelIt = dPos.upper_bound(lastPos);
                    if (toDelIt == dPos.end()) {
                        toDelIt = dPos.begin();
                    }
                    
                    dPos.erase(toDelIt);
                } else {
                    lastPos = *dCand;
                    auto toDelIt = rPos.upper_bound(lastPos);
                    if (toDelIt == rPos.end()) {
                        toDelIt = rPos.begin();
                    }
                    
                    rPos.erase(toDelIt);
                }
            } else if (rCand != rPos.end()) {
                lastPos = *rCand;
                dPos.erase(dPos.begin());
            } else {
                lastPos = *dCand;
                rPos.erase(rPos.begin());
            }
        }
        
        if (rPos.empty()) return "Dire";
        else return "Radiant";
    }
};
