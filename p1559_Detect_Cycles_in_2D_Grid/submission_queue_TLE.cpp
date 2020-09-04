#define MAXY 500
#define MAXX 500
#define INVALID -1

#define VI vector<int>
#define PI pair<int, int>
VI dirx{0, 0, -1, +1};
VI diry{-1, +1, 0, 0};

int xUpper;

struct PairHash {
	size_t operator () (pair<int, int> const &v) const {
	    return v.first*xUpper+v.second;
	}
};

class Solution {
public:   
    bool containsCycle(vector<vector<char>>& grid) {
        unordered_set<PI, PairHash> closedSet;

        int yUpper = grid.size();
        xUpper = grid[0].size();
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                // init
                char ref = grid[y][x];
                queue<VI> q;
                unordered_set<PI, PairHash> inQueue;
                q.push({y, x});
                inQueue.insert({y, x});
                
                // loop
                while (!q.empty()) {
                    int sz = q.size(); // using "LevelOrderTraversal"
                    for (int k = 0; k < sz; ++k) {
                        auto expander = q.front(); q.pop();
                        int ey = expander[0], ex = expander[1];
                        inQueue.erase({ey, ex});
                        closedSet.insert({ey, ex});

                        for (int i = 0; i < 4; ++i) {
                            int ny = ey+diry[i], nx = ex+dirx[i];
                            if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                            if (grid[ny][nx] != ref) continue;
                            if (closedSet.find({ny, nx}) != closedSet.end()) {
                                // Elements in "closedSet" prohibits "going backward".
                                continue;
                            }
                            if (inQueue.find({ny, nx}) != inQueue.end()) {
                                return true;
                            }
                            q.push({ny, nx});
                            inQueue.insert({ny, nx});
                        }        
                    }
                }
            }
        }
        
        return false;
    }
}; 
