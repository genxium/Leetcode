bool debug = true;
int const INVALID = -1;
int const MAXN = 50;
int const MAXPOS = MAXN*MAXN; // 2500, approx. (1 << 11)

typedef pair<int, int> PII;

int dy[] = {+1, 0};
int dx[] = {0, +1};

struct PairHash {
    size_t operator() (PII const& operand) const {
        // return (operand.first^operand.second);
        // return operand.first*MAXPOS + operand.second;
        // return ((operand.first << 12) | operand.second);
        return ((operand.first << 1) | operand.second); // Any one will work!
    }
};

/*
test cases
[[0,1,-1],[1,0,-1],[1,1,1]]
[[1,1,-1],[1,-1,1],[-1,1,1]]
*/
class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        /*
        The problem is equal to choosing 2 paths from (0, 0) to (n-1, n-1) to maximize the number of picked up cherries. 
        
        Denote "pos1" as the position in "path1" and "pos2" as the position in "path2", at any instant, "pos1 == y1*n+x1" && "pos2 == y2*n+x2" && "ManhattanDistance(pos1, 0) == ManhattanDistance(pos1, (n-1*n+n-1))".
        */
        int n = grid.size();
        unordered_set<PII, PairHash> openSet, closedSet;
        unordered_map<PII, int, PairHash> dp;
        queue<PII> q;
        
        // init
        dp[{0, 0}] = (grid[0][0]);
        openSet.insert({0, 0});
        q.push({0, 0});
        
        // loop
        while (!q.empty()) {
            auto f = q.front(); q.pop();
            openSet.erase(f);
            closedSet.insert(f);
            int pos1 = f.first, pos2 = f.second;
            int y1 = pos1/n, x1 = pos1%n;
            int y2 = pos2/n, x2 = pos2%n;
            for (int i = 0; i < 2; ++i) {
                int ny1 = y1 + dy[i], nx1 = x1 + dx[i];
                if (ny1 < 0 || ny1 >= n || nx1 < 0 || nx1 >= n) continue;
                if (INVALID == grid[ny1][nx1]) continue;
                for (int j = 0; j < 2; ++j) {
                    int ny2 = y2 + dy[j], nx2 = x2 + dx[j];
                    if (ny2 < 0 || ny2 >= n || nx2 < 0 || nx2 >= n) continue;
                    if (INVALID == grid[ny2][nx2]) continue;
                    
                    // The actual propagation.
                    int npos1 = ny1*n + nx1, npos2 = ny2*n + nx2;
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
        
        int targetPos1 = (n-1)*n+n-1, targetPos2 = targetPos1; 
        return dp[{targetPos1, targetPos2}];
    }
};
