bool const debug = false;
typedef vector<int> VI;
int const INVALID = -1;
int const DISJOINT_SET_NOT_FOUND = -1;
int const MAXPOS = 40000;
int djsParent[MAXPOS+1]; // "DisjointSetParent"
int djsSize[MAXPOS+1];

int djsFindRoot(int u) {
    int cur = u;
    int p = djsParent[cur];
    if (DISJOINT_SET_NOT_FOUND == p) {
        djsParent[u] = u; // lazy init
        djsSize[u] = 1;
        return u;
    }
    while (p != cur) {
        cur = p;
        p = djsFindRoot(cur); // lazy path compression
    }
    djsParent[u] = p;
    return p;
}

int djsUnite(int u, int v) {
    int uRoot = djsFindRoot(u);
    int vRoot = djsFindRoot(v);
    if (uRoot == vRoot) return uRoot;
    
    // [WARNING] Always attach to smaller root!
    if (uRoot < vRoot) {
        djsSize[uRoot] += djsSize[vRoot];
        return djsParent[vRoot] = uRoot;
    } else {
        djsSize[vRoot] += djsSize[uRoot];
        return djsParent[uRoot] = vRoot;
    }
}

int const dy[] = {+1, -1, 0, 0};
int const dx[] = {0, 0, +1, -1};

/*
test cases
[[1,0,0,0],[1,1,1,0]]
[[1,0]]
[[1,0,0,0],[1,1,0,0]]
[[1,1],[1,0]]
[[1],[1],[1],[1],[1]]
[[1,0],[2,0],[0,0]]
[[1],[1],[1],[1],[1]]
[[3,0],[4,0],[1,0],[2,0],[0,0]]
*/
class Solution {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int yUpper = grid.size(), xUpper = grid[0].size();
        
        memset(djsParent, DISJOINT_SET_NOT_FOUND, sizeof djsParent);
        memset(djsSize, 0, sizeof djsSize);
        
        // The trick is to answer the queries in reverse order.
        vector<VI> putbacks;
        
        for (auto &hit : hits) {
            if (grid[hit[0]][hit[1]] == 1) {
                grid[hit[0]][hit[1]] = 0;
                putbacks.push_back(hit);
            } else {
                putbacks.push_back({INVALID, INVALID});
            }
        }
        
        // init special "djsParent"s & "djsSize"s
        int grandRoot = 0; // assume that "grandRoot" is a big, monolithic piece at "y:-1"
        djsParent[grandRoot] = 0;
        djsSize[grandRoot] = 0;
        for (int x = 0; x < xUpper; ++x) {
            int pos = x+1;
            if (0 == grid[0][x]) continue;
            djsUnite(pos, grandRoot);
        }
        
        // init other connected components
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                if (0 == grid[y][x]) continue;
                int pos = y*xUpper+x+1;
                for (int i = 0; i < 4; ++i) {
                    int ny = y+dy[i], nx = x+dx[i];
                    if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                    if (0 == grid[ny][nx]) continue;
                    int npos = ny*xUpper+nx+1;
                    djsUnite(pos, npos);
                }
            }
        }
        
        if (debug) {
            printf("Resulted grid is\n");
            for (int y = 0; y < yUpper; ++y, puts("")) 
                for (int x = 0; x < xUpper; ++x) {
                    printf("%d ", grid[y][x]);
                }
            
            printf("Resulted grandRoot size is %d\n", djsSize[grandRoot]);
        }
        
        VI ans;
        for (auto it = putbacks.rbegin(); it != putbacks.rend(); ++it) {
            auto putback = *it;
            int y = putback[0], x = putback[1];
            if (INVALID == y) {
                ans.push_back(0);
                continue;
            }
            int oldSize = djsSize[grandRoot];
            if (debug) printf("Before putting back (y:%d, x:%d), oldSize is %d\n", y, x, oldSize);
            int putbackPos = y*xUpper+x+1;
            for (int i = 0; i < 4; ++i) {
                int ny = y+dy[i], nx = x+dx[i];
                if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                if (0 == grid[ny][nx]) continue;
                int npos = ny*xUpper+nx+1;
                djsUnite(putbackPos, npos);
            }
            if (0 == y) {
                // Putting back a "top grid".
                djsUnite(putbackPos, grandRoot);
            }
            grid[y][x] = 1;
            if (debug) printf("After putting back (y:%d, x:%d, putbackPos:%d), djsParent[putbackPos]: %d, djsSize[putbackPos]:%d, djsSize[grandRoot] is %d\n", y, x, putbackPos, djsParent[putbackPos], djsSize[putbackPos], djsSize[grandRoot]);
            ans.push_back(max(djsSize[grandRoot]-oldSize-1, 0)); 
        }
        reverse(ans.begin(), ans.end());
        
        return ans;
    }
};
