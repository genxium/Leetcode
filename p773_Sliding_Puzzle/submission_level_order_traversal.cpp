int swaps[] = {-3, +3, -1, +1}; // up, down, left, right

/*
test cases
[[1,2,3],[4,0,5]]
[[1,2,3],[5,4,0]]
[[4,1,2],[5,0,3]]
[[3,2,4],[1,5,0]]
*/
class Solution {
public:
    int slidingPuzzle(vector<vector<int>>& board) {
        int yUpper = 2, xUpper = 3, bsz = yUpper*xUpper;
        string state = "";
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                state.push_back('0' + board[y][x]);
            }
        }
        
        string target = "123450";
        
        unordered_set<string> openSet, closedSet;
        
        queue<string> q;
        q.push(state);
        openSet.insert(state);
        int lv = 0;
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                auto s = q.front(); q.pop(); openSet.erase(s); closedSet.insert(s);
                if (s == target) return lv;
                int zeroPos = 0;
                while (zeroPos < s.length() && s[zeroPos] != '0') ++zeroPos;
                for (int i = 0; i < 4; ++i) {
                    int swapPos = zeroPos-swaps[i];
                    if (swapPos < 0 || swapPos >= bsz) continue;
                    if (zeroPos%xUpper == xUpper-1 && swapPos%xUpper == 0) {
                        // Prohibits moving to the right when at rightmost
                        continue;
                    }
                    if (zeroPos%xUpper == 0 && swapPos%xUpper == xUpper-1) {
                        // Prohibits moving to the left when at leftmost
                        continue;
                    }
                    string ss(s); swap(ss[swapPos], ss[zeroPos]);
                    if (!openSet.count(ss) && !closedSet.count(ss)) {
                        q.push(ss);
                        openSet.insert(ss);
                    }
                }
            }
            ++lv;
        }   
        
        return -1;
    }
};
