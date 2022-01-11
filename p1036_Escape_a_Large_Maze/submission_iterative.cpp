bool const debug = false;
typedef pair<int, int> PII;
typedef vector<int> VI;
VI dirx{0, 0, -1, +1};
VI diry{-1, +1, 0, 0};

struct PairHash {
    size_t operator() (PII const& operand) const {
        // Any one will work!
        return (operand.first^operand.second);
        // return ((operand.first << 12) | operand.second);
        // return ((operand.first << 20) | operand.second); 
    }
};

/*
test cases
[[0,1],[1,0]]
[0,0]
[0,2]
[]
[0,0]
[999999,999999]
*/
class Solution {
public:
    bool isEscapePossible(vector<vector<int>>& blocked, vector<int>& source, vector<int>& target) {
        unordered_set<PII, PairHash> blockedSet;
        int nBlocks = blocked.size();
        for (auto &loc : blocked) {
            PII newLoc(loc[0], loc[1]);
            blockedSet.insert(newLoc);
        }
        int maxX = 1000000, maxY = 1000000;
        PII src(source[0], source[1]);
        PII dst(target[0], target[1]);

        return canEscape(src, dst, blockedSet, nBlocks, maxX, maxY) && canEscape(dst, src, blockedSet, nBlocks, maxX, maxY);
    }

    bool canEscape(PII source, PII target, unordered_set<PII, PairHash>& blocked, int nBlocks, int maxX, int maxY) {
        /*
        Max possible area given circumference is achieved by a circle, while in this problem.
        
        The "circle" can be a semi-circle formed by an "8-dir connected blocks", yet more aggressively, the "circle" can be just quater-semi-circle at a corner where the circumference is just 1/4 of a full circle.

        A truely safe way to assert that "source" can escape is by checking the "level" that's expandible from it, i.e. should be less than nBlocks if bounded by all possible blocks.
        */
        int level = 0, maxLevel = nBlocks;
        queue<PII> q;
        q.push(source);
        bool closedSet[401][401];
        memset(closedSet, 0, sizeof closedSet);

        while (!q.empty()) {
            // level traversal 
            int sz = q.size();
            while (sz--) {
                PII expander = q.front(); q.pop();
                int offsetX = 200 + (expander.first - source.first);
                int offsetY = 200 + (expander.second - source.second);
                if (closedSet[offsetX][offsetY]) continue; // Thus no need to check when traversing neighbours.
                closedSet[offsetX][offsetY] = true;
                for (int i = 0; i < 4; ++i) {
                    int nx = expander.first + dirx[i];
                    int ny = expander.second + diry[i];
                    if (nx < 0 || ny < 0 || nx >= maxX || ny >= maxY) continue;
                    if (nx == target.first && ny == target.second) {
                        if (debug) printf("Escaping by meeting target\n");
                        return true;
                    }
                    PII newLoc(nx, ny);
                    if (blocked.count(newLoc)) continue;
                    q.push(newLoc);
                }
            }
            ++level;
            if (level > maxLevel) {
                if (debug) printf("Escaping when level == %d\n", level);
                return true;
            }
        }
        return false;
    }
};
