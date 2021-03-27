bool debug = false;
int dy[] = {+1, 0, -1, 0};
int dx[] = {0, +1, 0, -1};

/*
test cases
[4,-1,3]
[]
[4,-1,4,-2,4]
[[2,4]]
[-2,-2,9]
[[0,-7]]
[-2,3,7]
[[4,0],[0,0]]
[-2,-1,-2,3,7]
[[1,-3],[2,-3],[4,0],[-2,5],[-5,2],[0,0],[4,-4],[-2,-5],[-1,-2],[0,2]]
[2,2,5,-1,-1]
[[-3,5],[-2,5],[3,2],[5,0],[-2,0],[-1,5],[5,-3],[0,0],[-4,4],[-3,4]]
*/
class Solution {
public:
    int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {
        unordered_map<int, set<int>> sameYObs, sameXObs;
        int ans = 0;
        
        for (auto &obs : obstacles) {
            int y = obs[1], x = obs[0];
            sameYObs[y].insert(x);
            sameXObs[x].insert(y);
        }
        
        vector<int> pos = {0, 0}; // the pos is {y, x} as my preference
        int dir = 0; 
        for (auto &command : commands) {
            int cury = pos[0], curx = pos[1];
            if (-1 == command) {
                dir = (dir+1)%4;
            } else if (-2 == command) {
                dir = (dir-1+4)%4;
            } else {
                if (dir == 0 || dir == 2) {
                    // Moves in y dir
                    auto it1 = dir == 0 ? sameXObs[curx].upper_bound(cury) : sameXObs[curx].lower_bound(cury);
                    int latery = cury + dy[dir]*command;
                    auto it2 = dir == 0 ? sameXObs[curx].upper_bound(latery) : sameXObs[curx].lower_bound(latery);
                    
                    if (it1 != it2) {
                        // Meets an obstable while moving
                        if (dir == 0) {
                            pos[0] = *it1-1;
                        } else {
                            if (it1 != sameXObs[curx].begin()) {
                                --it1;
                                pos[0] = *it1+1;                                
                            } else {
                                pos[0] = latery;
                            }
                        }
                    } else {
                        pos[0] = latery;
                    }
                } else {
                    // Moves in x dir
                    auto it1 = dir == 1 ? sameYObs[cury].upper_bound(curx) : sameYObs[cury].lower_bound(curx);
                    int laterx = curx + dx[dir]*command;
                    auto it2 = dir == 1 ? sameYObs[cury].upper_bound(laterx) : sameYObs[cury].lower_bound(laterx);
                    
                    if (it1 != it2) {
                        // Meets an obstable while moving
                        if (dir == 1) {
                            pos[1] = *it1-1;
                        } else {
                            if (it1 != sameYObs[cury].begin()) {
                                --it1;
                                pos[1] = *it1+1;                                
                            } else {
                                pos[1] = laterx;
                            }
                        }
                    } else {
                        pos[1] = laterx;
                    }
                }
            }
            if (debug) printf("now at {y:%d, x:%d}\n", pos[0], pos[1]);
            ans = max(ans, pos[0]*pos[0]+pos[1]*pos[1]);
        }
        
        return ans;
    }
};
