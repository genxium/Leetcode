bool debug = false;
int const INVALID = -1;
typedef vector<int> VI;
typedef unordered_set<int> NBS;
typedef unordered_map<int, NBS> ADJ;
int const MAXN = 100000;
int const MAXC = 26;
int dp[MAXN][MAXC]; // dp[i][c] = the biggest count for color "c" of any path starting from node "i" 

/*
test cases
"abaca"
[[0,1],[0,2],[2,3],[3,4]]
"a"
[[0,0]]
"g"
[]
*/
class Solution {
public:
    vector<int> topsort(ADJ &adj) {
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

    int dfs(int i, int c, string& colors, ADJ& adj) {
        if (INVALID != dp[i][c]) return dp[i][c];
        int currentCnt = (colors[i]-'a' == c);
        if (!adj[i].size()) return dp[i][c] = currentCnt;
        for (auto& j : adj[i]) {
            dp[i][c] = max(dp[i][c], currentCnt + dfs(j, c, colors, adj));
        }
        return dp[i][c];
    }

    int largestPathValue(string colors, vector<vector<int>>& edges) {
        memset(dp, INVALID, sizeof dp);
        ADJ adj;
        for (int i = 0; i < colors.size(); ++i) {
            adj[i] = NBS();
        }
        for (VI& edge : edges) {
            adj[edge[0]].insert(edge[1]);
        }
        VI topordered = topsort(adj);
        int n = topordered.size();
        if (debug) {
            printf("n = %d\n", n);
            for (int ki = 0; ki < n; ++ki) printf("%d ", topordered[ki]);
            puts("");
        }
        if (n < colors.size()) return INVALID; // ring exists
        reverse(topordered.begin(), topordered.end());
        int ans = 0;
        for (int ki = 0; ki < n; ++ki) {
            for (int c = 0; c < 26; ++c) {
                int i = topordered[ki];
                int cand = dfs(i, c, colors, adj);
                if (debug) printf("dp[i:%d][c:%c] = %d\n", i, 'a'+c, cand);
                ans = max(ans, cand);
            }
        }
        return ans;
    }
};
