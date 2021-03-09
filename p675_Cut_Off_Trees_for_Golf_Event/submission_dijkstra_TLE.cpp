bool const debug = false;
typedef pair<int, int> PII;
typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ;

int const MAXY = 50, MAXX = 50;
int const MAXN = MAXY*MAXX;
int const INVALID = -1;

int dy[] = {+1, -1, 0, 0};
int dx[] = {0, 0, +1, -1};

/*
test cases
[[1,2,3],[0,0,4],[7,6,5]]
[[1,2,3],[0,0,0],[7,6,5]]
[[2,3,4],[0,0,5],[8,7,6]]
*/
class Solution {
public:
    int dijkstra(int start, int end, ADJ &adj) {        
        // See https://www.yinxiang.com/everhub/note/8f531e5d-f4ab-4164-a459-266ab4b8cda8 for basics of the Dijkstra algorithm.
        priority_queue< PII, vector<PII>, greater<PII> > openHeap;
        bool closedSet[MAXN]; 
        memset(closedSet, false, sizeof closedSet);
        
        unordered_map<int, int> g; // "g[u] == x" being the shortest path to reach "u" from "start"

        // init
        openHeap.push({0, start});
        g[start] = 0;

        // loop
        while (!openHeap.empty()) {
            auto expander = openHeap.top(); openHeap.pop();
            int &u = expander.second;

            closedSet[u] = true;
            if (end == u) {
              return g[u];
            }

            auto nbs = adj[u];
            for (auto &v : nbs) {
                if (closedSet[v]) continue;

                int candidate = g[u] + 1;
                if (!g.count(v) || candidate < g[v]) {
                    g[v] = candidate;
                    openHeap.push({candidate, v});
                }
            }
        }

        return INVALID;    
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
        
        sort(trees.begin(), trees.end(), less<PII>());
        int lastPos = 0, ans = 0;
        for (auto &tree : trees) {
            int inc = dijkstra(lastPos, tree.second, adj);
            if (debug) {
                printf("From lastPos:{y:%d, x:%d} to treePos: {y:%d, x:%d}, inc == %d\n", lastPos/xUpper, lastPos%xUpper, tree.second/xUpper, tree.second%xUpper, inc);   
            }
            if (INVALID == inc) return INVALID;
            ans += inc;
            lastPos = tree.second;
        }
        
        return ans;
    }
};
