int const MAXPOS = 50;
typedef pair<int,int> PII;

struct PairHash {
    size_t operator() (PII const& operand) const {
        return operand.first*MAXPOS + operand.second;
    }
};

class Solution {
public:
    int projectionArea(vector<vector<int>>& grid) {
        unordered_set<PII, PairHash> xy, yz, xz;
        
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[y].size(); ++x) {
                for (int z = 0; z < grid[y][x]; ++z) {
                    xy.insert({x, y});
                    yz.insert({y, z});
                    xz.insert({x, z});
                }
            }
        }
        
        return xy.size() + yz.size() + xz.size();
    }
};
