bool debug = true;
int const INVALID = -1;
int const MAXN = 70;
int const MAXPOS = MAXN*MAXN;

typedef pair<int, int> PII;

int dy[] = {+1, +1, +1};
int dx[] = { 0, +1, -1};

struct PairHash {
    size_t operator() (PII const& operand) const {
        // Any one will work!
        // return (operand.first^operand.second);
        return operand.first*MAXPOS + operand.second;
        // return ((operand.first << 12) | operand.second);
        // return ((operand.first << 20) | operand.second); 
    }
};

/*
[TODO]

At each round, "robot#1" and "robot#2" must be at the same row, thus the space of "dp[pos1][pos2]" can be significantly reduced (e.g. by rotating dp[CUR | PRE][col1][col2]).
*/
class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int yUpper = grid.size(), xUpper = grid[0].size();
        unordered_set<PII, PairHash> openSet, closedSet;
        unordered_map<PII, int, PairHash> dp;
        queue<PII> q;
        
        // init
        int stPos1 = 0, stPos2 = xUpper-1;
        dp[{stPos1, stPos2}] = (grid[0][0] + grid[0][xUpper-1]);
        openSet.insert({stPos1, stPos2});
        q.push({stPos1, stPos2});
        
        // loop
        while (!q.empty()) {
            auto f = q.front(); q.pop();
            openSet.erase(f);
            closedSet.insert(f);
            int pos1 = f.first, pos2 = f.second;
            int y1 = pos1/xUpper, x1 = pos1%xUpper;
            int y2 = pos2/xUpper, x2 = pos2%xUpper;
            for (int i = 0; i < 3; ++i) {
                int ny1 = y1 + dy[i], nx1 = x1 + dx[i];
                if (ny1 < 0 || ny1 >= yUpper || nx1 < 0 || nx1 >= xUpper) continue;
                if (INVALID == grid[ny1][nx1]) continue;
                for (int j = 0; j < 3; ++j) {
                    int ny2 = y2 + dy[j], nx2 = x2 + dx[j];
                    if (ny2 < 0 || ny2 >= yUpper || nx2 < 0 || nx2 >= xUpper) continue;
                    if (INVALID == grid[ny2][nx2]) continue;
                    
                    // The actual propagation.
                    int npos1 = ny1*xUpper + nx1, npos2 = ny2*xUpper + nx2;
                    PII nkey = {npos1, npos2};
                    
                    if (closedSet.count(nkey)) {
                        continue;
                    }
                    
                    /*
                    [WARNING]
                    
                    The "openSet" is ALLOWED to contain "nkey" by now!
                    */
                    dp[nkey] = max(
                        dp[nkey], 
                        dp[f] + (npos1 == npos2 ? grid[ny1][nx1] : grid[ny1][nx1]+grid[ny2][nx2])
                    );
                    
                    if (openSet.count(nkey)) {
                        continue;
                    }
                    q.push(nkey);
                    openSet.insert(nkey);
                }
            }
        }

        int ans = 0;
        int lastRowPosStart = (yUpper-1)*xUpper, lastRowPosEnd = yUpper*xUpper; 
        for (int targetPos1 = lastRowPosStart; targetPos1 < lastRowPosEnd; ++targetPos1) {
            for (int targetPos2 = lastRowPosStart; targetPos2 < lastRowPosEnd; ++targetPos2) {
                ans = max(ans, dp[{targetPos1, targetPos2}]);
            }
        }
        return ans;
    }
};
