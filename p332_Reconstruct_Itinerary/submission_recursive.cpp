#define Pqs priority_queue<string, vector<string>, greater<string>>

class Solution {
public:
    void dfs(string u, unordered_map< string, Pqs > &adj, list<string> &path) {
        auto& nbs = adj[u];
        
        while (!nbs.empty()) {
            string greedyNb = nbs.top();
            nbs.pop();
            dfs(greedyNb, adj, path);   
        }
        path.push_front(u);
    }
    
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        int edgeIdx = 0;
        unordered_map< string, Pqs > adj;
        for (auto &ticket : tickets) {
            string u = ticket[0];
            string v = ticket[1];
            if (adj.end() == adj.find(u)) {
                adj[u] = {};
            }
            adj[u].push(v);
        }
        
        list<string> path;
        dfs("JFK", adj, path);
    
        vector<string> ans;
        for (auto &val : path) {
            ans.push_back(val);
        }
        
        return ans;
    }
};
