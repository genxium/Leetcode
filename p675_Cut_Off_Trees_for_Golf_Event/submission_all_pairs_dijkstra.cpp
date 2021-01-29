bool const debug = false;
typedef pair<int, int> PII;
typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ;

int const MAXY = 50, MAXX = 50;
int const MAXN = MAXY*MAXX;
int const INVALID = -1;

int dy[] = {+1, -1, 0, 0};
int dx[] = {0, 0, +1, -1};

int g[MAXN][MAXN];

int activeSrcPos = INVALID;

class MinHeapCmp {
public:
    bool operator() (int const &lhs, int const &rhs) const {
        // Note that "INVALID" should be treated as "INFINITE".
        if (INVALID == g[activeSrcPos][lhs] && INVALID == g[activeSrcPos][rhs]) return true;
        if (INVALID == g[activeSrcPos][lhs]) return true;
        if (INVALID == g[activeSrcPos][rhs]) return false;
        return g[activeSrcPos][lhs] > g[activeSrcPos][rhs];
    }
};

/*
test cases
[[1,2,3],[0,0,4],[7,6,5]]
[[1,2,3],[0,0,0],[7,6,5]]
[[2,3,4],[0,0,5],[8,7,6]]
*/
class Solution {
public: 
    void modifiedDijkstra(int s, ADJ &adj, int g[][MAXN], bool* closedSet) {  
        // Deliberately using a "priority_queue" with just "int" instead of "pair<int, int>" to speed up.
        activeSrcPos = s;
        
        //priority_queue< PII, vector<PII>, greater<PII> > openHeap;
        priority_queue< int, vector<int>, MinHeapCmp > openHeap;
        
        // init
        //openHeap.push({0, s});
        openHeap.push(s);
        g[s][s] = 0;
        
        while (!openHeap.empty()) {
            //auto expander = openHeap.top(); openHeap.pop();
            //int t = expander.second;
            int t = openHeap.top(); openHeap.pop();
            if (closedSet[t]) {
                for (auto &it : adj) {
                    int v = it.first;
                    if (INVALID == g[s][t] || INVALID == g[t][v]) continue;
                    int cand = g[s][t] + g[t][v]; 
                    if (INVALID == g[s][v] || cand < g[s][v]) {
                        g[s][v] = cand;
                    }
                }
            } else {
                for (auto v : adj[t]) {
                    if (INVALID == g[s][t]) continue;
                    int cand = g[s][t] + 1; // weight of "t -> v" is always 1 in this problem
                    if (INVALID == g[s][v] || cand < g[s][v]) {
                        g[s][v] = cand;
                        //openHeap.push({cand, v});
                        openHeap.push(v);
                    }
                }
            }
        }
        
        closedSet[s] = true;
    }

    int cutOffTree(vector<vector<int>>& forest) {
        int yUpper = forest.size(), xUpper = forest[0].size();
        
        ADJ adj;
        vector<PII> trees;
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                int pos = y*xUpper+x;
                int tree = forest[y][x];
                if (0 == tree) continue;
                
                for (int i = 0; i < 4; ++i) {
                    int ny = y+dy[i], nx = x+dx[i];
                    if (ny < 0 || ny >= yUpper || nx < 0 || nx >= xUpper) continue;
                    if (0 == forest[ny][nx]) continue;
                    int nbPos = ny*xUpper + nx;
                    adj[pos].insert(nbPos);
                }
                
                if (tree <= 1) continue;
                trees.push_back({tree, pos});
            }
        }
        
        trees.push_back({0, 0});
        sort(trees.begin(), trees.end(), less<PII>());
        
        bool closedSet[MAXN]; 
        memset(closedSet, false, sizeof closedSet);
        
        memset(g, INVALID, sizeof g);
        
        for (int i = 0; i < trees.size(); ++i) {
            int treePos = trees[i].second;
            modifiedDijkstra(treePos, adj, g, closedSet);
            if (debug) {
                printf("after traversing trees[i:%d].position: %d\n", i, treePos);
                for (int j = 0; j < trees.size(); ++j) {
                    if (j == i) continue;
                    int thatTreePos = trees[j].second;
                    printf("g[iPos:%d][jPos:%d] == %d\n", treePos, thatTreePos, g[treePos][thatTreePos]);
                }
            }
        }
        
        int ans = 0;
        for (int i = 0; i < trees.size()-1; ++i) {
            int treePos = trees[i].second;
            int nextTreePos = trees[i+1].second;
            if (INVALID == g[treePos][nextTreePos]) {
                return INVALID;
            }
            ans += g[treePos][nextTreePos];
        }
        
        return ans;
    }
};
