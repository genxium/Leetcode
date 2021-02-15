bool debug = false;
typedef vector<int> VI;

int dy[] = {1, -1, 0, 0};
int dx[] = {0, 0, 1, -1};

class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int yUpper = grid.size(), xUpper = grid[0].size();
        
        set<VI> openSet, closedSet;
        priority_queue<VI, vector<VI>, greater<VI>> q; // min-heap
        VI st = {grid[0][0], 0, 0}; // {elevation, y, x}
        
        int t = 0;
        q.push(st);
        while (!q.empty()) {
            auto expander = q.top(); q.pop(); openSet.erase(expander); closedSet.insert(expander);
            if (debug) printf("expander is {elev:%d, y:%d, x:%d}\n", expander[0], expander[1], expander[2]);
            
            t = max(t, expander[0]); // increases the "time" to the next critical value
            
            // By now (i.e. time "t") we can teleport to (expander[1], expander[2]) by any point in "closedSet".

            if (expander[1] == yUpper-1 && expander[2] == xUpper-1) {
                return t;
            }
            for (int i = 0; i < 4; ++i) {
                int ny = expander[1] + dy[i], nx = expander[2] + dx[i];
                if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                VI newState = {grid[ny][nx], ny, nx};
                if (openSet.count(newState) || closedSet.count(newState)) continue;
                q.push(newState);
                openSet.insert(newState);
            }
        }
        
        return -1;
    }
};
