bool debug = true;

class Solution {
public:
    vector<int> countPairs(int n, vector<vector<int>>& edges, vector<int>& queries) {
        vector<int> deg(n, 0);
        unordered_map<int, unordered_map<int, int>> adj; // adj[u][v] = shared edges count between (u, v)
        for (auto &edge : edges) {
            int u = --edge[0], v = --edge[1];
            ++deg[u]; ++deg[v];
            ++adj[u][v]; ++adj[v][u];
        }

        vector<int> sortedDeg = deg;
        sort(sortedDeg.begin(), sortedDeg.end());

        vector<int> ans;

        for (auto &q : queries) {
            int ret = 0;

            for (int u = 0; u < n; ++u) {
                // First, assume that "u" has no shared edge with anyone.
                int target = q-deg[u];
                if (target < 0) {
                    // Any pair (u, *) or (*, u) is feasible, the redundant order will be removed later.
                    ret += n-1;
                    continue;
                }

                auto it = upper_bound(sortedDeg.begin(), sortedDeg.end(), target);
                ret += (sortedDeg.end() - it);

                // Then, remove the "false positives".
                if (deg[u] + deg[u] > q) ret -= 1; // itself
                for (auto &[v, sharedEdgesCnt] : adj[u]) {
                    if (deg[u]+deg[v] > q && deg[u]+deg[v]-sharedEdgesCnt <= q) --ret;
                }
            }

            ret /= 2; // Cut the duplicates
            ans.push_back(ret);
        }

        return ans;
    }
};
