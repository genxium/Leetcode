bool debug = false;
int dy[] = {-1, -1, 0, 0, +1, +1};
int dx[] = {-1, +1, -1, +1, -1, +1};

bool hasAugmentedPath(int u, vector<vector<int>> &g, vector<int> &mt, vector<bool> &used) {
    if (used[u]) return false;
    // Notation convention: 'u' on the left (for even seats), 'v' on the right (for odd seats)
    used[u] = true;
    for (int v : g[u]) {
        int uu = mt[v];
        if (-1 == uu || hasAugmentedPath(uu, g, mt, used)) {
            // [WARNING] In the latter case, the existing value of "mt[to]" can be updated!
            if (debug) printf("assigning mt[v:%d] = u:%d\n", v, u);
            mt[v] = u;
            return true;
        }
    }
    return false;
}

int hungarianMaxBipartite(vector<vector<int>> &g, int maxEvenId, int maxOddId) {
    vector<int> mt(maxOddId+1, -1); // the "matching even seat" for the odd seat
    vector<bool> used(maxEvenId+1, false); // whether or not an "even seat" is used in the current dfs
    
    for (int v = 0; v <= maxEvenId; ++v) {
        used.assign(maxEvenId+1, false); // [WARNING] Resets every cycle.
        hasAugmentedPath(v, g, mt, used);
    }
    
    int ret = 0;
    for (int u : mt) {
        if (-1 == u) continue;
        ++ret;
    }
    
    return ret;
}

/*
test cases
[["#",".","#","#",".","#"],[".","#","#","#","#","."],["#",".","#","#",".","#"]]
[[".","#"],["#","#"],["#","."],["#","#"],[".","#"]]
[["#",".",".",".","#"],[".","#",".","#","."],[".",".","#",".","."],[".","#",".","#","."],["#",".",".",".","#"]]
*/
class Solution {
public:
    int maxStudents(vector<vector<char>>& seats) {
        /*
        Put all odd-column-seats on the left side and all even-column-seats on the right. If any pair of (odd, even) seats both exist, then put an edge between them.
        
        Now that any match in this new graph indicates an amount of seats that must be excluded. We should find the maximum match as the number of seats that must be excluded.
        */
        
        int yUpper = seats.size(), xUpper = seats[0].size();        
        vector<vector<int>> g; // adjacency list for the even seats
        
        int oddId = -1;
        vector<vector<int>> oddIds(yUpper, vector<int>(xUpper, -1));
        for (int x = 0; x < xUpper; x += 2) {
            for (int y = 0; y < yUpper; ++y) {
                if ('#' == seats[y][x]) continue;
                g.push_back({});
                for (int i = 0; i < 6; ++i) {
                    int ny = y + dy[i], nx = x + dx[i];
                    if (0 > ny || ny >= yUpper || 0 > nx || nx >= xUpper) continue;
                    if ('#' == seats[ny][nx]) continue;
                    int oddNeighbourId = (-1 == oddIds[ny][nx] ? ++oddId : oddIds[ny][nx]);
                    oddIds[ny][nx] = oddNeighbourId;
                    g.back().push_back(oddNeighbourId);
                }
                if (debug) printf("g[evenId:%d] has %d connected edges\n", g.size()-1, g.back().size());
            }
        }
        
        int oddSeats = 0;
        for (int x = 1; x < xUpper; x += 2) {
            for (int y = 0; y < yUpper; ++y) {
                if ('#' == seats[y][x]) continue;
                ++oddSeats;
            }
        }
                
        int toExclude = hungarianMaxBipartite(g, g.size()-1, oddId);
        if (debug) printf("There're %d even seats, %d odd seats, %d to exclude seats\n", g.size(), oddSeats, toExclude);
        int totSeats = g.size()+oddSeats;
        return totSeats - toExclude;
    }
};
