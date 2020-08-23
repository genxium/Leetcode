#define INVALID -1
#define MAXN 100
bool visited[MAXN];

class Solution {
private:
    unordered_map<int, unordered_set<int>> predecessors;
    void updatePredecessorsByRevAdj(int &v, int u, unordered_map<int, list<int>> &revAdj) {
        if (visited[u]) {
            return;
        }
        if (u != v) {
            predecessors[v].insert(u);
        }
        auto revNbs = revAdj[u];
        for (auto &revNb : revNbs) {
            updatePredecessorsByRevAdj(v, revNb, revAdj);
        }
        visited[u] = true;
    }

public:
    vector<bool> checkIfPrerequisite(int n, vector<vector<int>>& prerequisites, vector<vector<int>>& queries) {
        unordered_map<int, list<int>> revAdj;
        for (auto &edge : prerequisites) {
            int u = edge[0];
            int v = edge[1];
            
            if (revAdj.find(v) == revAdj.end()) {
                revAdj[v] = {};
            }
            revAdj[v].push_back(u);
        }
    
        for (int v = 0; v < n; ++v) {
            memset(visited, false, sizeof(visited));
            updatePredecessorsByRevAdj(v, v, revAdj);
        }
        
        vector<bool> ans;
        for (auto &query : queries) {
            auto predecessorsOfLatter = predecessors[query[1]];
            if (predecessorsOfLatter.find(query[0]) == predecessorsOfLatter.end()) {
                ans.push_back(false);
            } else {
                ans.push_back(true);
            }
        }
        
        return ans;
    }
};
