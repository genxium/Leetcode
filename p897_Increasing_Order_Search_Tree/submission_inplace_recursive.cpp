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
bool debug = false;
class Solution {
public:
    pair<TreeNode*, TreeNode*> dfs(TreeNode* root, int lv) {
        TreeNode *newHead = root, *newTail = root;
        int indent = lv*4;
        if (debug) {
            printf("%*schecking root->val: %d\n", indent, "", root->val);
        }
        
        if (root->left) {
            pair<TreeNode*, TreeNode*> lPair = dfs(root->left, lv+1);
            TreeNode *lHead = lPair.first, *lTail = lPair.second;
            newHead = lHead;
            lTail->right = root;
            root->left = NULL;
        }
        
        if (root->right) {
            pair<TreeNode*, TreeNode*> rPair = dfs(root->right, lv+1);
            TreeNode *rHead = rPair.first, *rTail = rPair.second;
            root->right = rHead;
            newTail = rTail;     
        }
        
        return {newHead, newTail};
    }
    
    TreeNode* increasingBST(TreeNode* root) {
        pair<TreeNode*, TreeNode*> ptt = dfs(root, 0);
        return ptt.first;
    }
};
