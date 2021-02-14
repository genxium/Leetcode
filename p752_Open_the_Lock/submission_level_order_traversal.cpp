class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> deads;
        for (auto &d : deadends) deads.insert(d);
        
        unordered_set<string> openSet, closedSet;
        
        if (deads.count("0000")) return -1;
        
        queue<string> q;
        q.push("0000");
        openSet.insert("0000");
        
        int lv = 0;
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                auto x = q.front(); q.pop();
                openSet.erase(x);
                closedSet.insert(x);
                if (x == target) return lv;
                for (int i = 0; i < 4; ++i) {
                    string y1(x), y2(x);
                    y1[i] += 1; if (y1[i] > '9') y1[i] = '0';
                    y2[i] -= 1; if (y2[i] < '0') y2[i] = '9';
                    if (false == (openSet.count(y1) || closedSet.count(y1) || deads.count(y1))) {
                        q.push(y1);
                        openSet.insert(y1);
                    }
                    if (false == (openSet.count(y2) || closedSet.count(y2) || deads.count(y2))) {
                        q.push(y2);
                        openSet.insert(y2);
                    }
                }
            }
            ++lv;
        }
        
        return -1;
    }
};
