#define MAXY 500
#define MAXX 500
#define INVALID -1

#define VI vector<int>
#define PI pair<int, int>
VI dirx{0, 0, -1, +1};
VI diry{-1, +1, 0, 0};

bool closedSet[MAXY][MAXX];
bool inQueue[MAXY][MAXX];

class Solution {
public:   
    bool containsCycle(vector<vector<char>>& grid) {
        memset(closedSet, false, sizeof(closedSet));
        memset(inQueue, false, sizeof(inQueue));
        
        int yUpper = grid.size(), xUpper = grid[0].size();
        vector<PI> q; q.reserve(MAXY*MAXX);
        int qHead = 0;

        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                // init
                char ref = grid[y][x];
                q.clear(); qHead = 0;
                q.push_back({y, x});
                inQueue[y][x] = true;
                
                // loop
                while (!q.empty() && qHead < q.size()) {
                    int sz = q.size()-qHead; // using "LevelOrderTraversal"
                    for (int k = 0; k < sz; ++k) {
                        PI expander = q[qHead]; ++qHead;
                        int ey = expander.first, ex = expander.second;
                        inQueue[ey][ex] = false;
                        closedSet[ey][ex] = true;

                        for (int i = 0; i < 4; ++i) {
                            int ny = ey+diry[i], nx = ex+dirx[i];
                            if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                            if (grid[ny][nx] != ref) continue;
                            if (closedSet[ny][nx]) {
                                // Elements in "closedSet" prohibits "going backward".
                                continue;
                            }
                            if (inQueue[ny][nx]) {
                                return true;
                            }
                            q.push_back({ny, nx});
                            inQueue[ny][nx] = true;
                        }        
                    }
                }
            }
        }
        
        return false;
    }
}; 
