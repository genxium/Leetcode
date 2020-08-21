#define Pqs priority_queue<string, vector<string>, greater<string>>

class Solution {
public:
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
        stack<string> stk; 
        
        // init
        stk.push("JFK");
        
        // loop
        while (!stk.empty()) {
            // mimics the recursive steps
            string from = stk.top();
            while (!adj[from].empty()) {
                auto nb = adj[from].top();
                stk.push(nb);
                adj[from].pop(); // makes sure that each "edge(a.k.a. ticket in this problem)" is used exactly once
                from = stk.top();
            }
            
            string currentTop = stk.top();
            // by now, "adj[currentTop]" must be empty, i.e. either originally or popped
            path.push_front(currentTop);
            stk.pop();
        }
        
        vector<string> ans;
        for (auto &val : path) {
            ans.push_back(val);
        }
        
        return ans;
    }
};
