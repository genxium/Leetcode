/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> ans;
        if (NULL == root) return ans;
        
        queue<Node*> q;
        q.push(root);
        
        while (!q.empty()) {
            int currentSz = q.size();
            vector<int> currentLayer;
            while (currentSz--) {
                auto expander = q.front(); q.pop();
                currentLayer.push_back(expander->val);
                for (auto chd : expander->children) {
                    q.push(chd);
                }
            }
            ans.push_back(currentLayer);
        }
        
        return ans;
    }
};
