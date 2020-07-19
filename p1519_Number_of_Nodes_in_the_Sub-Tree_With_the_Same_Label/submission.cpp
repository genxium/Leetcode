#define INVALID -1
#define MAXN 100000
bool visited[MAXN];

class Solution {
public:
    void dfs2(int u, unordered_map<int, list<int>> &adj, string &labels, unordered_map<char, int> &counter, vector<int> &ans) {
        ans[u] = 1;
        list<int> nbs = adj[u];
        ++counter[labels[u]];
        visited[u] = true;
        int origCount = counter[labels[u]];
        for (auto &v : nbs) {
            if (visited[v]) continue;
            unordered_map<char, int> localCounter;
            dfs2(v, adj, labels, counter, ans);
        }
        int count = counter[labels[u]];
        ans[u] += (count - origCount);
    }
    
    void dfs(int u, unordered_map<int, list<int>> &adj, unordered_map<char, list<int>> &revMap, string &labels, vector<int> &ans) {
        visited[u] = true;
        ans[u] = 1;
        if (revMap.find(labels[u]) != revMap.end()) {
            list<int> predecessorIdxes = revMap[labels[u]];
            for (auto &predecessorIdx : predecessorIdxes) {
                ans[predecessorIdx] += 1;
            }
        } else {
            revMap[labels[u]] = list<int>{};            
        }
        revMap[labels[u]].push_back(u);            
        list<int> nbs = adj[u];
        for (auto &v : nbs) {
            if (visited[v]) continue;
            dfs(v, adj, revMap, labels, ans);
        }
        revMap[labels[u]].pop_back(); // backtracking
    }
    
    vector<int> countSubTrees(int n, vector<vector<int>>& edges, string labels) {
        memset(visited, false, sizeof(visited));
        unordered_map<int, list<int>> adj;
        for (auto &edge : edges) {
            int u = edge[0], v = edge[1];
            if (adj.find(u) == adj.end()) {
                adj[u] = list<int>{};
            }
            adj[u].push_back(v);
            
            if (adj.find(v) == adj.end()) {
                adj[v] = list<int>{};
            }
            adj[v].push_back(u);
        }
        unordered_map<char, list<int>> revMap;
        vector<int> ans(n, 0);
        //dfs(0, adj, revMap, labels, ans);
        
        unordered_map<char, int> counter;
        dfs2(0, adj, labels, counter, ans);
        return ans;
    }
};
