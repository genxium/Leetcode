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
class Solution {
public:
    int longestUnivalueDownPath(TreeNode* root, int* pAns) {
        if (!root) return 0;
        int toRet = 1;
        /*
        [WARNING]
        
        Must call "longestUnivalueDownPath(...)" on "root->left" and "root->right" UNCONDITIONALLY to guarantee a full traversal of the tree!
        */
        int candL = longestUnivalueDownPath(root->left, pAns);
        int candR = longestUnivalueDownPath(root->right, pAns);
        int lDown = 0, rDown = 0;
        if (root->left && root->val == root->left->val) {
            lDown = 1+candL;
        }
        if (root->right && root->val == root->right->val) {
            rDown = 1+candR;
        }
        *pAns = max(*pAns, lDown+rDown);
        return max(lDown, rDown);
    }
    
    int longestUnivaluePath(TreeNode* root) {
        int ans = 0;
        longestUnivalueDownPath(root, &ans);
        return ans;
    }
};
