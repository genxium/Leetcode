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
    void lowestCommonAncestorExtra(TreeNode* root, TreeNode* p, TreeNode* q, bool &foundP, bool &foundQ, TreeNode* &ans) {
        if (NULL != ans) return;
        if (NULL == root) return;
        bool preFoundP = foundP;
        if (root == p) {
            foundP = true;
        }
        
        bool preFoundQ = foundQ;
        if (root == q) {
            foundQ = true;
        }
        
        lowestCommonAncestorExtra(root->left, p, q, foundP, foundQ, ans);
        if (NULL != ans) {
            // ans is in the left subtree
            return;
        }

        lowestCommonAncestorExtra(root->right, p, q, foundP, foundQ, ans);
        if (NULL != ans) {
            // ans is in the left subtree
            return;
        }
        
        if (!preFoundP && foundP && !preFoundQ && foundQ) {
            ans = root;
            return;
        }
    }
    
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        TreeNode* ans = NULL;
        bool foundP = false, foundQ = false;
        lowestCommonAncestorExtra(root, p, q, foundP, foundQ, ans);
        return ans;
    }
};
