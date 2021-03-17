bool debug = false;
typedef unordered_map<int, unordered_map<int, int>> ADJ; 
typedef pair<int, int> PII;
typedef uint64_t ULL;
typedef int64_t LL;
ULL const ULLMAX = UINT64_MAX;
LL const LLMAX = INT64_MAX;

void dijkstra(int start, ADJ &adj, unordered_map<int, int>& g) {        
    priority_queue< PII, vector<PII>, greater<PII> > openHeap;
    unordered_set<int> closedSet;

    // init
    openHeap.push({0, start});
    g[start] = 0;

    // loop
    while (false == openHeap.empty()) {
        auto expander = openHeap.top(); openHeap.pop();
        int minGByFar = expander.first;
        int u = expander.second;            
        closedSet.insert(u);
        auto &nbs = adj[u];
        if (debug) printf("[Dijkstra] Checking expander (minGByFar: %d, u: %d, #successors:%d)\n", minGByFar, u, nbs.size());
        for (auto &nb : nbs) {
            int v = nb.first;
            if (closedSet.count(v)) continue;
            int candidate = minGByFar + nb.second;
            if (!g.count(v) || candidate < g[v]) {
                g[v] = candidate;
                openHeap.push({candidate, v});
            }
        }
    }    
}

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    void fillGraph(TreeNode* root, ADJ &adj) {
        if (!root) return;
        int u = root->val;
        if (root->left) {
            int v = root->left->val;
            adj[u][v] = 1;
            adj[v][u] = 1;
            
            fillGraph(root->left, adj);
        } 
        if (root->right) {
            int v = root->right->val;
            adj[u][v] = 1;
            adj[v][u] = 1;
            
            fillGraph(root->right, adj);
        }
    }
    
    vector<int> distanceK(TreeNode* root, TreeNode* target, int K) {
        int src = target->val;
        ADJ adj;
        fillGraph(root, adj);
        
        unordered_map<int, int> g;
        dijkstra(src, adj, g);
        
        vector<int> ans;
        for (auto &[id, distance] : g) {
            if (distance == K) ans.push_back(id);
        }
        
        return ans;
    }
};
