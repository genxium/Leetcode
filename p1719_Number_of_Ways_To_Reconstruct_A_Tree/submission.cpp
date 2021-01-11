typedef vector<int> VI;
typedef pair<int, int> PII;
int const INVALID = -1;

class Solution {
public:
    int checkWays(vector<vector<int>>& pairs) {
        unordered_map<int, unordered_set<int>> adj;
        for (auto &pair : pairs) {
            adj[pair[0]].insert(pair[1]);
            adj[pair[1]].insert(pair[0]);
        }
        
        int n = adj.size(); // number of nodes
        
        priority_queue<PII, vector<PII>, less<PII>> pq; // max-heap
        for (auto &it : adj) {
            pq.push({it.second.size(), it.first}); // {degrees, nodeId}
        }
        
        bool multiple = false;
        unordered_set<int> visited;
        while (!pq.empty()) {
            PII t = pq.top(); pq.pop();
            int v = t.second, vDegree = t.first;
            /*
            Find parent of "v" == u", which should 
            - be a "visited node with smallest degree, and 
            - be reachable by "u", i.e. "u in adj[v]"
            - suffice that "any x in adj[v] is in adj[u]"
            */
            
            int u = INVALID, uDegree = INT_MAX;
            for (auto x : adj[v]) {
                if (visited.count(x) && adj[x].size() < uDegree) {
                    uDegree = adj[x].size();
                    u = x;
                }
            }
            
            visited.insert(v);
            if (INVALID == u) {
                if (vDegree != (n-1)) {
                    // in this case "v" must be root or a node in the "node chain segment" that contains a root
                    return 0;
                }
                continue;
            }
            
            // check whether "any x in adj[v] is in adj[u]"
            for (auto x : adj[v]) {
                if (x == u) continue;
                if (!adj[u].count(x)) return 0;
            }
            
            if (uDegree == vDegree) multiple = true;
        }
        
        if (multiple) return 2;
        return 1;
    }
};
