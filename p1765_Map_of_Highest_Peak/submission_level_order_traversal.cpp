bool debug = false;
typedef vector<int> VI;
typedef pair<int, int> PII;
    
int const MAXN = 1000;
int const INVALID = -1;
int h[MAXN][MAXN]; // assigned heights

int dy[] = {1, -1, 0, 0};
int dx[] = {0, 0, 1, -1};

class Solution {
public:
    vector<vector<int>> highestPeak(vector<vector<int>>& isWater) {
        int yUpper = isWater.size(), xUpper = isWater[0].size();
        // [WARNING] Using "priority_queue<VI, vector<VI>, greater<VI>> pq of {height, y, x}" here will result in TLE!
        queue<PII> pq;
        
        memset(h, INVALID, sizeof h);
        for (int y = 0; y < yUpper; ++y)
            for (int x = 0; x < xUpper; ++x) 
                if (isWater[y][x]) {
                    pq.push({y, x});
                    h[y][x] = 0;
                }
        
        
        while (!pq.empty()) {
            int sz = pq.size();
            while (sz--) {
                auto &t = pq.front();
                int y = t.first, x = t.second, curH = h[y][x];
                for (int i = 0; i < 4; ++i) {
                    int ny = y+dy[i], nx = x+dx[i];
                    if (ny >= yUpper || ny < 0 || nx >= xUpper || nx < 0) continue;
                    if (INVALID != h[ny][nx]) continue;

                    h[ny][nx] = curH + 1; // curH is guaranteed the lowest in pq
                    pq.push({ny, nx});
                }
                pq.pop();
            }
        }
        
        vector<vector<int>> ans;
        for (int y = 0; y < yUpper; ++y) {
            vector<int> row;
            for (int x = 0; x < xUpper; ++x) {
                row.push_back(h[y][x]);
            }
            ans.push_back(row);
        }
        
        return ans;
    }
};
