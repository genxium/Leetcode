bool debug = false;
int const MAXCOLOR = 60;
int const INVALID = -1;
typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ_LIST;
int lmost[MAXCOLOR+1], rmost[MAXCOLOR+1], tmost[MAXCOLOR+1], bmost[MAXCOLOR+1];

/*
test cases
[[1,1,1,1],[1,2,2,1],[1,2,2,1],[1,1,1,1]]
[[1,1,1,1],[1,1,3,3],[1,1,3,4],[5,5,1,4]]
[[1,2,1],[2,1,2],[1,2,1]]
[[1,1,1],[3,1,3]]
[[1,2,3],[3,2,1]]
[[6,2,2,5],[2,2,2,5],[2,2,2,5],[4,3,3,4]]
[[4,4,4,1,1,1,1,1,1,1],[4,4,4,1,1,1,1,1,1,1],[4,4,4,3,1,1,1,1,1,1],[1,3,3,3,1,1,1,1,1,1],[1,3,3,3,1,1,1,1,1,1],[1,3,3,3,1,1,1,1,1,1],[1,3,3,6,6,6,6,6,6,5],[1,3,3,6,6,6,6,6,6,5],[1,1,1,6,6,6,6,6,6,2]]
[[1,1,1,1,1,29,29,29,29,29,29,2,1,25,25],[1,1,1,1,1,1,1,1,1,1,1,2,1,25,25],[1,1,1,26,26,26,26,26,26,26,26,26,26,25,25],[1,1,1,26,26,26,26,26,26,26,26,26,26,25,25],[1,1,1,26,26,26,26,26,26,26,26,26,26,7,1],[1,1,1,26,26,26,26,26,26,26,26,26,26,7,1],[1,1,23,26,26,26,26,26,26,26,26,26,26,20,1],[1,1,23,23,10,22,22,22,22,22,22,22,22,20,17],[1,18,23,23,18,18,18,18,16,20,20,20,20,20,15],[1,18,18,18,31,31,31,31,31,31,20,20,20,20,15],[1,11,11,11,31,31,31,31,31,31,20,20,20,20,15],[1,1,1,4,27,27,27,27,30,30,30,27,27,27,27],[1,1,1,1,27,27,27,27,30,30,30,28,28,27,27],[1,1,1,1,27,27,27,27,30,30,30,27,27,27,27]]
*/
class Solution {
public:
    vector<int> topsort(ADJ_LIST &adj) {
        // Topological sorting of a DirectedGraph, returns an array whose size is less than adj.size() if a ring exists; otherwise return an array such that if "u can reach v", then "u precedes v in the returned array".
        unordered_map<int, int> inDegree;
        for (auto &[u, nbs] : adj) {
            for (auto v : nbs) {
                ++inDegree[v];
            }
        }

        queue<int> q; // Use a queue to do it iteratively.
        vector<int> toRet;
        
        // init
        for (auto &[u, nbs] : adj) {
            if (inDegree[u]) continue;
            q.push(u);
        }

        // loop
        while (!q.empty()) {
            auto u = q.front(); q.pop();
            toRet.push_back(u);
            for (auto v : adj[u]) {
                --inDegree[v];
                if (0 != inDegree[v]) continue;
                q.push(v);
            }
        }

        return toRet;
    }
    
    bool isPrintable(vector<vector<int>>& targetGrid) {
        int yUpper = targetGrid.size(), xUpper = targetGrid[0].size();
        memset(lmost, INVALID, sizeof lmost);
        memset(rmost, INVALID, sizeof rmost);
        memset(tmost, INVALID, sizeof tmost);
        memset(bmost, INVALID, sizeof bmost);
        
        unordered_set<int> colors;
        // traverse each row
        for (int y = 0; y < yUpper; ++y) {
            for (int x = 0; x < xUpper; ++x) {
                int currentColor = targetGrid[y][x]; 
                colors.insert(currentColor);
                if (INVALID == lmost[currentColor] || lmost[currentColor] > x) lmost[currentColor] = x;
                if (INVALID == tmost[currentColor] || tmost[currentColor] > y) tmost[currentColor] = y;
                if (INVALID == rmost[currentColor] || rmost[currentColor] < x) rmost[currentColor] = x;
                if (INVALID == bmost[currentColor] || bmost[currentColor] < y) bmost[currentColor] = y;
            }
        }
        
        ADJ_LIST adj;
        for (auto color : colors) {
            for (int y = tmost[color]; y <= bmost[color]; ++y) {
                for (int x = lmost[color]; x <= rmost[color]; ++x) {
                    int otherColor = targetGrid[y][x];
                    if (otherColor == color) continue;
                    adj[color].insert(otherColor); // must paint "color" prior to "otherColor"
                }
            }
        }
            
        vector<int> orderedToPaintColors = topsort(adj);
        
        if (debug) {
            printf("orderedToPaintColors[]: ");
            for (auto color : orderedToPaintColors) {
                printf("%d ", color);
            } puts("");
        }
        
         // If there's a cycle in "adj", we cannot produce a valid answer.
        if (orderedToPaintColors.size() < adj.size()) {
            if (debug) printf("There's a cycle in adj\n");
            return false;
        }
        
        return true;
    }
};
