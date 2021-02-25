typedef vector<int> VI;
typedef unordered_set<int> ADJ_ELE;
typedef unordered_map<int, ADJ_ELE> ADJ_LIST;

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

/*
test cases
[[1,2],[2,3],[5],[0],[5],[],[]]
[[1,2,3,4],[1,2],[3,4],[0,4],[]]
[[1],[2],[3],[0]]
*/
class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        ADJ_LIST adj; // A reverse of the original graph, a cycle will remain a cycle.
        for (int u = 0; u < graph.size(); ++u) {
            adj[u] = ADJ_ELE{};
        }
        for (int u = 0; u < graph.size(); ++u) {
            auto &nbs = graph[u];
            for (int v : nbs) {
                adj[v].insert(u);
            }
        }
        
        vector<int> notInCycle = topsort(adj);
        sort(notInCycle.begin(), notInCycle.end());
        return notInCycle;
    }
};
