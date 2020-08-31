class Solution {
public:
    vector<int> findSmallestSetOfVertices(int n, vector<vector<int>>& edges) {
        unordered_set<int> roots;
        for (int i = 0; i < n; ++i) {
            roots.insert(i);
        }
        
        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            roots.erase(v);
        }
        
        vector<int> ans;
        for (auto &u : roots) {
            ans.push_back(u);
        }
        
        return ans;
    }
};
