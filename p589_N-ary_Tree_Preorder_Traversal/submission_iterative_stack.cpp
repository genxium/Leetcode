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
bool const debug = false;
typedef vector<Node*>::iterator NodeIt;

/*
test cases
[]
[1,null,3,2,4,null,5,6]
[1,null,2,3,4,5,null,null,6,7,null,8,null,9,10,null,null,11,null,12,null,13,null,null,14]
*/
class Solution {
public:
    vector<int> preorder(Node* root) {
        stack<pair<Node*, NodeIt>> stk;
        vector<int> ans;
        if (!root) return ans;

        // init
        stk.push({root, root->children.begin()});
        ans.push_back(root->val);

        // loop 
        while (!stk.empty()) {
            while (stk.top().second != stk.top().first->children.end()) {
                Node* newNode = *(stk.top().second);
                ++stk.top().second;
                if (debug) printf("adding val: %d\n", newNode->val);
                ans.push_back(newNode->val);
                stk.push({newNode, newNode->children.begin()});
            }
            if (debug) printf("popping val: %d\n", (stk.top().first)->val);

            stk.pop(); // no more unvisited child for stk.top().first
        }

        return ans;
    }
};
