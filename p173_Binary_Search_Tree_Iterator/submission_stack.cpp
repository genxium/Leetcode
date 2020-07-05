/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class BSTIterator {
private:
    stack<TreeNode*> stk;    
    /*
    test case #1
    ["BSTIterator","next","next","hasNext","next","hasNext","next","hasNext","next","hasNext"]
[[[7,3,15,null,null,9,20]],[null],[null],[null],[null],[null],[null],[null],[null],[null]]
    
    test case #2
    ["BSTIterator","next","next","hasNext","next","hasNext","next","hasNext","next","hasNext"]
[[[7,3,15,null,5,9,20]],[null],[null],[null],[null],[null],[null],[null],[null],[null]]
    */
public:
    BSTIterator(TreeNode* root) {
        /*
        The stack serves a similar function ality to the "TreeNode.parent: TreeNode*" pointer.

        Moreover, as we specify that "next()" will return "stk.top()", each invocation of "next()" should also help to "push the left-child of `stk.top()->right` recursively".  
        */
        TreeNode *current = root;
        while (NULL != current) {
            stk.push(current);
            current = current->left;
        }
    }
    
    /** @return the next smallest number */
    int next() {
        TreeNode* current = stk.top(); stk.pop();
        int toRet = current->val;
        // If "current->right" exists, the next one must be "leftmost, deepest child of current->right".
        // Otherwise the next one should be "current->parent", which is by now still sitting on stack(not yet popped).
        if (NULL != current->right) {
            current = current->right;
            while (NULL != current) {
                stk.push(current);
                current = current->left;
            }
        }
        return toRet;
    }
    
    /** @return whether we have a next smallest number */
    bool hasNext() {
        if (false == stk.empty()) {
            return true;
        }
        return false;
    }
};

/**
 * Your BSTIterator object will be instantiated and called as such:
 * BSTIterator* obj = new BSTIterator(root);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */
